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

  _struct_view = _window->view ();
  _struct_scene = _struct_view->scene ();

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

  if (!data.isEmpty ())
    _struct_scene->load (data);

  updateFromModel ();
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
  data.append (_struct_scene->save ());

  emit setPluginData (data);

  return true;
}

void
StructuralViewPlugin::updateFromModel ()
{
  // Cache the old entity positions.
  QMap<QString, QStrMap> cache;
  for (const QString &key : _struct_scene->entities ().keys ())
  {
    StructuralEntity *ent = _struct_scene->entity (key);
    CPR_ASSERT_NON_NULL (ent);

    QString id = ent->id ();
    if (!id.isEmpty ())
    {
      QStrMap old_props = _struct_scene->entity (key)->properties ();
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

    for (Entity *child : current->entityChildren ())
    {
      StructuralType t = util::strToType (child->type ());
      StructuralCategory cat = util::categoryFromType (t);
      switch (cat)
      {
        case StructuralCategory::Node:
          nodes.push_back (child);
          break;

        case StructuralCategory::Interface:
          interfaces.push_back (child);
          break;

        case StructuralCategory::Edge:
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
  for (StructuralEntity *ent : _struct_scene->entities ())
  {
    QString id = ent->id ();
    if (cache.contains (id))
    {
      QStrMap props = ent->properties ();
      for (const QString &key : cache[id].keys ())
        props[key] = cache[id][key];

      _struct_view->change (ent->uid (), props, stgs);
    }
  }

  _struct_view->adjustAllReferences ();
}

void
StructuralViewPlugin::setReferences (QStrMap &props)
{
  if (props.contains (ST_ATTR_REF_REFER_ID))
  {
    QString coreUid = uidById (props.value (ST_ATTR_REF_REFER_ID));
    CPR_ASSERT (_core2view.contains (coreUid));
    props.insert (ST_ATTR_REF_REFER_UID, _core2view.value (coreUid));
  }

  if (props.contains (ST_ATTR_REF_COMPONENT_ID))
  {
    QString coreUid = uidById (props.value (ST_ATTR_REF_COMPONENT_ID));
    CPR_ASSERT (_core2view.contains (coreUid));
    props.insert (ST_ATTR_REF_COMPONENT_UID, _core2view.value (coreUid));
  }

  if (props.contains (ST_ATTR_REF_INTERFACE_ID))
  {
    bool hasReferInstead = false;
    QMap<QString, StructuralEntity *> ents = _struct_scene->entities ();
    if (ents.contains (props.value (ST_ATTR_REF_COMPONENT_UID)))
    {
      StructuralEntity *ent
          = ents.value (props.value (ST_ATTR_REF_COMPONENT_UID));
      for (StructuralEntity *e : ent->children ())
      {
        if (e->isReference ()
            && e->id () == props.value (ST_ATTR_REF_INTERFACE_ID))
        {
          props.insert (ST_ATTR_REF_INTERFACE_UID, e->uid ());
          hasReferInstead = true;
          break;
        }
      }
    }

    if (!hasReferInstead)
    {
      QString coreUid = uidById (props.value (ST_ATTR_REF_INTERFACE_ID));
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
    _core2view_insert (ent->uid (), _notified);

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
StructuralViewPlugin::viewPropsFromCoreEntity (const Entity *ent)
{
  QStrMap viewProps;

  StructuralType type = util::strToType (ent->type ());
  QString coreParentUid = ent->parentUid ();
  QString viewParentUid = _core2view.value (coreParentUid, "");

  QStrMap transls = util::createCoreTranslations (type);
  if (type != Structural::NoType)
  {
    viewProps = { { ST_ATTR_ENT_TYPE, ent->type () } };
    for (const QString &key : transls.keys ())
    {
      if (ent->hasAttr (key) && !ent->attr (key).isEmpty ())
        viewProps.insert (transls.value (key), ent->attr (key));
    }

    if (viewProps.contains (ST_ATTR_REF_COMPONENT_ID))
    {
      QString coreUid = uidById (viewProps.value (ST_ATTR_REF_COMPONENT_ID));
      CPR_ASSERT (_core2view.contains (coreUid));
      viewProps.insert (ST_ATTR_REF_COMPONENT_UID, _core2view.value (coreUid));
    }

    if (viewProps.contains (ST_ATTR_REF_INTERFACE_ID))
    {
      QString coreUid = uidById (viewProps.value (ST_ATTR_REF_INTERFACE_ID));
      CPR_ASSERT (_core2view.contains (coreUid));
      viewProps.insert (ST_ATTR_REF_INTERFACE_UID, _core2view.value (coreUid));
    }

    if (type == Structural::Bind)
    {
      viewParentUid = ent->parent ()->parentUid ();
      if (viewProps.contains (ST_ATTR_ENT_ID))
      {
        StructuralRole role
            = util::strToRole (viewProps.value (ST_ATTR_ENT_ID));

        viewProps.insert (ST_ATTR_BIND_ROLE, util::roleToString (role));
        viewProps.insert (ST_ATTR_REF_LINK_UID,
                          _core2view.value (ent->parentUid ()));

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
                            _core2view.value (ent->parentUid ()));
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
                            _core2view.value (ent->parentUid ()));
        }
      }
    }
    else if (type == Structural::Mapping)
    {
      viewParentUid = ent->parent ()->parentUid ();

      viewProps.insert (ST_ATTR_EDGE_ORIG,
                        _core2view.value (ent->parentUid ()));

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
  else if (ent->type () == "linkParam" || ent->type () == "bindParam")
  {
    StructuralEntity *parent
        = _struct_scene->entity (_core2view.value (ent->parentUid ()));

    QString uid;
    QStrMap next = parent->properties ();

    QString name, value;
    if (ent->type () == "linkParam")
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
      if (_core2view.contains (ent->uid ()))
        uid = _core2view.value (ent->uid ());
      else
        uid = ent->uid ();

      next[name + ":" + uid] = ent->attr ("name");
      next[value + ":" + uid] = ent->attr ("value");

      _core2view_insert (uid, uid);
    }
    else if (_core2view.contains (ent->uid ()))
    {
      uid = _core2view.value (ent->uid ());

      next[name + ":" + uid] = ent->attr ("name");
      next[value + ":" + uid] = ent->attr ("value");
    }
  }

  return viewProps;
}

void
StructuralViewPlugin::insertInView (Entity *ent, bool undo)
{
  CPR_ASSERT_NON_NULL (ent);
  CPR_ASSERT (!_core2view.contains (ent->uid ()));

  QStrMap stgs = util::createSettings (undo, false);
  Structural::Type type = util::strToType (ent->type ());
  QString uid = ent->uid ();
  QString coreParentUid = ent->parentUid ();
  QStrMap props = viewPropsFromCoreEntity (ent);

  if (type != Structural::NoType)
  {
    // If we are adding a bind, the parent will be the above context (not the
    // link!).
    if (type == Structural::Bind)
    {
      CPR_ASSERT (_core2view.contains (coreParentUid));
      coreParentUid = ent->parent ()->parentUid ();
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
  else if (ent->type () == "linkParam" || ent->type () == "bindParam")
  {
    if (ent->hasAttr ("name") && ent->hasAttr ("value"))
    {
      CPR_ASSERT (_core2view.contains (coreParentUid));
      QString viewParentUid = _core2view.value (coreParentUid);
      StructuralEntity *parent = _struct_scene->entity (viewParentUid);

      _core2view_insert (uid, uid);

      _struct_view->change (parent->uid (), props, stgs);
    }
  }
  else
  {
    qWarning () << "Trying to insert a entity of type " << ent->type ()
                << " which is not handled by structural.";
    //    CPR_ASSERT_NOT_REACHED ();
  }
}

void
StructuralViewPlugin::removeInView (Entity *ent, bool undo)
{
  CPR_ASSERT (_core2view.contains (ent->uid ()));

  QStrMap stgs = util::createSettings (undo, false);
  if (ent->type () == "linkParam" || ent->type () == "bindParam")
  {
    QString name, value;
    if (ent->type () == "linkParam")
    {
      name = QString (ST_ATTR_LINKPARAM_NAME);
      value = QString (ST_ATTR_LINKPARAM_VALUE);
    }
    else
    {
      name = QString (ST_ATTR_BINDPARAM_NAME);
      value = QString (ST_ATTR_BINDPARAM_VALUE);
    }

    StructuralEntity *e
        = _struct_scene->entity (_core2view.value (ent->parentUid ()));
    CPR_ASSERT_NON_NULL (e);

    QString uid = _core2view.value (ent->uid ());
    QStrMap next = e->properties ();

    if (next.contains (name + ":" + uid) || next.contains (value + ":" + uid))
    {
      next.remove (name + ":" + uid);
      next.remove (value + ":" + uid);

      _struct_view->change (e->uid (), next,
                            util::createSettings (undo, false));
    }
  }
  else
  {
    _struct_view->remove (_core2view.value (ent->uid ()), stgs);
  }

  _core2view_remove (ent->uid ());
}

void
StructuralViewPlugin::changeInView (Entity *ent)
{
  Structural::Type type = util::strToType (ent->type ());

  QStrMap stgs = util::createSettings (true, false);

  if (type != Structural::NoType)
  {
    CPR_ASSERT (_core2view.contains (ent->uid ()));

    QString uid = _core2view[ent->uid ()];
    CPR_ASSERT (_struct_scene->hasEntity (uid));

    QStrMap props = viewPropsFromCoreEntity (ent);
    setReferences (props);

    _struct_view->change (uid, props, stgs);
  }
  else if (ent->type () == "linkParam" || ent->type () == "bindParam")
  {
    StructuralEntity *parent
        = _struct_scene->entity (_core2view.value (ent->parentUid ()));

    QString uid = ent->uid ();
    QStrMap next = viewPropsFromCoreEntity (ent);

    _core2view_insert (uid, uid);

    _struct_view->change (parent->uid (), next, stgs);
  }
}

void
StructuralViewPlugin::selectInView (Entity *entity)
{
  QString coreUid = entity->uid ();
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
StructuralViewPlugin::coreAttrsFromStructuralEntity (const QStrMap &props)
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
      _core2view_remove (body->uid ());
      _core2view_insert (body->uid (), uid);
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

      emit addEntity ("ncl", proj->uid (), attrs);
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
    QStrMap attrs = coreAttrsFromStructuralEntity (props);

    _waiting = true;
    _notified = uid;

    emit addEntity (util::typeToStr (type), entParent->uid (), attrs);

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

  QString coreUid = _view2core.value (uid, "");
  if (!coreUid.isEmpty ())
  {
    Entity *ent = getProject ()->getEntityById (coreUid);
    CPR_ASSERT_NON_NULL (ent);

    emit removeEntity (ent);
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
    QStrMap attrs = coreAttrsFromStructuralEntity (props);

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
      for (Entity *c : ent->entityChildren ())
      {
        if (c->type () == tag)
        {
          paramUids.append (c->uid ());
          paramNames.append (c->attr ("name"));
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
            if (paramUids.contains (param->uid ()))
              paramUids.remove (paramUids.indexOf (param->uid ()));

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

            _core2view_insert (param->uid (), pUid);

            _waiting = true;

            emit setAttributes (param, pAttr);
          }
          else
          {
            _waiting = true;
            _notified = pUid;

            emit addEntity (tag, ent->uid (), pAttr);
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
StructuralViewPlugin::uidById (const QString &id, Entity *ent)
{
  QString uid = "";
  if (ent == nullptr)
    ent = getProject ();

  if (ent->type () == "property")
  {
    if (ent->attr ("name") == id)
      return ent->uid ();
  }
  else
  {
    if (ent->attr ("id") == id)
      return ent->uid ();
  }

  for (Entity *child : ent->entityChildren ())
  {
    QString result = uidById (id, child);
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

    for (Entity *e : connBase->entityChildren ())
    {
      // loading data from local causalConnector
      if (e->type () == "causalConnector")
      {
        QVector<QString> connConditions, connActions, connParams;
        QString connId = e->attr ("id");

        QStack<Entity *> next;
        for (Entity *ec : e->entityChildren ())
          next.push (ec);

        while (!next.isEmpty ())
        {
          Entity *current = next.pop ();

          if (current->type () == "simpleCondition"
              && current->hasAttr ("role"))
            connConditions.append (current->attr ("role"));

          if (current->type () == "attributeAssessment"
              && current->hasAttr ("role"))
            connConditions.append (current->attr ("role"));

          if (current->type () == "simpleAction" && current->hasAttr ("role"))
            connActions.append (current->attr ("role"));

          if (current->type () == "connectorParam"
              && current->hasAttr ("name"))
            connParams.append (current->attr ("name"));

          if (current->type () == "compoundCondition"
              || current->type () == "compoundAction"
              || current->type () == "assessmentStatement")
          {
            for (Entity *ec : current->entityChildren ())
              next.push (ec);
          }
        }

        conditions.insert (connId, connConditions);
        actions.insert (connId, connActions);
        params.insert (connId, connParams);

        // loading data from importBases
      }
      else if (e->type () == "importBase")
      {
        QString importAlias, importURI, projectURI;

        importAlias = e->attr ("alias");
        importURI = e->attr ("documentURI");

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

    _struct_view->dialog ()->setBase (conditions, actions, params);
  }
}
