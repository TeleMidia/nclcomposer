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

StructuralViewPlugin::StructuralViewPlugin (QObject *parent)
{
  Q_UNUSED (parent);

  _window = new StructuralWindow ();

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
  StructuralView *view = _window->getView ();

  connect (view, &StructuralView::inserted, this,
           &StructuralViewPlugin::insertInCore, Qt::DirectConnection);

  connect (view, &StructuralView::removed, this,
           &StructuralViewPlugin::removeInCore, Qt::DirectConnection);

  connect (view, &StructuralView::changed, this,
           &StructuralViewPlugin::changeInCore, Qt::DirectConnection);

  connect (view, &StructuralView::selected, this,
           &StructuralViewPlugin::selectInCore, Qt::DirectConnection);

  connect (view, &StructuralView::updateRequested, this,
           &StructuralViewPlugin::adjustConnectors, Qt::DirectConnection);
}

void
StructuralViewPlugin::init ()
{
  QString data
      = project->getPluginData ("br.puc-rio.telemidia.composer.structural");

  QString sc = "-- EXTRA DATA";
  QString ec = "-- END OF PLUGIN DATA";
  QString c = "";

  int si = data.indexOf (sc);
  int ei = data.indexOf (ec);
  int ci = si + sc.length ();

  c = data.mid (ci, ei - ci);

  int li = 0;
  int ni = c.indexOf (":");

  while (ni > 0)
  {
    QStringList list = (c.mid (li, ni - li)).split ("=");

    _coreToView[list.at (0)] = list.at (1);
    _viewToCore[list.at (1)] = list.at (0);

    li = ni + 1;
    ni = c.indexOf (":", ni + 1);
  }

  sc = "-- VIEW DATA";
  ec = "-- EXTRA DATA";
  c = "";

  si = data.indexOf (sc);
  ei = data.indexOf (ec);
  ci = si + sc.length ();

  c = data.mid (ci, ei - ci);

  StructuralView *view = _window->getView ();
  view->load (c);
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
  data.append (_window->getView ()->save ());

  data.append ("-- EXTRA DATA");
  for (const QString &key : _coreToView.keys ())
  {
    data.append (key + "=" + _coreToView[key] + ":");
  }

  data.append ("-- END OF PLUGIN DATA");

  emit setPluginData (data);

  return true;
}

