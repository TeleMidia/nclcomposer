/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "StructuralViewPlugin.h"

namespace util = StructuralUtil;

#define _core2view_insert(key, value)                                         \
  _core2view.insert (key, value);                                             \
  _view2core.insert (value, key);

#define _core2view_remove(key)                                                \
  _view2core.remove (_core2view.value (key));                                 \
  _core2view.remove (key);

#define _core2view_clear()                                                    \
  _core2view.clear ();                                                        \
  _view2core.clear ();

StructuralViewPlugin::StructuralViewPlugin (QObject *parent)
    : _window (new StructuralWindow ())
{
  Q_UNUSED (parent);

  _struct_view = _window->getView ();
  _struct_scene = _struct_view->getScene ();

  createConnections ();

  _syncing = false;
  _waiting = false;

  _notified = ""; // last entity uid notified by the view
  _selected = "";
}

StructuralViewPlugin::~StructuralViewPlugin () { delete _window; }

void
StructuralViewPlugin::createConnections ()
{
  connect (_struct_view, &StructuralView::inserted, this,
           &StructuralViewPlugin::insertInCore, Qt::DirectConnection);

  connect (_struct_view, &StructuralView::removed, this,
           &StructuralViewPlugin::removeInCore, Qt::DirectConnection);

  connect (_struct_view, &StructuralView::changed, this,
           &StructuralViewPlugin::changeInCore, Qt::DirectConnection);

  connect (_struct_view, &StructuralView::selected, this,
           &StructuralViewPlugin::selectInCore, Qt::DirectConnection);

  connect (_struct_view, &StructuralView::updateRequested, this,
           &StructuralViewPlugin::adjustConnectors, Qt::DirectConnection);
}

void
StructuralViewPlugin::init ()
{
  QString data
      = project->getPluginData ("br.puc-rio.telemidia.composer.structural");

  QString sc = "-- EXTRA DATA";
  QString ec = "-- END OF PLUGIN DATA";
  QString c;

  int si = data.indexOf (sc);
  int ei = data.indexOf (ec);
  int ci = si + sc.length ();

  c = data.mid (ci, ei - ci);

  int li = 0;
  int ni = c.indexOf (":");

  while (ni > 0)
  {
    QStringList list = (c.mid (li, ni - li)).split ("=");

    _core2view_insert (list.at (0), list.at (1))

        li
        = ni + 1;
    ni = c.indexOf (":", ni + 1);
  }

  sc = "-- VIEW DATA";
  ec = "-- EXTRA DATA";

  si = data.indexOf (sc);
  ei = data.indexOf (ec);
  ci = si + sc.length ();

  c = data.mid (ci, ei - ci);

  if (!c.isEmpty ())
    _struct_scene->load (c);

  _struct_view->adjustAllReferences();
}

QWidget *
StructuralViewPlugin::getWidget ()
{
  return _window;
}

bool
StructuralViewPlugin::saveSubsession ()
{
  QByteArray data;

  data.append ("-- BEGIN OF PLUGIN DATA");
  data.append ("-- VIEW DATA");
  data.append (_struct_scene->save ());

  data.append ("-- EXTRA DATA");
  for (const QString &key : _core2view.keys ())
    data.append (key + "=" + _core2view[key] + ":");

  data.append ("-- END OF PLUGIN DATA");

  emit setPluginData (data);

  return true;
}

