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
  connect (_struct_scene, &StructuralScene::inserted, this,
           &StructuralViewPlugin::insertInCore, Qt::DirectConnection);

  connect (_struct_scene, &StructuralScene::removed, this,
           &StructuralViewPlugin::removeInCore, Qt::DirectConnection);

  connect (_struct_scene, &StructuralScene::changed, this,
           &StructuralViewPlugin::changeInCore, Qt::DirectConnection);

  connect (_struct_view, &StructuralView::entitySelected, this,
           &StructuralViewPlugin::selectInCore, Qt::DirectConnection);

  connect (_struct_view, &StructuralView::updateRequested, this,
           &StructuralViewPlugin::adjustConnectors, Qt::DirectConnection);
}

void
StructuralViewPlugin::init ()
{
  QString data = _project->getPluginData ("br.puc-rio.telemidia.structural");

  if (!data.isEmpty ())
    _struct_scene->load (data);

  updateFromModel ();
}

QWidget *
StructuralViewPlugin::widget ()
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
  for (const QString &key : _struct_scene->nodes ().keys ())
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
  stack.push (_project);
  while (stack.size ())
  {
    Entity *current = stack.top ();
    stack.pop ();

    for (Entity *child : current->entityChildren ())
    {
      StructuralType t = util::strtotype (child->type ());
      StructuralCategory cat = util::categoryfromtype (t);
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
  for (StructuralEntity *ent : _struct_scene->nodes ())
  {
    QString id = ent->id ();
    if (cache.contains (id))
    {
      QStrMap props = ent->properties ();
      for (const QString &key : cache[id].keys ())
        props[key] = cache[id][key];

      _struct_scene->change (ent->uid (), props, stgs);
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
    QMap<QString, StructuralEntity *> nodes = _struct_scene->nodes ();
    if (nodes.contains (props.value (ST_ATTR_REF_COMPONENT_UID)))
    {
      StructuralEntity *ent
          = nodes.value (props.value (ST_ATTR_REF_COMPONENT_UID));
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

  if (pluginID != pluginInstanceID ())
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

  if (pluginID != pluginInstanceID () && !_waiting)
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

  if (pluginID != pluginInstanceID ())
  {
    removeInView (project ()->getEntityById (entID));
    _core2view_remove (entID);
  }
}

void
StructuralViewPlugin::changeSelectedEntity (const QString &pluginID,
                                            void *param)
{
  if (_syncing)
    return;

  if (pluginID != pluginInstanceID ())
  {
    QString *entUid = (QString *)param;

    if (entUid)
    {
      Entity *ent = project ()->getEntityById (*entUid);
      if (ent)
        selectInView (ent);
    }
  }
}

QStrMap
StructuralViewPlugin::viewPropsFromCoreEntity (const Entity *ent)
{
  QStrMap viewProps;

  StructuralType type = util::strtotype (ent->type ());
  QString coreParentUid = ent->parentUid ();
  QString viewParentUid = _core2view.value (coreParentUid, "");

  if (type != Structural::NoType)
  {
    viewProps = { { ST_ATTR_ENT_TYPE, ent->type () } };

    QStrMap transls = util::struct2nclTranslations (type);
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
            = util::strtorole (viewProps.value (ST_ATTR_ENT_ID));

        viewProps.insert (ST_ATTR_BIND_ROLE, util::roletostr (role));
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
  Structural::Type type = util::strtotype (ent->type ());
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

    _struct_scene->insert (uid, viewParentUid, props, stgs);
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

      _struct_scene->change (parent->uid (), props, stgs);
    }
  }
  else
  {
    qWarning (CPR_PLUGIN_STRUCT) << "Trying to insert a entity of type "
                                 << ent->type ()
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

      _struct_scene->change (e->uid (), next,
                             util::createSettings (undo, false));
    }
  }
  else
  {
    _struct_scene->remove (_core2view.value (ent->uid ()), stgs);
  }

  _core2view_remove (ent->uid ());
}

void
StructuralViewPlugin::changeInView (Entity *ent)
{
  Structural::Type type = util::strtotype (ent->type ());

  QStrMap stgs = util::createSettings (true, false);

  if (type != Structural::NoType)
  {
    CPR_ASSERT (_core2view.contains (ent->uid ()));

    QString uid = _core2view[ent->uid ()];
    CPR_ASSERT (_struct_scene->hasEntity (uid));

    QStrMap props = viewPropsFromCoreEntity (ent);
    setReferences (props);

    _struct_scene->change (uid, props, stgs);
  }
  else if (ent->type () == "linkParam" || ent->type () == "bindParam")
  {
    StructuralEntity *parent
        = _struct_scene->entity (_core2view.value (ent->parentUid ()));

    QString uid = ent->uid ();
    QStrMap next = viewPropsFromCoreEntity (ent);

    _core2view_insert (uid, uid);

    _struct_scene->change (parent->uid (), next, stgs);
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
  StructuralType type = util::strtotype (props[ST_ATTR_ENT_TYPE]);

  QStrMap attrs;
  QStrMap transls = util::ncl2structTranslations (type);
  for (const QString &key : transls.keys ())
  {
    if (props.contains (key) && !props.value (key).isEmpty ())
      attrs.insert (transls.value (key), props.value (key));
  }

  return attrs;
}

void
StructuralViewPlugin::insertInCore (QString uid, QString parent, QStrMap props,
                                    QStrMap stngs)
{
  Q_UNUSED (stngs);

  StructuralType type = util::strtotype (props[ST_ATTR_ENT_TYPE]);
  Entity *entParent = nullptr;

  if (type == Structural::Bind)
  {
    QString coreUid = _view2core.value (props.value (ST_ATTR_REF_LINK_UID));
    entParent = project ()->getEntityById (coreUid);
  }
  else if (type == Structural::Mapping)
  {
    QString coreUid = _view2core.value (props.value (ST_ATTR_EDGE_ORIG));
    entParent = project ()->getEntityById (coreUid);
  }
  else if (type == Structural::Body)
  {
    // Check if core already has a 'body' entity. If so, update the references.
    QList<Entity *> list = project ()->getEntitiesbyType ("body");
    if (!list.isEmpty ())
    {
      Entity *body = list.first ();
      _core2view_remove (body->uid ());
      _core2view_insert (body->uid (), uid);
      return;
    }

    // Check if core already has an 'ncl' entity.
    // If it doesn't, adds and sets entity as parent.
    list = project ()->getEntitiesbyType ("ncl");
    if (list.isEmpty ())
    {
      Entity *proj = project ();
      CPR_ASSERT_NON_NULL (proj);
      QStrMap attrs;

      emit addEntity ("ncl", proj->uid (), attrs);
    }

    list = project ()->getEntitiesbyType ("ncl");
    CPR_ASSERT (!list.isEmpty ());
    entParent = list.first ();
  }
  else if (parent.isEmpty () && !ST_OPT_WITH_BODY)
  {
    QList<Entity *> list = project ()->getEntitiesbyType ("body");
    if (!list.isEmpty ())
      entParent = list.first ();
  }
  else
  {
    entParent = project ()->getEntityById (_view2core.value (parent));
  }

  if (entParent)
  {
    QStrMap attrs = coreAttrsFromStructuralEntity (props);

    _waiting = true;
    _notified = uid;

    emit addEntity (util::typetostr (type), entParent->uid (), attrs);

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
    Entity *ent = project ()->getEntityById (coreUid);
    CPR_ASSERT_NON_NULL (ent);

    emit removeEntity (ent);
    _core2view_remove (uid);
  }
}

void
StructuralViewPlugin::selectInCore (QString uid)
{
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

  Entity *ent = project ()->getEntityById (_view2core.value (uid));
  if (ent)
  {
    StructuralType type = util::strtotype (props[ST_ATTR_ENT_TYPE]);
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

          Entity *param = project ()->getEntityById (_view2core.value (pUid));

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

            param = project ()->getEntityById (paramUids.at (index));
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
        emit removeEntity (project ()->getEntityById (pUid));
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
    ent = project ();

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
StructuralViewPlugin::connectorParts (Entity *ent,
                                      QVector<QString> &connConditions,
                                      QVector<QString> &connActions,
                                      QVector<QString> &connParams)
{
  CPR_ASSERT (ent->type () == "causalConnector");

  QStack<const Entity *> next;
  for (const Entity *e : ent->entityChildren ())
    next.push (e);

  while (!next.isEmpty ())
  {
    const Entity *curr = next.pop ();

    if ((curr->type () == "simpleCondition"
         || curr->type () == "attributeAssessment")
        && curr->hasAttr ("role"))
    {
      connConditions.append (curr->attr ("role"));
    }
    else if (curr->type () == "simpleAction" && curr->hasAttr ("role"))
    {
      connActions.append (curr->attr ("role"));
    }
    else if (curr->type () == "connectorParam" && curr->hasAttr ("name"))
    {
      connParams.append (curr->attr ("name"));
    }
    else if (curr->type () == "compoundCondition"
             || curr->type () == "compoundAction"
             || curr->type () == "assessmentStatement")
    {
      for (const Entity *e : curr->entityChildren ())
        next.push (e);
    }
  }
}

void
StructuralViewPlugin::connectorParts (QDomElement connElt,
                                      QVector<QString> &connConditions,
                                      QVector<QString> &connActions,
                                      QVector<QString> &connParams)
{
  QStack<QDomElement> next;
  for_each_qelem_child (role, connElt) next.push (role);

  while (!next.isEmpty ())
  {
    QDomElement curr = next.pop ();

    if ((curr.tagName () == "simpleCondition"
         || curr.tagName () == "attributeAssessment")
        && curr.hasAttribute ("role"))
    {
      connConditions.append (curr.attribute ("role"));
    }
    else if (curr.tagName () == "simpleAction" && curr.hasAttribute ("role"))
    {
      connActions.append (curr.attribute ("role"));
    }
    else if (curr.tagName () == "connectorParam" && curr.hasAttribute ("name"))
    {
      connParams.append (curr.attribute ("name"));
    }
    else if (curr.tagName () == "compoundCondition"
             || curr.nodeName () == "compoundAction"
             || curr.nodeName () == "assessmentStatement")
    {
      for_each_qelem_child (nc, curr) next.push (nc);
    }
  }
}

// \todo this function should be called only when a change occurs in
// <connectorBase>.  Currently, it is called every time the link's dialog is
// shown.
void
StructuralViewPlugin::adjustConnectors ()
{
  auto connectorBases = project ()->getEntitiesbyType ("connectorBase");
  if (!connectorBases.isEmpty ())
  {
    Entity *connBase = connectorBases.first ();
    QMap<QString, QVector<QString> > conditions, actions, params;

    for (Entity *child : connBase->entityChildren ())
    {
      // load data from local connector
      if (child->type () == "causalConnector")
      {
        QString connId = child->attr ("id");
        CPR_ASSERT (!connId.isEmpty ());

        QVector<QString> connConditions, connActions, connParams;
        connectorParts (child, connConditions, connActions, connParams);

        conditions.insert (connId, connConditions);
        actions.insert (connId, connActions);
        params.insert (connId, connParams);
      }
      // load data from importBase
      else if (child->type () == "importBase")
      {
        QString importAlias = child->attr ("alias");
        QString importURI = child->attr ("documentURI");

        // projectURI uses '/' as separator
        QString projectURI = project ()->getLocation ();

        QFile importFile (projectURI.left (projectURI.lastIndexOf ("/")) + "/"
                          + importURI);

        if (importFile.open (QIODevice::ReadOnly))
        {
          QDomDocument doc;
          if (!doc.setContent (&importFile))
            continue;

          QDomElement ncl = doc.firstChildElement ();  // <ncl>
          QDomElement head = ncl.firstChildElement (); // <head>

          for_each_qelem_child_of_type (connBase, head, "connectorBase")
          {
            for_each_qelem_child_of_type (conn, connBase, "causalConnector")
            {
              QVector<QString> connConditions, connActions, connParams;
              QString connId = importAlias + "#" + conn.attribute ("id");

              connectorParts (conn, connConditions, connActions, connParams);

              conditions.insert (connId, connConditions);
              actions.insert (connId, connActions);
              params.insert (connId, connParams);
            }
          }
        }
      }
    }

    _struct_view->dialog ()->setBase (conditions, actions, params);
  }
}