void
StructuralViewPlugin::updateFromModel ()
{
  //
  // Caching...
  //
  QMap<QString, QMap<QString, QString> > cache;

  for (const QString &key : _coreToView.keys ())
  {
    QString viewKey = _coreToView.value (key);

    if (_window->getView ()->getScene()->hasEntity (viewKey))
    {
      StructuralEntity *e = _window->getView ()->getScene()->getEntity (viewKey);

      CPR_ASSERT (e);

      if (!e->getId ().isEmpty ())
      {
        QString cacheId = createCacheId (e);
        QMap<QString, QString> properties = e->getProperties ();

        for (const QString &key2 : e->getProperties ().keys ())
        {
          if (key2.contains (ST_ATTR_LINKPARAM_NAME)
              || key2.contains (ST_ATTR_LINKPARAM_VALUE)
              || key2.contains (ST_ATTR_BINDPARAM_NAME)
              || key2.contains (ST_ATTR_BINDPARAM_VALUE))

            properties.remove (key2);
        }

        cache.insert (e->getId () + cacheId, properties);
      }
    }
  }

  //
  // Cleaning...
  //
  _window->getView ()->clean ();
  clean ();

  //
  // Inserting...
  //
  QStack<Entity *> stack;
  stack.push (project);

  while (stack.size ())
  {
    Entity *current = stack.top ();
    stack.pop ();

    for (Entity *child : current->getEntityChildren ())
    {
      insertInView (child, false);
      stack.push (child);
    }
  }

  //
  // Settings...
  //
  QMap<QString, QString> refs;
  refs[ST_ATTR_REFERENCE_REFER_ID] = ST_ATTR_REFERENCE_REFER_UID;
  refs[ST_ATTR_REFERENCE_COMPONENT_ID] = ST_ATTR_REFERENCE_COMPONENT_UID;
  refs[ST_ATTR_REFERENCE_INTERFACE_ID] = ST_ATTR_REFERENCE_INTERFACE_UID;

  for (StructuralEntity *e : _window->getView ()->getEntities ().values ())
  {
    // When ST_DEFAULT_WITH_INTERFACES is disabled, Structural::Reference
    // entities exists but they are hidden. So, there is no need for
    // adjustment.
    if (e->getType () == Structural::Reference && !ST_DEFAULT_WITH_INTERFACES)
      continue;

    bool hasChange = false;

    for (const QString &r : refs.keys ())
    {
      QString pId = r;
      QString pUid = refs.value (r);

      if (!e->getProperty (pId).isEmpty ())
      {
        QString coreUid = getUidById (e->getProperty (pId));

        if (_coreToView.contains (coreUid))
        {
          e->setProperty (pUid, _coreToView.value (coreUid));
          hasChange = true;
        }
      }
    }

    if (hasChange)
      _window->getView ()->adjustReferences (e);
  }

  QStack<StructuralEntity *> stack2;

  for (StructuralEntity *cur : _window->getView ()->getEntities ().values ())
  {
    if (cur->getParent () == nullptr)
      stack2.push (cur);
  }

  while (!stack2.isEmpty ())
  {
    StructuralEntity *e = stack2.pop ();

    CPR_ASSERT (e != nullptr);

    QString cacheId = createCacheId (e);

    QMap<QString, QString> settings
        = StructuralUtil::createSettings (false, false);

    // When ST_DEFAULT_WITH_INTERFACES is disabled, Structural::Reference
    // entities exists but they are hidden.  So, they could be ignore here.
    // In fact, Structural::Reference are created/removed/changed dynamically
    // based in Structural::Port properties in
    // StructuralView::adjustReferences().
    if (e->getType () == Structural::Reference && !ST_DEFAULT_WITH_INTERFACES)
      continue;

    // Setting cached data...
    if (cache.contains (e->getId () + cacheId))
    {
      QMap<QString, QString> props = cache.value (e->getId () + cacheId);
      props.insert (ST_ATTR_ENT_UID, e->getUid ());

      QMap<QString, QString> transls
          = StructuralUtil::createCoreTranslations (e->getType ());

      for (const QString &transl : transls.values ())
      {
        if (e->getProperty (transl).isEmpty ())
          props.remove (transl);

        if (e->getProperty (transl) != props.value (transl))
          props.insert (transl, e->getProperty (transl));
      }

      if (!e->getProperty (ST_ATTR_REFERENCE_REFER_ID).isEmpty ())
      {
        props[ST_ATTR_REFERENCE_REFER_ID]
            = e->getProperty (ST_ATTR_REFERENCE_REFER_ID);

        if (!e->getProperty (ST_ATTR_REFERENCE_REFER_UID).isEmpty ())
          props[ST_ATTR_REFERENCE_REFER_UID]
              = e->getProperty (ST_ATTR_REFERENCE_REFER_UID);
        else
          props.remove (ST_ATTR_REFERENCE_REFER_UID);
      }

      if (!e->getProperty (ST_ATTR_REFERENCE_COMPONENT_ID).isEmpty ())
      {
        props[ST_ATTR_REFERENCE_COMPONENT_ID]
            = e->getProperty (ST_ATTR_REFERENCE_COMPONENT_ID);

        if (!e->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
          props[ST_ATTR_REFERENCE_COMPONENT_UID]
              = e->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID);
        else
          props.remove (ST_ATTR_REFERENCE_COMPONENT_UID);
      }

      if (!e->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID).isEmpty ())
      {
        props[ST_ATTR_REFERENCE_INTERFACE_ID]
            = e->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID);

        bool hasReferInstead = false;

        if (_window->getView ()->getScene()->hasEntity (
                props.value (ST_ATTR_REFERENCE_COMPONENT_UID)))
        {
          StructuralEntity *re = _window->getView ()->getScene ()->getEntity (
              props.value (ST_ATTR_REFERENCE_COMPONENT_UID));

          for (StructuralEntity *cre : re->getChildren ())
          {
            if (cre->isReference ())
              if (cre->getId ()
                  == props.value (ST_ATTR_REFERENCE_INTERFACE_ID))
              {
                props.insert (ST_ATTR_REFERENCE_INTERFACE_UID, cre->getUid ());
                hasReferInstead = true;
                break;
              }
          }
        }

        if (!hasReferInstead)
        {
          if (!e->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
            props[ST_ATTR_REFERENCE_INTERFACE_UID]
                = e->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID);
          else
            props.remove (ST_ATTR_REFERENCE_INTERFACE_UID);
        }
      }

      // Setting 'bind' cached data...
      if (e->getType () == Structural::Bind)
      {
        if (!props.value (ST_ATTR_ENT_ID).isEmpty ())
        {
          StructuralRole role
              = StructuralUtil::strToRole (props.value (ST_ATTR_ENT_ID));

          QString coreBindUID = _viewToCore.value (e->getUid ());
          QString viewLinkUID
              = _coreToView.value (getProject ()
                                       ->getEntityById (coreBindUID)
                                       ->getParentUniqueId ());

          props.insert (ST_ATTR_REFERENCE_LINK_UID, viewLinkUID);

          if (StructuralUtil::isCondition (role))
          {
            if (!props.value (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            props.value (ST_ATTR_REFERENCE_INTERFACE_UID));
              props.insert (ST_ATTR_EDGE_HEAD,
                            props.value (ST_ATTR_REFERENCE_LINK_UID));
            }
            else if (!props.value (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            props.value (ST_ATTR_REFERENCE_COMPONENT_UID));
              props.insert (ST_ATTR_EDGE_HEAD,
                            props.value (ST_ATTR_REFERENCE_LINK_UID));
            }
          }
          else if (StructuralUtil::isAction (role))
          {
            if (!props.value (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            props.value (ST_ATTR_REFERENCE_LINK_UID));
              props.insert (ST_ATTR_EDGE_HEAD,
                            props.value (ST_ATTR_REFERENCE_INTERFACE_UID));
            }
            else if (!props.value (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            props.value (ST_ATTR_REFERENCE_LINK_UID));
              props.insert (ST_ATTR_EDGE_HEAD,
                            props.value (ST_ATTR_REFERENCE_COMPONENT_UID));
            }
          }

          for (const QString &key : e->getProperties ().keys ())
          {
            if (key.contains (ST_ATTR_BINDPARAM_NAME)
                || key.contains (ST_ATTR_BINDPARAM_VALUE))

              props.insert (key, e->getProperty (key));
          }
        }

        // Setting 'link' cached data...
      }
      else if (e->getType () == Structural::Link)
      {
        for (const QString &key : e->getProperties ().keys ())
        {
          if (key.contains (ST_ATTR_LINKPARAM_NAME)
              || key.contains (ST_ATTR_LINKPARAM_VALUE))

            props.insert (key, e->getProperty (key));
        }
      }

      _window->getView ()->change (e->getUid (), props, e->getProperties (),
                                   settings);

      // Setting non cached data...
    }
    else
    {
      QMap<QString, QString> props = e->getProperties ();

      bool hasChanged = false;

      if (e->getType () == Structural::Port
          || e->getType () == Structural::Bind
          || e->getType () == Structural::Mapping)
      {

        if (props.contains (ST_ATTR_REFERENCE_COMPONENT_ID))
        {
          QString coreUid
              = getUidById (props.value (ST_ATTR_REFERENCE_COMPONENT_ID));

          if (_coreToView.contains (coreUid))
          {
            if (e->getProperty (ST_ATTR_REFERENCE_COMPONENT_UID)
                != _coreToView.value (coreUid))
            {
              props.insert (ST_ATTR_REFERENCE_COMPONENT_UID,
                            _coreToView.value (coreUid));
              hasChanged = true;
            }
          }
        }

        if (props.contains (ST_ATTR_REFERENCE_INTERFACE_ID))
        {
          QString coreUid
              = getUidById (props.value (ST_ATTR_REFERENCE_INTERFACE_ID));

          if (_coreToView.contains (coreUid))
          {
            if (e->getProperty (ST_ATTR_REFERENCE_INTERFACE_UID)
                != _coreToView.value (coreUid))
            {
              props.insert (ST_ATTR_REFERENCE_INTERFACE_UID,
                            _coreToView.value (coreUid));
              hasChanged = true;
            }
          }
        }

        if (e->getType () == Structural::Bind)
        {
          if (StructuralUtil::isCondition (e->getProperty (ST_ATTR_ENT_ID)))
          {
            if (!props.value (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            props.value (ST_ATTR_REFERENCE_INTERFACE_UID));
              props.insert (ST_ATTR_EDGE_HEAD,
                            e->getProperty (ST_ATTR_REFERENCE_LINK_UID));
            }
            else if (!props.value (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            props.value (ST_ATTR_REFERENCE_COMPONENT_UID));
              props.insert (ST_ATTR_EDGE_HEAD,
                            e->getProperty (ST_ATTR_REFERENCE_LINK_UID));
            }
          }
          else if (StructuralUtil::isAction (e->getProperty (ST_ATTR_ENT_ID)))
          {
            if (!props.value (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            e->getProperty (ST_ATTR_REFERENCE_LINK_UID));
              props.insert (ST_ATTR_EDGE_HEAD,
                            props.value (ST_ATTR_REFERENCE_INTERFACE_UID));
            }
            else if (!props.value (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            e->getProperty (ST_ATTR_REFERENCE_LINK_UID));
              props.insert (ST_ATTR_EDGE_HEAD,
                            props.value (ST_ATTR_REFERENCE_COMPONENT_UID));
            }
          }
        }
        else if (e->getType () == Structural::Mapping)
        {
          if (!props.value (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
          {
            props.insert (ST_ATTR_EDGE_HEAD,
                          props.value (ST_ATTR_REFERENCE_INTERFACE_UID));
          }
          else if (!props.value (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
          {
            props.insert (ST_ATTR_EDGE_HEAD,
                          props.value (ST_ATTR_REFERENCE_COMPONENT_UID));
          }
        }
      }

      if (hasChanged)
        _window->getView ()->change (e->getUid (), props, e->getProperties (),
                                     settings);
    }

    for (StructuralEntity *c : e->getChildren ())
      stack2.push (c);
  }

  for (const QString &key : _window->getView ()->getEntities ().keys ())
  {
    if (_window->getView ()->getScene ()->hasEntity (key))
    {
      StructuralEntity *r = _window->getView ()->getScene()->getEntity (key);

      if (r->getCategory () == Structural::Edge
          || r->getType () == Structural::Port)
      {
        _window->getView ()->adjustReferences (r);
      }
    }
  }
}

void
StructuralViewPlugin::setReferences (QMap<QString, QString> &props)
{
  if (props.contains (ST_ATTR_REFERENCE_REFER_ID))
  {
    QString coreUid = getUidById (props.value (ST_ATTR_REFERENCE_REFER_ID));

    if (_coreToView.contains (coreUid))
      props.insert (ST_ATTR_REFERENCE_REFER_UID, _coreToView.value (coreUid));
  }

  if (props.contains (ST_ATTR_REFERENCE_COMPONENT_ID))
  {
    QString coreUid
        = getUidById (props.value (ST_ATTR_REFERENCE_COMPONENT_ID));

    if (_coreToView.contains (coreUid))
      props.insert (ST_ATTR_REFERENCE_COMPONENT_UID,
                    _coreToView.value (coreUid));
  }

  if (props.contains (ST_ATTR_REFERENCE_INTERFACE_ID))
  {
    bool hasReferInstead = false;

    QMap<QString, StructuralEntity *> entities
        = _window->getView ()->getEntities ();

    if (entities.contains (props.value (ST_ATTR_REFERENCE_COMPONENT_UID)))
    {
      StructuralEntity *ent
          = entities.value (props.value (ST_ATTR_REFERENCE_COMPONENT_UID));

      for (StructuralEntity *e : ent->getChildren ())
      {
        if (e->isReference ()
            && e->getId () == props.value (ST_ATTR_REFERENCE_INTERFACE_ID))
        {
          props.insert (ST_ATTR_REFERENCE_INTERFACE_UID, e->getUid ());
          hasReferInstead = true;
          break;
        }
      }
    }

    if (!hasReferInstead)
    {
      QString coreUid
          = getUidById (props.value (ST_ATTR_REFERENCE_INTERFACE_ID));

      if (_coreToView.contains (coreUid))
      {
        props.insert (ST_ATTR_REFERENCE_INTERFACE_UID,
                      _coreToView.value (coreUid));
      }
    }
  }
}

void
StructuralViewPlugin::clean ()
{
  _coreToView.clear ();
  _viewToCore.clear ();
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
    _coreToView[ent->getUniqueId ()] = _notified;
    _viewToCore[_notified] = ent->getUniqueId ();

    _waiting = false;
  }
}

void
StructuralViewPlugin::errorMessage (const QString &error)
{
  Q_UNUSED (error);
}

void
StructuralViewPlugin::onEntityChanged (const QString &pluginID, Entity *entity)
{
  if (_syncing)
    return;

  if (pluginID != getPluginInstanceID () && !_waiting)
  {
    changeInView (entity);
  }

  _waiting = false;
}

void
StructuralViewPlugin::onEntityRemoved (const QString &pluginID,
                                       const QString &entityID)
{
  if (_syncing)
    return;

  if (pluginID != getPluginInstanceID ())
  {
    removeInView (getProject ()->getEntityById (entityID));
    _coreToView.remove (entityID);
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
    QString *entityUID = (QString *)param;

    if (entityUID != NULL)
    {
      Entity *entity = getProject ()->getEntityById (*entityUID);

      if (entity != NULL)
        selectInView (entity);
    }
  }
}

void
StructuralViewPlugin::insertInView (Entity *ent, bool undo)
{
  QMap<QString, QString> props;
  Structural::Type type = StructuralUtil::strToType (ent->getType ());

  if (type != Structural::NoType)
  {
    props[ST_ATTR_ENT_TYPE] = ent->getType ();

    _coreToView[ent->getUniqueId ()] = ent->getUniqueId ();
    _viewToCore[ent->getUniqueId ()] = ent->getUniqueId ();

    QMap<QString, QString> transls
        = StructuralUtil::createCoreTranslations (type);

    if (!transls.isEmpty ())
    {
      for (const QString &key : transls.keys ())
      {
        if (!ent->getAttribute (key).isEmpty ())
          props.insert (transls.value (key), ent->getAttribute (key));
      }

      QMap<QString, QString> settings
          = StructuralUtil::createSettings (undo, false);

      QString parentUid = ent->getParentUniqueId ();
      parentUid = _coreToView.value (ent->getParentUniqueId (), "");

      if (!undo)
        setReferences (props);

      if (props.contains (ST_ATTR_REFERENCE_COMPONENT_ID))
      {
        QString coreUid
            = getUidById (props.value (ST_ATTR_REFERENCE_COMPONENT_ID));

        if (_coreToView.contains (coreUid))
        {
          props.insert (ST_ATTR_REFERENCE_COMPONENT_UID,
                        _coreToView.value (coreUid));
        }
      }

      if (props.contains (ST_ATTR_REFERENCE_INTERFACE_ID))
      {
        QString coreUid
            = getUidById (props.value (ST_ATTR_REFERENCE_INTERFACE_ID));

        if (_coreToView.contains (coreUid))
        {
          props.insert (ST_ATTR_REFERENCE_INTERFACE_UID,
                        _coreToView.value (coreUid));
        }
      }

      if (type == Structural::Bind)
      {
        parentUid = ent->getParent ()->getParentUniqueId ();
        assert (parentUid != NULL);

        if (!props.value (ST_ATTR_ENT_ID).isEmpty ())
        {
          StructuralRole role
              = StructuralUtil::strToRole (props.value (ST_ATTR_ENT_ID));

          props.insert (ST_ATTR_BIND_ROLE,
                        StructuralUtil::roleToString (role));
          props.insert (ST_ATTR_REFERENCE_LINK_UID,
                        _coreToView.value (ent->getParentUniqueId ()));

          if (StructuralUtil::isCondition (role))
          {
            if (!props.value (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            props.value (ST_ATTR_REFERENCE_INTERFACE_UID));
            }
            else if (!props.value (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            props.value (ST_ATTR_REFERENCE_COMPONENT_UID));
            }

            props.insert (ST_ATTR_EDGE_HEAD,
                          _coreToView.value (ent->getParentUniqueId ()));
          }
          else if (StructuralUtil::isAction (role))
          {
            if (!props.value (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_HEAD,
                            props.value (ST_ATTR_REFERENCE_INTERFACE_UID));
            }
            else if (!props.value (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_HEAD,
                            props.value (ST_ATTR_REFERENCE_COMPONENT_UID));
            }

            props.insert (ST_ATTR_EDGE_TAIL,
                          _coreToView.value (ent->getParentUniqueId ()));
          }
        }
      }
      else if (type == Structural::Mapping)
      {
        parentUid = ent->getParent ()->getParentUniqueId ();

        if (!props.value (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
        {
          props.insert (ST_ATTR_EDGE_TAIL,
                        _coreToView.value (ent->getParentUniqueId ()));
          props.insert (ST_ATTR_EDGE_HEAD,
                        props.value (ST_ATTR_REFERENCE_INTERFACE_UID));
        }
        else if (!props.value (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
        {
          props.insert (ST_ATTR_EDGE_TAIL,
                        _coreToView.value (ent->getParentUniqueId ()));
          props.insert (ST_ATTR_EDGE_HEAD,
                        props.value (ST_ATTR_REFERENCE_COMPONENT_UID));
        }
      }

      _window->getView ()->insert (ent->getUniqueId (), parentUid, props,
                                   settings);
    }
  }
  else if (ent->getType () == "linkParam" || ent->getType () == "bindParam")
  {

    if (!ent->getAttribute ("name").isEmpty ()
        && !ent->getAttribute ("value").isEmpty ())
    {

      StructuralEntity *e = _window->getView ()->getScene ()->getEntity (
          _coreToView.value (ent->getParentUniqueId ()));

      QString uid = ent->getUniqueId ();
      QMap<QString, QString> previous = e->getProperties ();
      QMap<QString, QString> next = e->getProperties ();

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

      next[name + ":" + uid] = ent->getAttribute ("name");
      next[value + ":" + uid] = ent->getAttribute ("value");

      _coreToView[uid] = uid;
      _viewToCore[uid] = uid;

      _window->getView ()->change (
          e->getUid (), next, previous,
          StructuralUtil::createSettings (true, false));
    }
  }
}

void
StructuralViewPlugin::removeInView (Entity *entity, bool undo)
{
  if (_coreToView.contains (entity->getUniqueId ()))
  {
    QMap<QString, QString> settings
        = StructuralUtil::createSettings (undo, false);

    if (entity->getType () == "linkParam" || entity->getType () == "bindParam")
    {

      QString name, value;

      if (entity->getType () == "linkParam")
      {
        name = QString (ST_ATTR_LINKPARAM_NAME);
        value = QString (ST_ATTR_LINKPARAM_VALUE);
      }
      else
      {
        name = QString (ST_ATTR_BINDPARAM_NAME);
        value = QString (ST_ATTR_BINDPARAM_VALUE);
      }

      StructuralEntity *e = _window->getView ()->getScene ()->getEntity (
          _coreToView.value (entity->getParentUniqueId ()));

      if (entity != NULL)
      {
        QString uid = _coreToView.value (entity->getUniqueId ());
        QMap<QString, QString> previous = e->getProperties ();
        QMap<QString, QString> next = e->getProperties ();

        if (next.contains (name + ":" + uid)
            || next.contains (value + ":" + uid))
        {
          next.remove (name + ":" + uid);
          next.remove (value + ":" + uid);

          _window->getView ()->change (
              e->getUid (), next, previous,
              StructuralUtil::createSettings (undo, false));
        }
      }
    }
    else
    {

      _window->getView ()->remove (_coreToView[entity->getUniqueId ()],
                                   settings);
    }

    _viewToCore.remove (_coreToView.value (entity->getUniqueId ()));
    _coreToView.remove (entity->getUniqueId ());
  }
}

void
StructuralViewPlugin::changeInView (Entity *ent)
{
  QMap<QString, QString> props;

  Structural::Type type = StructuralUtil::strToType (ent->getType ());

  if (type != Structural::NoType)
  {
    props[ST_ATTR_ENT_TYPE] = ent->getType ();

    QMap<QString, QString> transls
        = StructuralUtil::createCoreTranslations (type);

    for (const QString &key : transls.keys ())
    {
      if (!ent->getAttribute (key).isEmpty ())
        props.insert (transls.value (key), ent->getAttribute (key));
    }

    if (_coreToView.contains (ent->getUniqueId ()))
    {
      setReferences (props);

      if (type == Structural::Bind)
      {
        if (!props.value (ST_ATTR_ENT_ID).isEmpty ())
        {
          StructuralRole role
              = StructuralUtil::strToRole (props.value (ST_ATTR_ENT_ID));

          props.insert (ST_ATTR_BIND_ROLE,
                        StructuralUtil::roleToString (role));
          props.insert (ST_ATTR_REFERENCE_LINK_UID,
                        _coreToView.value (ent->getParentUniqueId ()));

          if (StructuralUtil::isCondition (role))
          {
            if (!props.value (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            props.value (ST_ATTR_REFERENCE_INTERFACE_UID));
              props.insert (ST_ATTR_EDGE_HEAD,
                            _coreToView.value (ent->getParentUniqueId ()));
            }
            else if (!props.value (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            props.value (ST_ATTR_REFERENCE_COMPONENT_UID));
              props.insert (ST_ATTR_EDGE_HEAD,
                            _coreToView.value (ent->getParentUniqueId ()));
            }
          }
          else if (StructuralUtil::isAction (role))
          {
            if (!props.value (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            _coreToView.value (ent->getParentUniqueId ()));
              props.insert (ST_ATTR_EDGE_HEAD,
                            props.value (ST_ATTR_REFERENCE_INTERFACE_UID));
            }
            else if (!props.value (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
            {
              props.insert (ST_ATTR_EDGE_TAIL,
                            _coreToView.value (ent->getParentUniqueId ()));
              props.insert (ST_ATTR_EDGE_HEAD,
                            props.value (ST_ATTR_REFERENCE_COMPONENT_UID));
            }
          }
        }
      }
      else if (type == Structural::Mapping)
      {
        if (!props.value (ST_ATTR_REFERENCE_INTERFACE_UID).isEmpty ())
        {
          props.insert (ST_ATTR_EDGE_TAIL,
                        _coreToView.value (ent->getParentUniqueId ()));
          props.insert (ST_ATTR_EDGE_HEAD,
                        props.value (ST_ATTR_REFERENCE_INTERFACE_UID));
        }
        else if (!props.value (ST_ATTR_REFERENCE_COMPONENT_UID).isEmpty ())
        {
          props.insert (ST_ATTR_EDGE_TAIL,
                        _coreToView.value (ent->getParentUniqueId ()));
          props.insert (ST_ATTR_EDGE_HEAD,
                        props.value (ST_ATTR_REFERENCE_COMPONENT_UID));
        }
      }

      QMap<QString, QString> settings
          = StructuralUtil::createSettings (true, false);

      QString uid = _coreToView[ent->getUniqueId ()];

      if (_window->getView ()->getScene ()->hasEntity (uid))
      {
        _window->getView ()->change (
            uid, props, _window->getView ()->getScene ()->getEntity (uid)->getProperties (),
            settings);
      }
    }
  }
  else if (ent->getType () == "linkParam" || ent->getType () == "bindParam")
  {

    StructuralEntity *e = _window->getView ()->getScene ()->getEntity (
        _coreToView.value (ent->getParentUniqueId ()));

    QString uid;
    QMap<QString, QString> previous = e->getProperties ();
    QMap<QString, QString> next = e->getProperties ();

    QString name;
    QString value;

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

    if (!ent->getAttribute ("name").isEmpty ()
        && !ent->getAttribute ("value").isEmpty ())
    {

      if (_coreToView.contains (ent->getUniqueId ()))
        uid = _coreToView.value (ent->getUniqueId ());
      else
        uid = ent->getUniqueId ();

      next[name + ":" + uid] = ent->getAttribute ("name");
      next[value + ":" + uid] = ent->getAttribute ("value");

      _coreToView[uid] = uid;
      _viewToCore[uid] = uid;

      _window->getView ()->change (
          e->getUid (), next, previous,
          StructuralUtil::createSettings (true, false));
    }
    else if (_coreToView.contains (ent->getUniqueId ()))
    {
      uid = _coreToView.value (ent->getUniqueId ());

      next[name + ":" + uid] = ent->getAttribute ("name");
      next[value + ":" + uid] = ent->getAttribute ("value");

      _window->getView ()->change (
          e->getUid (), next, previous,
          StructuralUtil::createSettings (true, false));
    }
  }
}

void
StructuralViewPlugin::selectInView (Entity *entity)
{
  if (_coreToView.contains (entity->getUniqueId ())
      && _selected != entity->getUniqueId ())
  {
    _window->getView ()->select (_coreToView[entity->getUniqueId ()],
                                 StructuralUtil::createSettings ());
  }
  else
  {
    _window->getView ()->select ("", StructuralUtil::createSettings ());
  }
}

void
StructuralViewPlugin::insertInCore (QString uid, QString parent,
                                    QMap<QString, QString> props,
                                    QMap<QString, QString> settings)
{
  Q_UNUSED (settings);

  StructuralType type = StructuralUtil::strToType (props[ST_ATTR_ENT_TYPE]);

  Entity *entityParent = NULL;

  if (type == Structural::Bind)
  {
    entityParent = getProject ()->getEntityById (
        _viewToCore.value (props.value (ST_ATTR_REFERENCE_LINK_UID)));
  }
  else if (type == Structural::Mapping)
  {
    entityParent = getProject ()->getEntityById (
        _viewToCore.value (props.value (ST_ATTR_EDGE_TAIL)));
  }
  else if (type == Structural::Body)
  {
    QList<Entity *> list;

    // Check if core already has a 'body' entity. If so, update the references.
    list = getProject ()->getEntitiesbyType ("body");

    if (!list.isEmpty ())
    {
      Entity *body = list.first ();

      // Cleaning
      _viewToCore.remove (_coreToView[body->getUniqueId ()]);

      // Updating
      _coreToView[body->getUniqueId ()] = uid;
      _viewToCore[uid] = body->getUniqueId ();

      // Finishing
      return;
    }

    // Check if core already has a 'ncl' entity.
    // If don't, adds and set entity as parent.
    list = getProject ()->getEntitiesbyType ("ncl");

    if (list.isEmpty ())
    {
      Entity *proj = getProject ();
      if (proj != nullptr)
      {
        QMap<QString, QString> attrs;
        emit addEntity ("ncl", proj->getUniqueId (), attrs);
      }

      list = getProject ()->getEntitiesbyType ("ncl");

      if (!list.isEmpty ())
        entityParent = list.first ();
    }
  }
  else if (parent.isEmpty () && !ST_DEFAULT_WITH_BODY)
  {
    QList<Entity *> list = getProject ()->getEntitiesbyType ("body");

    if (!list.isEmpty ())
      entityParent = list.first ();
  }
  else
  {
    entityParent = getProject ()->getEntityById (_viewToCore.value (parent));
  }

  if (entityParent != nullptr)
  {
    QMap<QString, QString> attrs;

    QMap<QString, QString> transls
        = StructuralUtil::createPluginTranslations (type);

    for (const QString &key : transls.keys ())
    {
      if (!props.value (key).isEmpty ())
        attrs.insert (transls.value (key), props.value (key));
    }

    _waiting = true;
    _notified = uid;

    emit addEntity (StructuralUtil::typeToStr (type),
                    entityParent->getUniqueId (), attrs);

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

          QMap<QString, QString> pAttr;
          pAttr.insert ("name", props.value (key));
          pAttr.insert ("value", props.value (value + ":" + pUid));

          _waiting = true;
          _notified = pUid;

          emit addEntity (tag, _viewToCore.value (uid), pAttr);
        }
      }
    }
  }
}

void
StructuralViewPlugin::removeInCore (QString uid,
                                    QMap<QString, QString> settings)
{
  Q_UNUSED (settings);

  if (!_viewToCore.value (uid, "").isEmpty ())
  {
    emit removeEntity (getProject ()->getEntityById (_viewToCore.value (uid)));

    _coreToView.remove (_viewToCore.value (uid));
    _viewToCore.remove (uid);
  }
}

void
StructuralViewPlugin::selectInCore (QString uid,
                                    QMap<QString, QString> settings)
{
  Q_UNUSED (settings);

  if (!_viewToCore.value (uid, "").isEmpty ())
  {
    emit sendBroadcastMessage ("changeSelectedEntity",
                               new QString (_coreToView.key (uid)));
  }
}

void
StructuralViewPlugin::changeInCore (QString uid, QMap<QString, QString> props,
                                    QMap<QString, QString> previous,
                                    QMap<QString, QString> settings)
{
  Q_UNUSED (previous);
  Q_UNUSED (settings);

  Entity *ent = getProject ()->getEntityById (_viewToCore.value (uid));

  if (ent != nullptr)
  {
    QMap<QString, QString> attributes;

    Structural::Type type
        = StructuralUtil::strToType (props[ST_ATTR_ENT_TYPE]);

    QMap<QString, QString> translations
        = StructuralUtil::createPluginTranslations (type);

    for (const QString &key : translations.keys ())
    {
      if (!props.value (key).isEmpty ())
        attributes.insert (translations.value (key), props.value (key));
    }

    // The code below should not be here. The logic was moved to
    // StructuralView::performAutostart (). However, i am keeping this
    // for later review.

    /*
    if (!ST_DEFAULT_WITH_BODY && !ST_DEFAULT_WITH_FLOATING_INTERFACES)
    {
      if (properties.contains (ST_ATTR_ENT_AUTOSTART))
      {
        bool isInterface = false;

        if (properties[ST_ATTR_ENT_CATEGORY]
            == StructuralUtil::translateCategoryToString (
                   Structural::Interface))
        {
          isInterface = true;
        }

        bool hasPort = false;

        QList<Entity *> list = getProject ()->getEntitiesbyType ("port");

        foreach (Entity *current, list)
        {
          if (isInterface
              && current->getAttribute ("interface")
                     == properties[ST_ATTR_ENT_ID])
          {
            hasPort = true;

            if (properties[ST_ATTR_ENT_AUTOSTART] == ST_VALUE_FALSE)
              _window->getView ()->remove (
                  _mapCoreToView[current->getUniqueId ()], settings);
          }
          else if (current->getAttribute ("component")
                       == properties[ST_ATTR_ENT_ID]
                   && current->getAttribute ("interface").isEmpty ())
          {
            hasPort = true;

            if (properties[ST_ATTR_ENT_AUTOSTART] == ST_VALUE_FALSE)
              _window->getView ()->remove (
                  _mapCoreToView[current->getUniqueId ()], settings);
          }
        }

        if (!hasPort
            && (properties[ST_ATTR_ENT_AUTOSTART] == ST_VALUE_TRUE))
        {
          QString parent = _mapCoreToView.value (entity->getParentUniqueId ());

          QMap<QString, QString> pp;
          pp[ST_ATTR_ENT_TYPE]
              = StructuralUtil::translateTypeToString (Structural::Port);
          pp.insert (ST_ATTR_ENT_ID,
                     "p" + properties.value (ST_ATTR_ENT_ID));

          if (isInterface)
          {
            pp.insert (ST_ATTR_REFERENCE_INTERFACE_ID,
                       entity->getAttribute ("id"));
            pp.insert (ST_ATTR_REFERENCE_COMPONENT_ID,
                       ((Entity *)entity->getParent ())->getAttribute ("id"));

            parent = _mapCoreToView.value (
                entity->getParent ()->getParentUniqueId ());
          }
          else
          {
            pp.insert (ST_ATTR_REFERENCE_COMPONENT_ID,
                       properties[ST_ATTR_ENT_ID]);
          }

          setReferences (pp);

          _window->getView ()->insert (StructuralUtil::createUid (), parent,
                                       pp, settings);
        }
      }
    }
    */

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
          paramNames.append (c->getAttribute ("name"));
        }
      }

      for (const QString &key : props.keys ())
      {
        if (key.contains (name))
        {
          QString pUid = key.right (key.length () - key.lastIndexOf (':') - 1);
          QString pName = props.value (key);
          QString pValue = props.value (value + ":" + pUid);

          QMap<QString, QString> pAttr
              = { { "name", pName }, { "value", pValue } };

          Entity *param
              = getProject ()->getEntityById (_viewToCore.value (pUid));

          // Change a param entity in core that
          // has been added by the view.
          if (param != nullptr)
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

            _coreToView[param->getUniqueId ()] = pUid;
            _viewToCore[pUid] = param->getUniqueId ();

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

    emit setAttributes (ent, attributes);
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

  _window->getView ()->cleanErrors ();
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

    if (_coreToView.contains (p->first)
        && _window->getView ()->getScene ()->hasEntity (p->first))
      _window->getView ()->setError (_coreToView.value (p->first), p->second);
  }
}

QString
StructuralViewPlugin::createCacheId (StructuralEntity *ent)
{
  QString cacheId = "";

  if (ent->getParent () != nullptr)
  {
    cacheId = ent->getParent ()->getId ();
  }
  else
  {
    Entity *body = getProject ()->getEntitiesbyType ("body").first ();

    if (body != nullptr)
    {
      cacheId = body->getAttribute ("id");
    }
  }

  if (ent->getType () == Structural::Bind)
  {
    StructuralEntity *l = _window->getView ()->getScene ()->getEntity (
        ent->getProperty (ST_ATTR_REFERENCE_LINK_UID));

    if (l != nullptr)
    {
      cacheId += ent->getProperty (ST_ATTR_REFERENCE_LINK_ID);
    }

    cacheId += ent->getProperty (ST_ATTR_REFERENCE_COMPONENT_ID);
    cacheId += ent->getProperty (ST_ATTR_REFERENCE_INTERFACE_ID);
  }

  return cacheId;
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
    if (ent->getAttribute ("name") == id)
      return ent->getUniqueId ();
  }
  else
  {
    if (ent->getAttribute ("id") == id)
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

  if (entity->getAttribute ("name") == name)
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
  // TODO: this function should be calling only when a change occurs in
  // <connectorBase>. Currently, this function is calling every time the
  // link's dialog is displayed.
  if (!getProject ()->getEntitiesbyType ("connectorBase").isEmpty ())
  {
    Entity *connectorBase
        = getProject ()->getEntitiesbyType ("connectorBase").first ();

    QMap<QString, QVector<QString> > conditions;
    QMap<QString, QVector<QString> > actions;
    QMap<QString, QVector<QString> > params;

    for (Entity *e : connectorBase->getEntityChildren ())
    {
      // loading data from local causalConnector
      if (e->getType () == "causalConnector")
      {
        QString connId;
        QVector<QString> connConditions, connActions, connParams;

        connId = e->getAttribute ("id");

        QStack<Entity *> next;

        for (Entity *ec : e->getEntityChildren ())
          next.push (ec);

        while (!next.isEmpty ())
        {
          Entity *current = next.pop ();

          if (current->getType () == "simpleCondition")
            if (current->hasAttribute ("role"))
              connConditions.append (current->getAttribute ("role"));

          if (current->getType () == "attributeAssessment")
            if (current->hasAttribute ("role"))
              connConditions.append (current->getAttribute ("role"));

          if (current->getType () == "simpleAction")
            if (current->hasAttribute ("role"))
              connActions.append (current->getAttribute ("role"));

          if (current->getType () == "connectorParam")
            if (current->hasAttribute ("name"))
              connParams.append (current->getAttribute ("name"));

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

        importAlias = e->getAttribute ("alias");
        importURI = e->getAttribute ("documentURI");

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

    _window->getView ()->getDialog ()->setBase (conditions, actions, params);
  }
}