void
StructuralViewPlugin::updateFromModel ()
{
  // Cache the old entity positions.
  QMap<QString, QStrMap> cache;
  for (const QString &key : _struct_scene->getEntities ().keys ())
  {
    StructuralEntity *ent = _struct_scene->getEntity (key);
    CPR_ASSERT_NON_NULL (ent);

    QString id = ent->getId ();
    if (!id.isEmpty ())
    {
      QStrMap old_props = _struct_scene->getEntity (key)->getProperties ();
      QStrMap old_pos_props
          = { { ST_ATTR_ENT_TOP, old_props[ST_ATTR_ENT_TOP] },
              { ST_ATTR_ENT_LEFT, old_props[ST_ATTR_ENT_LEFT] },
              { ST_ATTR_ENT_WIDTH, old_props[ST_ATTR_ENT_WIDTH] },
              { ST_ATTR_ENT_HEIGHT, old_props[ST_ATTR_ENT_HEIGHT] },
              { ST_ATTR_ENT_COLLAPSED, old_props[ST_ATTR_ENT_COLLAPSED] },
              { ST_ATTR_ENT_UNCOLLAPSED_TOP,
                old_props[ST_ATTR_ENT_UNCOLLAPSED_TOP] },
              { ST_ATTR_ENT_UNCOLLAPSED_LEFT,
                old_props[ST_ATTR_ENT_UNCOLLAPSED_LEFT] },
              { ST_ATTR_ENT_UNCOLLAPSED_WIDTH,
                old_props[ST_ATTR_ENT_UNCOLLAPSED_WIDTH] },
              { ST_ATTR_ENT_UNCOLLAPSED_HEIGHT,
                old_props[ST_ATTR_ENT_UNCOLLAPSED_HEIGHT] } };

      cache.insert (id, old_pos_props);
    }
  }

  // Clean the scene.
  _struct_view->clean ();
  clean ();

  // Get the entities from the core again in the required order
  // (nodes->interfaces->edges).
  QVector<Entity *> nodes, interfaces, edges;
  QStack<Entity *> stack;
  stack.push (project);
  while (stack.size ())
  {
    Entity *current = stack.top ();
    stack.pop ();

    for (Entity *child : current->getEntityChildren ())
    {
      StructuralType t = util::strToType (child->getType ());
      switch (t)
      {
        case Structural::Media:
        case Structural::Body:
        case StructuralType::Context:
        case StructuralType::Switch:
        case StructuralType::Link:
          nodes.push_back (child);
          break;

        case Structural::Port:
        case Structural::SwitchPort:
        case Structural::Area:
        case Structural::Property:
          interfaces.push_back (child);
          break;
        case Structural::Bind:
        case Structural::Reference:
        case Structural::Mapping:
          edges.push_back (child);
          break;
        default:
          // do nothing.
          break;
      }

      stack.push (child);
    }
  }

  // Insert the nodes and interfaces entities in the view again.
  for (Entity *ent : nodes)
    insertInView (ent, false);

  for (Entity *ent : interfaces)
    insertInView (ent, false);

  // Insert the edges again.
  for (Entity *ent : edges)
    insertInView (ent);

  // Restore the old position properties.
  QStrMap stgs = util::createSettings (false, false);
  for (StructuralEntity *ent : _struct_scene->getEntities ())
  {
    QString id = ent->getId ();
    if (cache.contains (id))
    {
      QStrMap props = ent->getProperties ();
      for (const QString &key : cache[id].keys ())
        props[key] = cache[id][key];

      _struct_view->change (ent->getUid (), props, stgs);
    }
  }

  _struct_view->adjustAllReferences();
}

void
StructuralViewPlugin::setReferences (QStrMap &props)
{
  if (props.contains (ST_ATTR_REF_REFER_ID))
  {
    QString coreUid = getUidById (props.value (ST_ATTR_REF_REFER_ID));
    CPR_ASSERT (_core2view.contains (coreUid));
    props.insert (ST_ATTR_REF_REFER_UID, _core2view.value (coreUid));
  }

  if (props.contains (ST_ATTR_REF_COMPONENT_ID))
  {
    QString coreUid = getUidById (props.value (ST_ATTR_REF_COMPONENT_ID));
    CPR_ASSERT (_core2view.contains (coreUid));
    props.insert (ST_ATTR_REF_COMPONENT_UID, _core2view.value (coreUid));
  }

  if (props.contains (ST_ATTR_REF_INTERFACE_ID))
  {
    bool hasReferInstead = false;
    QMap<QString, StructuralEntity *> ents = _struct_scene->getEntities ();
    if (ents.contains (props.value (ST_ATTR_REF_COMPONENT_UID)))
    {
      StructuralEntity *ent
          = ents.value (props.value (ST_ATTR_REF_COMPONENT_UID));
      for (StructuralEntity *e : ent->getChildren ())
      {
        if (e->isReference ()
            && e->getId () == props.value (ST_ATTR_REF_INTERFACE_ID))
        {
          props.insert (ST_ATTR_REF_INTERFACE_UID, e->getUid ());
          hasReferInstead = true;
          break;
        }
      }
    }

    if (!hasReferInstead)
    {
      QString coreUid = getUidById (props.value (ST_ATTR_REF_INTERFACE_ID));
      CPR_ASSERT (_core2view.contains (coreUid));
      props.insert (ST_ATTR_REF_INTERFACE_UID, _core2view.value (coreUid));
    }
  }
}

void
StructuralViewPlugin::clean ()
{
  _core2view.clear ();
  _view2core.clear ();
}

void
StructuralViewPlugin::onEntityAdded (const QString &pluginID, Entity *ent)
{
  if (_syncing)
    return;

  if (pluginID != getPluginInstanceID ())
  {
    insertInView (ent);
  }
  else if (_waiting)
  {
    _core2view_insert (ent->getUniqueId (), _notified);

    _waiting = false;
  }
}

void
StructuralViewPlugin::errorMessage (const QString &error)
{
  Q_UNUSED (error);
}

void
StructuralViewPlugin::onEntityChanged (const QString &pluginID, Entity *ent)
{
  if (_syncing)
    return;

  if (pluginID != getPluginInstanceID () && !_waiting)
  {
    changeInView (ent);
  }

  _waiting = false;
}

void
StructuralViewPlugin::onEntityRemoved (const QString &pluginID,
                                       const QString &entID)
{
  if (_syncing)
    return;

  if (pluginID != getPluginInstanceID ())
  {
    removeInView (getProject ()->getEntityById (entID));
    _core2view_remove (entID);
  }
}

void
StructuralViewPlugin::changeSelectedEntity (const QString &pluginID,
                                            void *param)
{
  if (_syncing)
    return;

  if (pluginID != getPluginInstanceID ())
  {
    QString *entUid = (QString *)param;

    if (entUid)
    {
      Entity *ent = getProject ()->getEntityById (*entUid);
      if (ent)
        selectInView (ent);
    }
  }
}

QStrMap
StructuralViewPlugin::getViewPropsFromCoreEntity (const Entity *ent)
{
  QStrMap viewProps;

  StructuralType type = util::strToType (ent->getType ());
  QString coreParentUid = ent->getParentUniqueId ();
  QString viewParentUid = _core2view.value (coreParentUid, "");

  QStrMap transls = util::createCoreTranslations (type);
  if (type != Structural::NoType)
  {
    viewProps = { { ST_ATTR_ENT_TYPE, ent->getType () } };
    for (const QString &key : transls.keys ())
    {
      if (ent->hasAttr (key) && !ent->getAttr (key).isEmpty ())
        viewProps.insert (transls.value (key), ent->getAttr (key));
    }

    if (viewProps.contains (ST_ATTR_REF_COMPONENT_ID))
    {
      QString coreUid
          = getUidById (viewProps.value (ST_ATTR_REF_COMPONENT_ID));
      CPR_ASSERT (_core2view.contains (coreUid));
      viewProps.insert (ST_ATTR_REF_COMPONENT_UID, _core2view.value (coreUid));
    }

    if (viewProps.contains (ST_ATTR_REF_INTERFACE_ID))
    {
      QString coreUid
          = getUidById (viewProps.value (ST_ATTR_REF_INTERFACE_ID));
      CPR_ASSERT (_core2view.contains (coreUid));
      viewProps.insert (ST_ATTR_REF_INTERFACE_UID, _core2view.value (coreUid));
    }

    if (type == Structural::Bind)
    {
      viewParentUid = ent->getParent ()->getParentUniqueId ();
      if (viewProps.contains (ST_ATTR_ENT_ID))
      {
        StructuralRole role
            = util::strToRole (viewProps.value (ST_ATTR_ENT_ID));

        viewProps.insert (ST_ATTR_BIND_ROLE, util::roleToString (role));
        viewProps.insert (ST_ATTR_REF_LINK_UID,
                          _core2view.value (ent->getParentUniqueId ()));

        if (util::isCondition (role))
        {
          if (viewProps.contains (ST_ATTR_REF_INTERFACE_UID))
          {
            viewProps.insert (ST_ATTR_EDGE_ORIG,
                              viewProps.value (ST_ATTR_REF_INTERFACE_UID));
          }
          else if (viewProps.contains (ST_ATTR_REF_COMPONENT_UID))
          {
            viewProps.insert (ST_ATTR_EDGE_ORIG,
                              viewProps.value (ST_ATTR_REF_COMPONENT_UID));
          }

          viewProps.insert (ST_ATTR_EDGE_DEST,
                            _core2view.value (ent->getParentUniqueId ()));
        }
        else if (util::isAction (role))
        {
          if (viewProps.contains (ST_ATTR_REF_INTERFACE_UID))
          {
            viewProps.insert (ST_ATTR_EDGE_DEST,
                              viewProps.value (ST_ATTR_REF_INTERFACE_UID));
          }
          else if (viewProps.contains (ST_ATTR_REF_COMPONENT_UID))
          {
            viewProps.insert (ST_ATTR_EDGE_DEST,
                              viewProps.value (ST_ATTR_REF_COMPONENT_UID));
          }

          viewProps.insert (ST_ATTR_EDGE_ORIG,
                            _core2view.value (ent->getParentUniqueId ()));
        }
      }
    }
    else if (type == Structural::Mapping)
    {
      viewParentUid = ent->getParent ()->getParentUniqueId ();

      viewProps.insert (ST_ATTR_EDGE_ORIG,
                        _core2view.value (ent->getParentUniqueId ()));

      if (viewProps.contains (ST_ATTR_REF_INTERFACE_UID))
      {
        viewProps.insert (ST_ATTR_EDGE_DEST,
                          viewProps.value (ST_ATTR_REF_INTERFACE_UID));
      }
      else if (viewProps.contains (ST_ATTR_REF_COMPONENT_UID))
      {
        viewProps.insert (ST_ATTR_EDGE_DEST,
                          viewProps.value (ST_ATTR_REF_COMPONENT_UID));
      }
    }
  }
  else if (ent->getType () == "linkParam" || ent->getType () == "bindParam")
  {
    StructuralEntity *parent = _struct_scene->getEntity (
        _core2view.value (ent->getParentUniqueId ()));

    QString uid;
    QStrMap next = parent->getProperties ();

    QString name, value;
    if (ent->getType () == "linkParam")
    {
      name = QString (ST_ATTR_LINKPARAM_NAME);
      value = QString (ST_ATTR_LINKPARAM_VALUE);
    }
    else
    {
      name = QString (ST_ATTR_BINDPARAM_NAME);
      value = QString (ST_ATTR_BINDPARAM_VALUE);
    }

    if (ent->hasAttr ("name") && !ent->hasAttr ("value"))
    {
      if (_core2view.contains (ent->getUniqueId ()))
        uid = _core2view.value (ent->getUniqueId ());
      else
        uid = ent->getUniqueId ();

      next[name + ":" + uid] = ent->getAttr ("name");
      next[value + ":" + uid] = ent->getAttr ("value");

      _core2view_insert (uid, uid);
    }
    else if (_core2view.contains (ent->getUniqueId ()))
    {
      uid = _core2view.value (ent->getUniqueId ());

      next[name + ":" + uid] = ent->getAttr ("name");
      next[value + ":" + uid] = ent->getAttr ("value");
    }
  }

  return viewProps;
}

void
StructuralViewPlugin::insertInView (Entity *ent, bool undo)
{
  CPR_ASSERT_NON_NULL (ent);
  CPR_ASSERT (!_core2view.contains (ent->getUniqueId ()));

  QStrMap stgs = util::createSettings (undo, false);
  Structural::Type type = util::strToType (ent->getType ());
  QString uid = ent->getUniqueId ();
  QString coreParentUid = ent->getParentUniqueId ();
  QStrMap props = getViewPropsFromCoreEntity (ent);

  if (type != Structural::NoType)
  {
    // If we are adding a bind, the parent will be the above context (not the
    // link!).
    if (type == Structural::Bind)
    {
      CPR_ASSERT (_core2view.contains (coreParentUid));
      coreParentUid = ent->getParent ()->getParentUniqueId ();
    }

    QString viewParentUid = _core2view.value (coreParentUid, "");

    _core2view_insert (uid, uid);

    if (!undo)
      setReferences (props);

    _struct_view->insert (uid, viewParentUid, props, stgs);
  }
  // LinkParam and BindParam are not represented as independent entities in
  // StructuralView, thus instead of creating a new entity, we need to update
  // the associated link.
  else if (ent->getType () == "linkParam" || ent->getType () == "bindParam")
  {
    if (ent->hasAttr ("name") && ent->hasAttr ("value"))
    {
      CPR_ASSERT (_core2view.contains (coreParentUid));
      QString viewParentUid = _core2view.value (coreParentUid);
      StructuralEntity *parent = _struct_scene->getEntity (viewParentUid);

      _core2view_insert (uid, uid);

      _struct_view->change (parent->getUid (), props, stgs);
    }
  }
  else
  {
    qWarning () << "Trying to insert a entity of type " << ent->getType ()
                << " which is not handled by structural.";
    //    CPR_ASSERT_NOT_REACHED ();
  }
}

void
StructuralViewPlugin::removeInView (Entity *ent, bool undo)
{
  CPR_ASSERT (_core2view.contains (ent->getUniqueId ()));

  QStrMap stgs = util::createSettings (undo, false);
  if (ent->getType () == "linkParam" || ent->getType () == "bindParam")
  {
    QString name, value;
    if (ent->getType () == "linkParam")
    {
      name = QString (ST_ATTR_LINKPARAM_NAME);
      value = QString (ST_ATTR_LINKPARAM_VALUE);
    }
    else
    {
      name = QString (ST_ATTR_BINDPARAM_NAME);
      value = QString (ST_ATTR_BINDPARAM_VALUE);
    }

    StructuralEntity *e = _struct_scene->getEntity (
        _core2view.value (ent->getParentUniqueId ()));
    CPR_ASSERT_NON_NULL (e);

    QString uid = _core2view.value (ent->getUniqueId ());
    QStrMap next = e->getProperties ();

    if (next.contains (name + ":" + uid) || next.contains (value + ":" + uid))
    {
      next.remove (name + ":" + uid);
      next.remove (value + ":" + uid);

      _struct_view->change (e->getUid (), next,
                            util::createSettings (undo, false));
    }
  }
  else
  {
    _struct_view->remove (_core2view.value (ent->getUniqueId ()), stgs);
  }

  _core2view_remove (ent->getUniqueId ());
}

void
StructuralViewPlugin::changeInView (Entity *ent)
{
  Structural::Type type = util::strToType (ent->getType ());

  QStrMap stgs = util::createSettings (true, false);

  if (type != Structural::NoType)
  {
    CPR_ASSERT (_core2view.contains (ent->getUniqueId ()));

    QString uid = _core2view[ent->getUniqueId ()];
    CPR_ASSERT (_struct_scene->hasEntity (uid));

    QStrMap props = getViewPropsFromCoreEntity (ent);
    setReferences (props);

    _struct_view->change (uid, props, stgs);
  }
  else if (ent->getType () == "linkParam" || ent->getType () == "bindParam")
  {
    StructuralEntity *parent = _struct_scene->getEntity (
        _core2view.value (ent->getParentUniqueId ()));

    QString uid = ent->getUniqueId ();
    QStrMap next = getViewPropsFromCoreEntity (ent);

    _core2view_insert (uid, uid);

    _struct_view->change (parent->getUid (), next, stgs);
  }
}

void
StructuralViewPlugin::selectInView (Entity *entity)
{
  QString coreUid = entity->getUniqueId ();
  QString viewUid = _core2view.value (coreUid, "");

  if (_struct_scene->hasEntity (viewUid))
  {
    _struct_view->select (viewUid, util::createSettings ());
  }
  else
  {
    qWarning (CPR_PLUGIN_STRUCT)
        << "Trying to select an entity that is not in the view.";
    _struct_view->select ("", util::createSettings ());
  }
}

QStrMap
StructuralViewPlugin::getCoreAttrsFromStructuralEntity (const QStrMap &props)
{
  StructuralType type = util::strToType (props[ST_ATTR_ENT_TYPE]);

  QStrMap attrs;
  QStrMap transls = util::createPluginTranslations (type);
  for (const QString &key : transls.keys ())
  {
    if (props.contains (key) && !props.value (key).isEmpty ())
      attrs.insert (transls.value (key), props.value (key));
  }

  return attrs;
}

void
StructuralViewPlugin::insertInCore (QString uid, QString parent, QStrMap props,
                                    QStrMap settings)
{
  Q_UNUSED (settings);

  StructuralType type = util::strToType (props[ST_ATTR_ENT_TYPE]);
  Entity *entParent = nullptr;

  if (type == Structural::Bind)
  {
    QString coreUid = _view2core.value (props.value (ST_ATTR_REF_LINK_UID));
    entParent = getProject ()->getEntityById (coreUid);
  }
  else if (type == Structural::Mapping)
  {
    QString coreUid = _view2core.value (props.value (ST_ATTR_EDGE_ORIG));
    entParent = getProject ()->getEntityById (coreUid);
  }
  else if (type == Structural::Body)
  {
    // Check if core already has a 'body' entity. If so, update the references.
    QList<Entity *> list = getProject ()->getEntitiesbyType ("body");
    if (!list.isEmpty ())
    {
      Entity *body = list.first ();

      _view2core.remove (_core2view[body->getUniqueId ()]);
      _core2view[body->getUniqueId ()] = uid;
      _view2core[uid] = body->getUniqueId ();

      return;
    }

    // Check if core already has an 'ncl' entity.
    // If it doesn't, adds and sets entity as parent.
    list = getProject ()->getEntitiesbyType ("ncl");
    if (list.isEmpty ())
    {
      Entity *proj = getProject ();
      CPR_ASSERT_NON_NULL (proj);
      QStrMap attrs;

      emit addEntity ("ncl", proj->getUniqueId (), attrs);
    }

    list = getProject ()->getEntitiesbyType ("ncl");
    CPR_ASSERT (!list.isEmpty ());
    entParent = list.first ();
  }
  else if (parent.isEmpty () && !ST_OPT_WITH_BODY)
  {
    QList<Entity *> list = getProject ()->getEntitiesbyType ("body");
    if (!list.isEmpty ())
      entParent = list.first ();
  }
  else
  {
    entParent = getProject ()->getEntityById (_view2core.value (parent));
  }

  if (entParent)
  {
    QStrMap attrs = getCoreAttrsFromStructuralEntity (props);

    _waiting = true;
    _notified = uid;

    emit addEntity (util::typeToStr (type), entParent->getUniqueId (), attrs);

    if (type == Structural::Link || type == Structural::Bind)
    {

      QString tag, name, value;

      if (type == Structural::Link)
      {
        tag = "linkParam";
        name = QString (ST_ATTR_LINKPARAM_NAME);
        value = QString (ST_ATTR_LINKPARAM_VALUE);
      }
      else
      {
        tag = "bindParam";
        name = QString (ST_ATTR_BINDPARAM_NAME);
        value = QString (ST_ATTR_BINDPARAM_VALUE);
      }

      for (const QString &key : props.keys ())
      {
        if (key.contains (name))
        {
          QString pUid = key.right (key.length () - key.lastIndexOf (':') - 1);

          QStrMap pAttr;
          pAttr.insert ("name", props.value (key));
          pAttr.insert ("value", props.value (value + ":" + pUid));

          _waiting = true;
          _notified = pUid;

          emit addEntity (tag, _view2core.value (uid), pAttr);
        }
      }
    }
  }
}

void
StructuralViewPlugin::removeInCore (QString uid, QStrMap stgs)
{
  Q_UNUSED (stgs);

  if (!_view2core.value (uid, "").isEmpty ())
  {
    emit removeEntity (getProject ()->getEntityById (_view2core.value (uid)));
    _core2view_remove (uid);
  }
}

void
StructuralViewPlugin::selectInCore (QString uid, QStrMap settings)
{
  Q_UNUSED (settings);

  QString coreUid = _view2core.value (uid, "");
  if (!coreUid.isEmpty ())
    emit sendBroadcastMessage ("changeSelectedEntity", new QString (coreUid));
}

void
StructuralViewPlugin::changeInCore (QString uid, QStrMap props,
                                    QStrMap previous, QStrMap settings)
{
  Q_UNUSED (previous);
  Q_UNUSED (settings);

  Entity *ent = getProject ()->getEntityById (_view2core.value (uid));
  if (ent)
  {
    StructuralType type = util::strToType (props[ST_ATTR_ENT_TYPE]);
    QStrMap attrs = getCoreAttrsFromStructuralEntity (props);

    if (type == Structural::Link || type == Structural::Bind)
    {
      QString tag, name, value;
      if (type == Structural::Link)
      {
        tag = "linkParam";
        name = QString (ST_ATTR_LINKPARAM_NAME);
        value = QString (ST_ATTR_LINKPARAM_VALUE);
      }
      else
      {
        tag = "bindParam";
        name = QString (ST_ATTR_BINDPARAM_NAME);
        value = QString (ST_ATTR_BINDPARAM_VALUE);
      }

      QVector<QString> paramUids, paramNames;
      for (Entity *c : ent->getEntityChildren ())
      {
        if (c->getType () == tag)
        {
          paramUids.append (c->getUniqueId ());
          paramNames.append (c->getAttr ("name"));
        }
      }

      for (const QString &key : props.keys ())
      {
        if (key.contains (name))
        {
          QString pUid = key.right (key.length () - key.lastIndexOf (':') - 1);
          QString pName = props.value (key);
          QString pValue = props.value (value + ":" + pUid);

          QStrMap pAttr = { { "name", pName }, { "value", pValue } };

          Entity *param
              = getProject ()->getEntityById (_view2core.value (pUid));

          // Change a param entity in core that has been added by the view.
          if (param)
          {
            if (paramUids.contains (param->getUniqueId ()))
              paramUids.remove (paramUids.indexOf (param->getUniqueId ()));

            _waiting = true;

            emit setAttributes (param, pAttr);

            // Change a param entity in core that has not been added
            // by the view and contains a empty 'value' attribute.
          }
          else if (paramNames.contains (pName))
          {
            int index = paramNames.indexOf (pName);

            param = getProject ()->getEntityById (paramUids.at (index));
            paramUids.remove (index);

            _core2view_insert (param->getUniqueId (), pUid);

            _waiting = true;

            emit setAttributes (param, pAttr);
          }
          else
          {
            _waiting = true;
            _notified = pUid;

            emit addEntity (tag, ent->getUniqueId (), pAttr);
          }
        }
      }

      for (const QString &pUid : paramUids)
        emit removeEntity (getProject ()->getEntityById (pUid));
    }

    _waiting = true;

    emit setAttributes (ent, attrs);
  }
}

void
StructuralViewPlugin::textualStartSync (QString, void *)
{
  _syncing = true;
}

void
StructuralViewPlugin::textualFinishSync (QString, void *)
{
  _syncing = false;
  updateFromModel ();
}

void
StructuralViewPlugin::clearValidationMessages (QString, void *param)
{
  Q_UNUSED (param);

  _struct_view->clearErrors ();
}

void
StructuralViewPlugin::validationError (QString pluginID, void *param)
{
  Q_UNUSED (pluginID);

  if (_syncing)
    return;

  if (param)
  {
    pair<QString, QString> *p = (pair<QString, QString> *)param;

    if (_core2view.contains (p->first) && _struct_scene->hasEntity (p->first))
      _struct_view->setError (_core2view.value (p->first), p->second);
  }
}

QString
StructuralViewPlugin::getUidById (const QString &id)
{
  return getUidById (id, getProject ());
}

QString
StructuralViewPlugin::getUidById (const QString &id, Entity *ent)
{
  QString uid = "";

  if (ent->getType () == "property")
  {
    if (ent->getAttr ("name") == id)
      return ent->getUniqueId ();
  }
  else
  {
    if (ent->getAttr ("id") == id)
      return ent->getUniqueId ();
  }

  for (Entity *child : ent->getEntityChildren ())
  {
    QString result = getUidById (id, child);
    if (result != "")
    {
      uid = result;
      break;
    }
  }

  return uid;
}

QString
StructuralViewPlugin::getUidByName (const QString &name, Entity *entity)
{
  QString uid = "";

  if (entity->getAttr ("name") == name)
    return entity->getUniqueId ();

  for (Entity *child : entity->getEntityChildren ())
  {
    QString result = getUidByName (name, child);

    if (result != "")
    {
      uid = result;
      break;
    }
  }

  return uid;
}

void
StructuralViewPlugin::adjustConnectors ()
{
  // TODO: this function should be called only when a change occurs in
  // <connectorBase>.  Currently, this function is called every time the
  // link's dialog is displayed.
  auto connectorBases = getProject ()->getEntitiesbyType ("connectorBase");
  if (!connectorBases.isEmpty ())
  {
    Entity *connBase = connectorBases.first ();
    QMap<QString, QVector<QString> > conditions, actions, params;

    for (Entity *e : connBase->getEntityChildren ())
    {
      // loading data from local causalConnector
      if (e->getType () == "causalConnector")
      {
        QVector<QString> connConditions, connActions, connParams;
        QString connId = e->getAttr ("id");

        QStack<Entity *> next;
        for (Entity *ec : e->getEntityChildren ())
          next.push (ec);

        while (!next.isEmpty ())
        {
          Entity *current = next.pop ();

          if (current->getType () == "simpleCondition"
              && current->hasAttr ("role"))
            connConditions.append (current->getAttr ("role"));

          if (current->getType () == "attributeAssessment"
              && current->hasAttr ("role"))
            connConditions.append (current->getAttr ("role"));

          if (current->getType () == "simpleAction"
              && current->hasAttr ("role"))
            connActions.append (current->getAttr ("role"));

          if (current->getType () == "connectorParam"
              && current->hasAttr ("name"))
            connParams.append (current->getAttr ("name"));

          if (current->getType () == "compoundCondition"
              || current->getType () == "compoundAction"
              || current->getType () == "assessmentStatement")
          {
            for (Entity *ec : current->getEntityChildren ())
              next.push (ec);
          }
        }

        conditions.insert (connId, connConditions);
        actions.insert (connId, connActions);
        params.insert (connId, connParams);

        // loading data from importBases
      }
      else if (e->getType () == "importBase")
      {
        QString importAlias, importURI, projectURI;

        importAlias = e->getAttr ("alias");
        importURI = e->getAttr ("documentURI");

        // projectURI use '/' as separator
        projectURI = getProject ()->getLocation ();

        QFile importFile (projectURI.left (projectURI.lastIndexOf ("/")) + "/"
                          + importURI);

        if (importFile.open (QIODevice::ReadOnly))
        {
          QDomDocument d;
          if (d.setContent (&importFile))
          {
            QDomElement r = d.firstChildElement (); // <ncl>
            QDomElement h = r.firstChildElement (); // <head>
            QDomElement b;                          // <connectorBase>

            bool hasConnBase = false;

            QDomNodeList hc = h.childNodes ();
            for (int i = 0; i < hc.length (); i++)
            {
              if (hc.item (i).isElement ())
              {

                if (hc.item (i).nodeName () == "connectorBase")
                {
                  b = hc.item (i).toElement ();
                  hasConnBase = true;
                  break;
                }
              }
            }

            if (hasConnBase)
            {
              QDomNodeList bc = b.childNodes ();
              for (int i = 0; i < bc.length (); i++)
              {
                if (bc.item (i).isElement ())
                {
                  if (bc.item (i).nodeName () == "causalConnector")
                  {
                    QString connId;
                    QVector<QString> connConditions;
                    QVector<QString> connActions;
                    QVector<QString> connParams;

                    if (bc.item (i).attributes ().namedItem ("id").isNull ())
                      break;

                    connId = bc.item (i)
                                 .attributes ()
                                 .namedItem ("id")
                                 .nodeValue ();
                    connId = importAlias + "#" + connId;

                    QDomNodeList bcc = bc.item (i).childNodes ();

                    QStack<QDomNode> next;

                    for (int j = 0; j < bcc.length (); j++)
                      next.push (bcc.item (j));

                    while (!next.isEmpty ())
                    {
                      QDomNode current = next.pop ();

                      if (current.nodeName () == "simpleCondition")
                        if (!current.attributes ()
                                 .namedItem ("role")
                                 .isNull ())
                          connConditions.append (current.attributes ()
                                                     .namedItem ("role")
                                                     .nodeValue ());

                      if (current.nodeName () == "attributeAssessment")
                        if (!current.attributes ()
                                 .namedItem ("role")
                                 .isNull ())
                          connConditions.append (current.attributes ()
                                                     .namedItem ("role")
                                                     .nodeValue ());

                      if (current.nodeName () == "simpleAction")
                        if (!current.attributes ()
                                 .namedItem ("role")
                                 .isNull ())
                          connActions.append (current.attributes ()
                                                  .namedItem ("role")
                                                  .nodeValue ());

                      if (current.nodeName () == "connectorParam")
                        if (!current.attributes ()
                                 .namedItem ("name")
                                 .isNull ())
                          connParams.append (current.attributes ()
                                                 .namedItem ("name")
                                                 .nodeValue ());

                      if (current.nodeName () == "compoundCondition"
                          || current.nodeName () == "compoundAction"
                          || current.nodeName () == "assessmentStatement")
                      {
                        QDomNodeList nc = current.childNodes ();
                        for (int j = 0; j < nc.length (); j++)
                          next.push (nc.item (j));
                      }
                    }

                    conditions.insert (connId, connConditions);
                    actions.insert (connId, connActions);
                    params.insert (connId, connParams);
                  }
                }
              }
            }
          }
        }
      }
    }

    _struct_view->getDialog ()->setBase (conditions, actions, params);
  }
}
