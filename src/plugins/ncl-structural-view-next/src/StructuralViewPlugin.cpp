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

StructuralViewPlugin::StructuralViewPlugin(QObject* parent)
{
  createWidgets();
  createConnections();

  _synching = false;
  _waiting  = false;

  _notified = "";     // last entity uid notified by the view

  lastSelected = "";
}

StructuralViewPlugin::~StructuralViewPlugin()
{
  delete(_window);
}

void StructuralViewPlugin::createWidgets()
{
  _window = new StructuralWindow();
}

void StructuralViewPlugin::createConnections()
{
  StructuralView* view = _window->getView();

  connect(view, SIGNAL(inserted(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(notifyEntityAddedInView(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
  connect(view, SIGNAL(removed(QString,QMap<QString,QString>)), SLOT(notifyEntityDeletedInView(QString,QMap<QString,QString>)));
  connect(view, SIGNAL(changed(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)),SLOT(notifyEntityChangedInView(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
  connect(view, SIGNAL(selected(QString,QMap<QString,QString>)),SLOT(notifyEntitySelectionInView(QString,QMap<QString,QString>)));

  connect(view, SIGNAL(requestLinkDialogUpdate()), SLOT(updateConnectorsDataInView()));
}

void StructuralViewPlugin::init()
{
  QString data = project->getPluginData("br.puc-rio.telemidia.composer.structural");

  QString sc = "-- EXTRA DATA";
  QString ec = "-- END OF PLUGIN DATA";
  QString c = "";

  int si = data.indexOf(sc);
  int ei = data.indexOf(ec);
  int ci = si + sc.length();

  c = data.mid(ci, ei - ci);

  int li = 0;
  int ni = c.indexOf(":");

  while(ni > 0)
  {
    QStringList list = (c.mid(li,ni - li)).split("="); entities.insert(list.at(0), list.at(1));

    li = ni+1;
    ni = c.indexOf(":", ni+1);
  }

  sc = "-- VIEW DATA";
  ec = "-- EXTRA DATA";
  c = "";

  si = data.indexOf(sc);
  ei = data.indexOf(ec);
  ci = si + sc.length();

  c = data.mid(ci, ei - ci);

  StructuralView* view = _window->getView();
  view->load(c);
}

QWidget* StructuralViewPlugin::getWidget()
{
  return _window;
}

bool StructuralViewPlugin::saveSubsession()
{
  QByteArray data;

  data.append("-- BEGIN OF PLUGIN DATA");
  data.append("-- VIEW DATA");
  data.append(_window->getView()->serialize());
  data.append("-- EXTRA DATA");
  foreach(QString key, entities.keys()){
      data.append(key+"="+entities[key]+":");
  }
  data.append("-- END OF PLUGIN DATA");

  emit setPluginData(data);

  return true;
}

void StructuralViewPlugin::updateFromModel()
{
  QMap <QString, QMap<QString,QString> > prop;

  foreach (QString key, entities.keys())
  {
    StructuralEntity* e = _window->getView()->getEntity(entities.value(key));

    if (e != NULL)
    {
      QString pid = "";

      if (e->getStructuralParent() != NULL)
         pid = e->getStructuralParent()->getStructuralId();

      QMap<QString,QString> tocache = e->getStructuralProperties();

      foreach (QString key, e->getStructuralProperties().keys()) {
        if (key.contains(PLG_ENTITY_LINKPARAM_NAME) ||
            key.contains(PLG_ENTITY_LINKPARAM_VALUE) ||
            key.contains(PLG_ENTITY_BINDPARAM_NAME) ||
            key.contains(PLG_ENTITY_BINDPARAM_VALUE))
          tocache.remove(key);
      }

      prop.insert(e->getStructuralId()+pid, tocache);
    }
  }

  _window->getView()->clearAllData(); clear();

  QStack <Entity *> stack; stack.push(project);
  while(stack.size())
  {
    Entity *current = stack.top();
    stack.pop();

    QVector <Entity *> children = current->getChildren();
    for(int i = 0; i <children.size(); i++)
    {
      requestEntityAddition(children[i], false);
      stack.push(children[i]);
    }
  }

  StructuralEntity* root = _window->getView()->getBody();
  if (root != NULL)
  {
    QStack<StructuralEntity*> s;
    s.push(root);

    while (!s.isEmpty()) {
      StructuralEntity* e = s.pop();

      QString pid = "";

      if (e->getStructuralParent() != NULL)
         pid = e->getStructuralParent()->getStructuralId();

      QMap<QString, QString> settings = StructuralUtil::createSettings(false, false);

      if (prop.contains(e->getStructuralId()+pid)){
        QMap<QString, QString> cacheprop = prop.value(e->getStructuralId()+pid);
        cacheprop.insert(PLG_ENTITY_UID,e->getStructuralUid());

        QMap<QString, QString> m = StructuralUtil::createCoreTranslationMap(e->getStructuralType());

        foreach (QString p, m.values()) {
          if (e->getStructuralProperty(p).isEmpty())
            cacheprop.remove(p);

          if (e->getStructuralProperty(p) != cacheprop.value(p))
            cacheprop.insert(p,e->getStructuralProperty(p));
        }

        if (e->getStructuralType() == Structural::Port){
          if (!cacheprop.value(PLG_ENTITY_COMPONENT_ID).isEmpty()){
            QString coreUID = getUidById(cacheprop.value(PLG_ENTITY_COMPONENT_ID));

            cacheprop.insert(PLG_ENTITY_COMPONENT_UID,entities.value(coreUID));
          }

          if (!cacheprop.value(PLG_ENTITY_INTERFACE_ID).isEmpty()){
            QString coreUID = getUidById(cacheprop.value(PLG_ENTITY_INTERFACE_ID));

            cacheprop.insert(PLG_ENTITY_INTERFACE_UID,entities.value(coreUID));
          }
        }

        if (e->getStructuralType() == Structural::Bind)
        {
          if (!cacheprop.value(PLG_ENTITY_ID).isEmpty()){
            StructuralRole role = StructuralUtil::getStructuralRoleFromStr(cacheprop.value(PLG_ENTITY_ID));

            if (cacheprop.contains(PLG_ENTITY_COMPONENT_ID)){
              QString coreUID = getUidById(cacheprop.value(PLG_ENTITY_COMPONENT_ID));

              if (entities.contains(coreUID))
                cacheprop.insert(PLG_ENTITY_COMPONENT_UID, entities.value(coreUID));
            }

            if (cacheprop.contains(PLG_ENTITY_INTERFACE_ID)){
              QString coreUID = getUidById(cacheprop.value(PLG_ENTITY_INTERFACE_ID));

              if (entities.contains(coreUID)){
                cacheprop.insert(PLG_ENTITY_INTERFACE_UID, entities.value(coreUID));
              }
            }

            QString coreBindUID = entities.key(e->getStructuralUid());
            QString viewLinkUID = entities.value(getProject()->getEntityById(coreBindUID)->getParentUniqueId());

            cacheprop.insert(PLG_ENTITY_LINK_UID, viewLinkUID);

            if (StructuralUtil::isConditionRole(role))
            {
              if (!cacheprop.value(PLG_ENTITY_INTERFACE_UID).isEmpty()){
                cacheprop.insert(PLG_ENTITY_START_UID, cacheprop.value(PLG_ENTITY_INTERFACE_UID));
                cacheprop.insert(PLG_ENTITY_END_UID, cacheprop.value(PLG_ENTITY_LINK_UID));

              }else if (!cacheprop.value(PLG_ENTITY_COMPONENT_UID).isEmpty()){
                cacheprop.insert(PLG_ENTITY_START_UID, cacheprop.value(PLG_ENTITY_COMPONENT_UID));
                cacheprop.insert(PLG_ENTITY_END_UID, cacheprop.value(PLG_ENTITY_LINK_UID));
              }

            }else if (StructuralUtil::isActionRole(role)){
              if (!cacheprop.value(PLG_ENTITY_INTERFACE_UID).isEmpty()){
                cacheprop.insert(PLG_ENTITY_START_UID, cacheprop.value(PLG_ENTITY_LINK_UID));
                cacheprop.insert(PLG_ENTITY_END_UID, cacheprop.value(PLG_ENTITY_INTERFACE_UID));

              }else if (!cacheprop.value(PLG_ENTITY_COMPONENT_UID).isEmpty()){
                cacheprop.insert(PLG_ENTITY_START_UID, cacheprop.value(PLG_ENTITY_LINK_UID));
                cacheprop.insert(PLG_ENTITY_END_UID, cacheprop.value(PLG_ENTITY_COMPONENT_UID));
              }
            }
          }
        }

        if (e->getStructuralType() == Structural::Link){
          foreach (QString key, e->getStructuralProperties().keys()) {
            if (key.contains(PLG_ENTITY_LINKPARAM_NAME) ||
                key.contains(PLG_ENTITY_LINKPARAM_VALUE))
              cacheprop.insert(key, e->getStructuralProperty(key));
          }

        }else if (e->getStructuralType() == Structural::Bind){
          foreach (QString key, e->getStructuralProperties().keys()) {
            if (key.contains(PLG_ENTITY_BINDPARAM_NAME) ||
                key.contains(PLG_ENTITY_BINDPARAM_VALUE))
              cacheprop.insert(key, e->getStructuralProperty(key));
          }
        }

        _window->getView()->change(e->getStructuralUid(),cacheprop,e->getStructuralProperties(),settings);

      }else{
        QMap<QString, QString> p = e->getStructuralProperties();

        bool haschange = false;

        // the code below ensure that new ports draws its references
        // correctly
        if (e->getStructuralType() == Structural::Port){

          if (p.contains(PLG_ENTITY_COMPONENT_ID)){
            QString coreUID = getUidById(p.value(PLG_ENTITY_COMPONENT_ID));

            if (entities.contains(coreUID)) {
              if (e->getStructuralProperty(PLG_ENTITY_COMPONENT_UID) != entities.value(coreUID)){
                p.insert(PLG_ENTITY_COMPONENT_UID, entities.value(coreUID));
                haschange = true;
              }
            }
          }

          if (p.contains(PLG_ENTITY_INTERFACE_ID)){
            QString coreUID = getUidById(p.value(PLG_ENTITY_INTERFACE_ID));

            if (entities.contains(coreUID)){
              if (e->getStructuralProperty(PLG_ENTITY_INTERFACE_UID) != entities.value(coreUID)){
                p.insert(PLG_ENTITY_INTERFACE_UID, entities.value(coreUID));
                haschange = true;
              }
            }
          }

        }else if (e->getStructuralType() == Structural::Bind) {
          if (p.contains(PLG_ENTITY_COMPONENT_ID)){
            QString coreUID = getUidById(p.value(PLG_ENTITY_COMPONENT_ID));

            if (entities.contains(coreUID)) {
              if (e->getStructuralProperty(PLG_ENTITY_COMPONENT_UID) != entities.value(coreUID)){
                p.insert(PLG_ENTITY_COMPONENT_UID, entities.value(coreUID));
                haschange = true;
              }
            }
          }

          if (p.contains(PLG_ENTITY_INTERFACE_ID)){
            QString coreUID = getUidById(p.value(PLG_ENTITY_INTERFACE_ID));

            if (entities.contains(coreUID)){
              if (e->getStructuralProperty(PLG_ENTITY_INTERFACE_UID) != entities.value(coreUID)){
                p.insert(PLG_ENTITY_INTERFACE_UID, entities.value(coreUID));
                haschange = true;
              }
            }
          }

          if (StructuralUtil::isConditionRole(e->getStructuralProperty(PLG_ENTITY_ID)))
          {
            if (!p.value(PLG_ENTITY_INTERFACE_UID).isEmpty()){
              p.insert(PLG_ENTITY_START_UID, p.value(PLG_ENTITY_INTERFACE_UID));
              p.insert(PLG_ENTITY_END_UID, e->getStructuralProperty(PLG_ENTITY_LINK_UID));

            }else if (!p.value(PLG_ENTITY_COMPONENT_UID).isEmpty()){
              p.insert(PLG_ENTITY_START_UID, p.value(PLG_ENTITY_COMPONENT_UID));
              p.insert(PLG_ENTITY_END_UID, e->getStructuralProperty(PLG_ENTITY_LINK_UID));
            }

          }else if (StructuralUtil::isActionRole(e->getStructuralProperty(PLG_ENTITY_ID))){
            if (!p.value(PLG_ENTITY_INTERFACE_UID).isEmpty()){
              p.insert(PLG_ENTITY_START_UID, e->getStructuralProperty(PLG_ENTITY_LINK_UID));
              p.insert(PLG_ENTITY_END_UID, p.value(PLG_ENTITY_INTERFACE_UID));

            }else if (!p.value(PLG_ENTITY_COMPONENT_UID).isEmpty()){
              p.insert(PLG_ENTITY_START_UID, e->getStructuralProperty(PLG_ENTITY_LINK_UID));
              p.insert(PLG_ENTITY_END_UID, p.value(PLG_ENTITY_COMPONENT_UID));
            }
          }
        }else if (e->getStructuralType() == Structural::Mapping) {
          if (p.contains(PLG_ENTITY_COMPONENT_ID)){
            QString coreUID = getUidById(p.value(PLG_ENTITY_COMPONENT_ID));

            if (entities.contains(coreUID)) {
              if (e->getStructuralProperty(PLG_ENTITY_COMPONENT_UID) != entities.value(coreUID)){
                p.insert(PLG_ENTITY_COMPONENT_UID, entities.value(coreUID));
                haschange = true;
              }
            }
          }

          if (p.contains(PLG_ENTITY_INTERFACE_ID)){
            QString coreUID = getUidById(p.value(PLG_ENTITY_INTERFACE_ID));

            if (entities.contains(coreUID)){
              if (e->getStructuralProperty(PLG_ENTITY_INTERFACE_UID) != entities.value(coreUID)){
                p.insert(PLG_ENTITY_INTERFACE_UID, entities.value(coreUID));
                haschange = true;
              }
            }
          }

          if (!p.value(PLG_ENTITY_INTERFACE_UID).isEmpty()){
            p.insert(PLG_ENTITY_END_UID, p.value(PLG_ENTITY_INTERFACE_UID));

          }else if (!p.value(PLG_ENTITY_COMPONENT_UID).isEmpty()){
            p.insert(PLG_ENTITY_END_UID, p.value(PLG_ENTITY_COMPONENT_UID));
          }
        }

        if (haschange)
          _window->getView()->change(e->getStructuralUid(),p,e->getStructuralProperties(),settings);
      }

      foreach (StructuralEntity* c, e->getStructuralEntities()) {
          s.push(c);
      }
    }
  }

  _window->getView()->cleanUndoRedo();
}

void StructuralViewPlugin::clear()
{
  entities.clear();
}

void StructuralViewPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
  // do not add any entity while textual is synching
  if(_synching)
    return;

  if (pluginID != getPluginInstanceID()){
    requestEntityAddition(entity);

  }else if (_waiting) {
      entities[entity->getUniqueId()] = _notified;

      _waiting = false;
  }
}

void StructuralViewPlugin::errorMessage(QString error)
{

}

void StructuralViewPlugin::onEntityChanged(QString pluginID, Entity *entity)
{
  // do not change any entity while textual is synching
  if(_synching)
    return;

  if (pluginID != getPluginInstanceID() && !_waiting){
    requestEntityChange(entity);
  }

  _waiting = false;
}

void StructuralViewPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
  // do not remove any entity while textual is synching
  if(_synching)
    return;

  if (pluginID != getPluginInstanceID())
  {
      requestEntityRemotion(getProject()->getEntityById(entityID));
      entities.remove(entityID);
  }
}

void StructuralViewPlugin::changeSelectedEntity(QString pluginID, void* param)
{
  // do not select any entity while textual is synching
  if(_synching)
    return;

  QString* entityUID = (QString*) param;

  if(entityUID != NULL)
  {
    Entity *entity = getProject()->getEntityById(*entityUID);
    if(entity != NULL)
    {
      requestEntitySelection(entity);
    }
  }
}

void StructuralViewPlugin::requestEntityAddition(Entity* entity, bool enableUndo)
{

  QMap<QString, QString> properties;


  Structural::StructuralType type = StructuralUtil::getnstTypeFromStr(entity->getType());

  if (!DEFAULT_BODY_ENABLE && type == Structural::Body)
    return;

  if (type != Structural::NoType){
    properties[PLG_ENTITY_TYPE] = QString::number(type);

    QMap<QString, QString> m = StructuralUtil::createCoreTranslationMap(type);

    entities[entity->getUniqueId()] = entity->getUniqueId();

    if (!m.isEmpty()){
      foreach (QString key, m.keys()) {
        if (!entity->getAttribute(key).isEmpty())
          properties.insert(m.value(key),entity->getAttribute(key));
      }

      QMap<QString,QString> settings = StructuralUtil::createSettings(enableUndo, false);

      QString parentUID = entity->getParentUniqueId();
      parentUID = entities.value(entity->getParentUniqueId(),"");

      if (!DEFAULT_BODY_ENABLE &&
          parentUID.isEmpty() &&
          type != Structural::Property &&
          type != Structural::Port)
        return;

      if (type == Structural::Port){
        if (properties.contains(PLG_ENTITY_COMPONENT_ID)){
          QString coreUID = getUidById(properties.value(PLG_ENTITY_COMPONENT_ID));

          if (entities.contains(coreUID))
            properties.insert(PLG_ENTITY_COMPONENT_UID, entities.value(coreUID));
        }

        if (properties.contains(PLG_ENTITY_INTERFACE_ID)){
          QString coreUID = getUidById(properties.value(PLG_ENTITY_INTERFACE_ID));

          if (entities.contains(coreUID)){
            properties.insert(PLG_ENTITY_INTERFACE_UID, entities.value(coreUID));
          }
        }
      }

      if (type == Structural::Bind)
      {
        parentUID = entity->getParent()->getParentUniqueId();

        if (!properties.value(PLG_ENTITY_ID).isEmpty()){
          StructuralRole role = StructuralUtil::getStructuralRoleFromStr(properties.value(PLG_ENTITY_ID));

          properties.insert(PLG_ENTITY_ROLE, QString::number(role));

          if (properties.contains(PLG_ENTITY_COMPONENT_ID)){
            QString coreUID = getUidById(properties.value(PLG_ENTITY_COMPONENT_ID));

            if (entities.contains(coreUID))
              properties.insert(PLG_ENTITY_COMPONENT_UID, entities.value(coreUID));
          }

          if (properties.contains(PLG_ENTITY_INTERFACE_ID)){
            QString coreUID = getUidById(properties.value(PLG_ENTITY_INTERFACE_ID));

            if (entities.contains(coreUID)){
              properties.insert(PLG_ENTITY_INTERFACE_UID, entities.value(coreUID));
            }
          }

          properties.insert(PLG_ENTITY_LINK_UID, entities.value(entity->getParentUniqueId()));

          if (StructuralUtil::isConditionRole(role))
          {
            if (!properties.value(PLG_ENTITY_INTERFACE_UID).isEmpty()){
              properties.insert(PLG_ENTITY_START_UID, properties.value(PLG_ENTITY_INTERFACE_UID));
              properties.insert(PLG_ENTITY_END_UID, entities.value(entity->getParentUniqueId()));

            }else if (!properties.value(PLG_ENTITY_COMPONENT_UID).isEmpty()){
              properties.insert(PLG_ENTITY_START_UID, properties.value(PLG_ENTITY_COMPONENT_UID));
              properties.insert(PLG_ENTITY_END_UID, entities.value(entity->getParentUniqueId()));

            }

          }else if (StructuralUtil::isActionRole(role)){
            if (!properties.value(PLG_ENTITY_INTERFACE_UID).isEmpty()){
              properties.insert(PLG_ENTITY_START_UID, entities.value(entity->getParentUniqueId()));
              properties.insert(PLG_ENTITY_END_UID, properties.value(PLG_ENTITY_INTERFACE_UID));

            }else if (!properties.value(PLG_ENTITY_COMPONENT_UID).isEmpty()){
              properties.insert(PLG_ENTITY_START_UID, entities.value(entity->getParentUniqueId()));
              properties.insert(PLG_ENTITY_END_UID, properties.value(PLG_ENTITY_COMPONENT_UID));
            }
          }
        }
      }

      if (type == Structural::Mapping)
      {
        parentUID = entity->getParent()->getParentUniqueId();

        if (properties.contains(PLG_ENTITY_COMPONENT_ID)){
          QString coreUID = getUidById(properties.value(PLG_ENTITY_COMPONENT_ID));

          if (entities.contains(coreUID))
            properties.insert(PLG_ENTITY_COMPONENT_UID, entities.value(coreUID));
        }

        if (properties.contains(PLG_ENTITY_INTERFACE_ID)){
          QString coreUID = getUidById(properties.value(PLG_ENTITY_INTERFACE_ID));

          if (entities.contains(coreUID)){
            properties.insert(PLG_ENTITY_INTERFACE_UID, entities.value(coreUID));
          }
        }

        if (!properties.value(PLG_ENTITY_INTERFACE_UID).isEmpty()){
          properties.insert(PLG_ENTITY_START_UID, entities.value(entity->getParentUniqueId()));
          properties.insert(PLG_ENTITY_END_UID, properties.value(PLG_ENTITY_INTERFACE_UID));

        }else if (!properties.value(PLG_ENTITY_COMPONENT_UID).isEmpty()){
          properties.insert(PLG_ENTITY_START_UID, entities.value(entity->getParentUniqueId()));
          properties.insert(PLG_ENTITY_END_UID, properties.value(PLG_ENTITY_COMPONENT_UID));
        }
      }

      _window->getView()->insert(entity->getUniqueId(), parentUID, properties, settings);
    }

  }else if (entity->getType() == "linkParam"){
    if (!entity->getAttribute("name").isEmpty() &&
        !entity->getAttribute("value").isEmpty()){

      StructuralEntity* link = _window->getView()->getEntity(
            entities.value(entity->getParentUniqueId()));

      QString lpUid = entity->getUniqueId();
      QMap<QString, QString> prev = link->getStructuralProperties();
      QMap<QString, QString> next = link->getStructuralProperties();

      next.insert(QString(PLG_ENTITY_LINKPARAM_NAME)+":"+lpUid, entity->getAttribute("name"));
      next.insert(QString(PLG_ENTITY_LINKPARAM_VALUE)+":"+lpUid, entity->getAttribute("value"));

      entities[lpUid] = lpUid;

      _window->getView()->change(link->getStructuralUid(), next, prev,
                                 StructuralUtil::createSettings(true, false));
    }

  }else if (entity->getType() == "bindParam"){
    if (!entity->getAttribute("name").isEmpty() &&
        !entity->getAttribute("value").isEmpty()){

      StructuralEntity* bind = _window->getView()->getEntity(
            entities.value(entity->getParentUniqueId()));

      QString bpUid = entity->getUniqueId();
      QMap<QString, QString> prev = bind->getStructuralProperties();
      QMap<QString, QString> next = bind->getStructuralProperties();

      next.insert(QString(PLG_ENTITY_BINDPARAM_NAME)+":"+bpUid, entity->getAttribute("name"));
      next.insert(QString(PLG_ENTITY_BINDPARAM_VALUE)+":"+bpUid, entity->getAttribute("value"));

      entities[bpUid] = bpUid;

      _window->getView()->change(bind->getStructuralUid(), next, prev,
                                 StructuralUtil::createSettings(true, false));
    }
  }
}

void StructuralViewPlugin::requestEntityRemotion(Entity* entity, bool enableUndo)
{
    QString entityID = entity->getUniqueId();
    if(entities.contains(entityID))
    {
      QMap<QString,QString> settings = StructuralUtil::createSettings(enableUndo,false);

      if (entity->getType() == "linkParam"){
        StructuralEntity* link = _window->getView()->getEntity(
              entities.value(entity->getParentUniqueId()));

        if (link != NULL){
          QString lpUid = entities.value(entity->getUniqueId());
          QMap<QString, QString> prev = link->getStructuralProperties();
          QMap<QString, QString> next = link->getStructuralProperties();

          if (next.contains(QString(PLG_ENTITY_LINKPARAM_NAME)+":"+lpUid) ||
              next.contains(QString(PLG_ENTITY_LINKPARAM_VALUE)+":"+lpUid)){

            next.remove(QString(PLG_ENTITY_LINKPARAM_NAME)+":"+lpUid);
            next.remove(QString(PLG_ENTITY_LINKPARAM_VALUE)+":"+lpUid);

            _window->getView()->change(link->getStructuralUid(), next, prev,
                                       StructuralUtil::createSettings(true, false));
          }
        }

      }else if (entity->getType() == "bindParam"){
        StructuralEntity* bind = _window->getView()->getEntity(
              entities.value(entity->getParentUniqueId()));

        if (bind != NULL){
          QString bpUid = entities.value(entity->getUniqueId());
          QMap<QString, QString> prev = bind->getStructuralProperties();
          QMap<QString, QString> next = bind->getStructuralProperties();

          if (next.contains(QString(PLG_ENTITY_BINDPARAM_NAME)+":"+bpUid) ||
              next.contains(QString(PLG_ENTITY_BINDPARAM_VALUE)+":"+bpUid)){

            next.remove(QString(PLG_ENTITY_BINDPARAM_NAME)+":"+bpUid);
            next.remove(QString(PLG_ENTITY_BINDPARAM_VALUE)+":"+bpUid);

            _window->getView()->change(bind->getStructuralUid(), next, prev,
                                       StructuralUtil::createSettings(true, false));
          }
        }

      }else{
        _window->getView()->remove(entities[entityID], settings);
      }

      entities.remove(entityID);
    }

}

void StructuralViewPlugin::requestEntityChange(Entity* entity)
{

  QMap<QString, QString> properties;

  Structural::StructuralType type = StructuralUtil::getnstTypeFromStr(entity->getType());

  if (type != Structural::NoType){

  properties[PLG_ENTITY_TYPE] = QString::number(type);

  QMap<QString, QString> m = StructuralUtil::createCoreTranslationMap(type);

  foreach (QString key, m.keys()) {
    if (!entity->getAttribute(key).isEmpty())
      properties.insert(m.value(key),entity->getAttribute(key));
  }

    if(entities.contains(entity->getUniqueId()))
    {
      if (type == Structural::Port){
        if (properties.contains(PLG_ENTITY_COMPONENT_ID)){
          QString coreUID = getUidById(properties.value(PLG_ENTITY_COMPONENT_ID));

          if (entities.contains(coreUID))
            properties.insert(PLG_ENTITY_COMPONENT_UID, entities.value(coreUID));
        }

        if (properties.contains(PLG_ENTITY_INTERFACE_ID)){
          QString coreUID = getUidById(properties.value(PLG_ENTITY_INTERFACE_ID));

          if (entities.contains(coreUID)){
            properties.insert(PLG_ENTITY_INTERFACE_UID, entities.value(coreUID));
          }
        }
      }

      if (type == Structural::Bind)
      {
        if (!properties.value(PLG_ENTITY_ID).isEmpty()){
          StructuralRole role = StructuralUtil::getStructuralRoleFromStr(properties.value(PLG_ENTITY_ID));

          properties.insert(PLG_ENTITY_ROLE, QString::number(role));

          if (properties.contains(PLG_ENTITY_COMPONENT_ID)){
            QString coreUID = getUidById(properties.value(PLG_ENTITY_COMPONENT_ID));

            if (entities.contains(coreUID))
              properties.insert(PLG_ENTITY_COMPONENT_UID, entities.value(coreUID));
          }

          if (properties.contains(PLG_ENTITY_INTERFACE_ID)){
            QString coreUID = getUidById(properties.value(PLG_ENTITY_INTERFACE_ID));

            if (entities.contains(coreUID)){
              properties.insert(PLG_ENTITY_INTERFACE_UID, entities.value(coreUID));
            }
          }

          properties.insert(PLG_ENTITY_LINK_UID, entities.value(entity->getParentUniqueId()));

          if (StructuralUtil::isConditionRole(role))
          {
            if (!properties.value(PLG_ENTITY_INTERFACE_UID).isEmpty()){
              properties.insert(PLG_ENTITY_START_UID, properties.value(PLG_ENTITY_INTERFACE_UID));
              properties.insert(PLG_ENTITY_END_UID, entities.value(entity->getParentUniqueId()));

            }else if (!properties.value(PLG_ENTITY_COMPONENT_UID).isEmpty()){
              properties.insert(PLG_ENTITY_START_UID, properties.value(PLG_ENTITY_COMPONENT_UID));
              properties.insert(PLG_ENTITY_END_UID, entities.value(entity->getParentUniqueId()));
            }

          }else if (StructuralUtil::isActionRole(role)){
            if (!properties.value(PLG_ENTITY_INTERFACE_UID).isEmpty()){
              properties.insert(PLG_ENTITY_START_UID, entities.value(entity->getParentUniqueId()));
              properties.insert(PLG_ENTITY_END_UID, properties.value(PLG_ENTITY_INTERFACE_UID));

            }else if (!properties.value(PLG_ENTITY_COMPONENT_UID).isEmpty()){
              properties.insert(PLG_ENTITY_START_UID, entities.value(entity->getParentUniqueId()));
              properties.insert(PLG_ENTITY_END_UID, properties.value(PLG_ENTITY_COMPONENT_UID));
            }
          }
        }
      }

      if (type == Structural::Mapping)
      {
        if (properties.contains(PLG_ENTITY_COMPONENT_ID)){
          QString coreUID = getUidById(properties.value(PLG_ENTITY_COMPONENT_ID));

          if (entities.contains(coreUID))
            properties.insert(PLG_ENTITY_COMPONENT_UID, entities.value(coreUID));
        }

        if (properties.contains(PLG_ENTITY_INTERFACE_ID)){
          QString coreUID = getUidById(properties.value(PLG_ENTITY_INTERFACE_ID));

          if (entities.contains(coreUID)){
            properties.insert(PLG_ENTITY_INTERFACE_UID, entities.value(coreUID));
          }
        }

        if (!properties.value(PLG_ENTITY_INTERFACE_UID).isEmpty()){
          properties.insert(PLG_ENTITY_START_UID, entities.value(entity->getParentUniqueId()));
          properties.insert(PLG_ENTITY_END_UID, properties.value(PLG_ENTITY_INTERFACE_UID));

        }else if (!properties.value(PLG_ENTITY_COMPONENT_UID).isEmpty()){
          properties.insert(PLG_ENTITY_START_UID, entities.value(entity->getParentUniqueId()));
          properties.insert(PLG_ENTITY_END_UID, properties.value(PLG_ENTITY_COMPONENT_UID));
        }
      }

      QMap<QString,QString> settings = StructuralUtil::createSettings(true,false);

      QString entityPlgUID = entities[entity->getUniqueId()];

     if (_window->getView()->hasEntity(entityPlgUID)){
      _window->getView()->change(entityPlgUID, properties, _window->getView()->getEntity(entityPlgUID)->getStructuralProperties(), settings);
     }
    }

  }else if (entity->getType() == "linkParam"){
    StructuralEntity* link = _window->getView()->getEntity(
          entities.value(entity->getParentUniqueId()));

    QString lpUid;
    QMap<QString, QString> prev = link->getStructuralProperties();
    QMap<QString, QString> next = link->getStructuralProperties();

    if (!entity->getAttribute("name").isEmpty() &&
        !entity->getAttribute("value").isEmpty()){

      if (entities.contains(entity->getUniqueId()))
        lpUid = entities.value(entity->getUniqueId());
      else
        lpUid = entity->getUniqueId();

      entities.insert(entity->getUniqueId(), lpUid);

      next.insert(QString(PLG_ENTITY_LINKPARAM_NAME)+":"+lpUid, entity->getAttribute("name"));
      next.insert(QString(PLG_ENTITY_LINKPARAM_VALUE)+":"+lpUid, entity->getAttribute("value"));

      _window->getView()->change(link->getStructuralUid(), next, prev,StructuralUtil::createSettings(true, false));

    }else if (entities.contains(entity->getUniqueId())){
      lpUid = entities.value(entity->getUniqueId());

      next.remove(QString(PLG_ENTITY_LINKPARAM_NAME)+":"+lpUid);
      next.remove(QString(PLG_ENTITY_LINKPARAM_VALUE)+":"+lpUid);

      _window->getView()->change(link->getStructuralUid(), next, prev,StructuralUtil::createSettings(true, false));
    }

  }else if (entity->getType() == "bindParam"){
    StructuralEntity* bind = _window->getView()->getEntity(
          entities.value(entity->getParentUniqueId()));

    QString bpUid;
    QMap<QString, QString> prev = bind->getStructuralProperties();
    QMap<QString, QString> next = bind->getStructuralProperties();

    if (!entity->getAttribute("name").isEmpty() &&
        !entity->getAttribute("value").isEmpty()){

      if (entities.contains(entity->getUniqueId()))
        bpUid = entities.value(entity->getUniqueId());
      else
        bpUid = entity->getUniqueId();

      entities.insert(entity->getUniqueId(), bpUid);

      next.insert(QString(PLG_ENTITY_BINDPARAM_NAME)+":"+bpUid, entity->getAttribute("name"));
      next.insert(QString(PLG_ENTITY_BINDPARAM_VALUE)+":"+bpUid, entity->getAttribute("value"));

      _window->getView()->change(bind->getStructuralUid(), next, prev,StructuralUtil::createSettings(true, false));

    }else if (entities.contains(entity->getUniqueId())){
      bpUid = entities.value(entity->getUniqueId());

      next.remove(QString(PLG_ENTITY_BINDPARAM_NAME)+":"+bpUid);
      next.remove(QString(PLG_ENTITY_BINDPARAM_VALUE)+":"+bpUid);

      _window->getView()->change(bind->getStructuralUid(), next, prev,StructuralUtil::createSettings(true, false));
    }
  }

}

void StructuralViewPlugin::requestEntitySelection(Entity* entity)
{
  if (entities.contains(entity->getUniqueId()) &&
      lastSelected != entity->getUniqueId())
  {

      QMap<QString,QString> settings = StructuralUtil::createSettings(false,false);
      _window->getView()->select(entities[entity->getUniqueId()], settings);

  }
  else
  {
    _window->getView()->unSelect();
  }
}

void StructuralViewPlugin::notifyEntityAddedInView(const QString uid,
                                                 const QString parent,
                                              QMap<QString, QString> properties,
                                              QMap<QString, QString> settings)
{
  StructuralType type = (StructuralType) properties[PLG_ENTITY_TYPE].toInt();

  Entity* cparent = NULL;

  if (type == Structural::Bind){
    cparent = getProject()->getEntityById(entities.key(properties.value(PLG_ENTITY_LINK_UID)));

  }else if (type == Structural::Mapping){
    cparent = getProject()->getEntityById(entities.key(properties.value(PLG_ENTITY_START_UID)));

  }else if (!parent.isEmpty())
    cparent = getProject()->getEntityById(entities.key(parent));

  else if (type == Structural::Body || !DEFAULT_BODY_ENABLE)
  {
    requestBodyDependence();

    QList<Entity*> list = getProject()->getEntitiesbyType("ncl");
    if (!list.isEmpty())
      cparent = list.first();
    else
      return;

    if (!DEFAULT_BODY_ENABLE){
      list = getProject()->getEntitiesbyType("body");

      if (list.isEmpty()){
        QMap<QString, QString> att;
        att.insert("id","newBody");

        emit addEntity("body",
                       cparent->getUniqueId(), att, false);

        list = getProject()->getEntitiesbyType("body");
      }

      if (!list.isEmpty())
        cparent = list.first();
      else
        return;
    }
  }else
    return;

    QMap<QString, QString> m = StructuralUtil::createViewTranslationMap(type);
    QMap<QString, QString> attributes;

    if(cparent != NULL)
    {
      foreach (QString key, m.keys()) {
        if (!properties.value(key).isEmpty())
          attributes.insert(m.value(key),properties.value(key));
      }

      _waiting = true;
      _notified = uid;
      emit addEntity(StructuralUtil::getStrFromNstType(type),
                     cparent->getUniqueId(), attributes, false);

      if (type == Structural::Link){
        foreach (QString name, properties.keys()) {
          if (name.contains(PLG_ENTITY_LINKPARAM_NAME)){
            QString lpUid = name.right(name.length() - name.lastIndexOf(':') - 1);

            QString lpName = properties.value(name);
            QString lpValue = properties.value(QString(PLG_ENTITY_LINKPARAM_VALUE)+":"+lpUid);

            QMap<QString, QString> lpAttr;
            lpAttr.insert("name", lpName);
            lpAttr.insert("value", lpValue);

            _waiting = true;
            _notified = lpUid;
            emit addEntity("linkParam",entities.key(uid), lpAttr, false);
          }
        }
      }

      if (type == Structural::Bind){
        foreach (QString name, properties.keys()) {
          if (name.contains(PLG_ENTITY_BINDPARAM_NAME)){
            QString bpUid = name.right(name.length() - name.lastIndexOf(':') - 1);

            QString bpName = properties.value(name);
            QString bpValue = properties.value(QString(PLG_ENTITY_BINDPARAM_VALUE)+":"+bpUid);

            QMap<QString, QString> bpAttr;
            bpAttr.insert("name", bpName);
            bpAttr.insert("value", bpValue);

            _waiting = true;
            _notified = bpUid;
            emit addEntity("bindParam",entities.key(uid), bpAttr, false);
          }
        }
      }
    }
}

void StructuralViewPlugin::notifyEntityDeletedInView(const QString uid, QMap<QString, QString> settings)
{
  if (entities.key(uid, "nil") != "nil")
  {
    emit removeEntity(getProject()->getEntityById(entities.key(uid)), false);
    entities.remove(entities.key(uid));
  }
}

void StructuralViewPlugin::notifyEntitySelectionInView(const QString uid, QMap<QString, QString> settings)
{
  if (entities.key(uid, "nil") != "nil"){
    _selectedId = new QString(entities.key(uid));
    emit sendBroadcastMessage("changeSelectedEntity", _selectedId);
  }
}

void StructuralViewPlugin::notifyEntityChangedInView(const QString uid,
                                                     QMap<QString, QString> properties,
                                                     QMap<QString, QString> previous,
                                                     QMap<QString, QString> settings)
{

  Structural::StructuralType type = (Structural::StructuralType) properties[PLG_ENTITY_TYPE].toInt();

  QMap<QString, QString> m = StructuralUtil::createViewTranslationMap(type);
  QMap<QString, QString> attributes;
  Entity* entity = getProject()->getEntityById(entities.key(uid));

  foreach (QString key, m.keys()) {
    if (!properties.value(key).isEmpty())
      attributes.insert(m.value(key),properties.value(key));
  }

  if (type == Structural::Link){
    QVector<QString> coreLinkParamUid;
    QVector<QString> coreLinkParamName;

    foreach (Entity* c, entity->getChildren()) {
      if (c->getType() == "linkParam"){
        coreLinkParamUid.append(c->getUniqueId());
        coreLinkParamName.append(c->getAttribute("name"));
      }
    }

    foreach (QString name, properties.keys()) {
      if (name.contains(PLG_ENTITY_LINKPARAM_NAME)){
        QString lpUid = name.right(name.length() - name.lastIndexOf(':') - 1);

        QString lpName = properties.value(name);
        QString lpValue = properties.value(QString(PLG_ENTITY_LINKPARAM_VALUE)+":"+lpUid);

        QMap<QString, QString> lpAttr;
        lpAttr.insert("name", lpName);
        lpAttr.insert("value", lpValue);

        Entity* bp = getProject()->getEntityById(entities.key(lpUid));

        // Change a linkParam entity in core that
        // has been added by the view.
        if (bp != NULL){
          if (coreLinkParamUid.contains(bp->getUniqueId()))
            coreLinkParamUid.remove(coreLinkParamUid.indexOf(bp->getUniqueId()));

          _waiting = true;
          emit setAttributes(bp, lpAttr, false);

        // Change a linkParam entity in core that has not been added
        // by the view and contains a empty 'value' attribute.
        }else if (coreLinkParamName.contains(lpName)){

          int index = coreLinkParamName.indexOf(lpName);

          bp = getProject()->getEntityById(coreLinkParamUid.at(index));

          coreLinkParamUid.remove(index);

          entities.insert(bp->getUniqueId(), lpUid);

          _waiting = true;
          emit setAttributes(bp, lpAttr, false);

        }else {
          _waiting = true;
          _notified = lpUid;
          emit addEntity("linkParam",entity->getUniqueId(), lpAttr, false);
        }
      }
    }

    foreach (QString lpCoreUid, coreLinkParamUid) {
      emit removeEntity(getProject()->getEntityById(lpCoreUid), false);
    }
  }

  if (type == Structural::Bind){
    QVector<QString> coreBindParamUid;
    QVector<QString> coreBindParamName;

    foreach (Entity* c, entity->getChildren()) {
      if (c->getType() == "bindParam"){
        coreBindParamUid.append(c->getUniqueId());
        coreBindParamUid.append(c->getAttribute("name"));
      }
    }

    foreach (QString name, properties.keys()) {
      if (name.contains(PLG_ENTITY_BINDPARAM_NAME)){
        QString bpUid = name.right(name.length() - name.lastIndexOf(':') - 1);

        QString bpName = properties.value(name);
        QString bpValue = properties.value(QString(PLG_ENTITY_BINDPARAM_VALUE)+":"+bpUid);

        QMap<QString, QString> bpAttr;
        bpAttr.insert("name", bpName);
        bpAttr.insert("value", bpValue);

        Entity* bp = getProject()->getEntityById(entities.key(bpUid));

        // Change a linkParam entity in core that
        // has been added by the view.
        if (bp != NULL){
          if (coreBindParamUid.contains(bp->getUniqueId()))
            coreBindParamUid.remove(coreBindParamUid.indexOf(bp->getUniqueId()));

          _waiting = true;
          emit setAttributes(bp, bpAttr, false);

        // Change a linkParam entity in core that has not been added
        // by the view and contains a empty 'value' attribute.
        }else if (coreBindParamName.contains(bpName)){

          int index = coreBindParamName.indexOf(bpName);

          bp = getProject()->getEntityById(coreBindParamUid.at(index));

          coreBindParamUid.remove(index);

          entities.insert(bp->getUniqueId(), bpUid);

          _waiting = true;
          emit setAttributes(bp, bpAttr, false);

        }else {
          _waiting = true;
          _notified = bpUid;
          emit addEntity("bindParam",entity->getUniqueId(), bpAttr, false);
        }
      }
    }

    foreach (QString lpCoreUid, coreBindParamUid) {
      emit removeEntity(getProject()->getEntityById(lpCoreUid), false);
    }
  }
  _waiting = true;
  emit setAttributes(entity, attributes, false);
}

void StructuralViewPlugin::requestEntitySelection(const QString uid)
{
  if (entities.key(uid, "nil") != "nil")
  {
    lastSelected = entities.key(uid);

    emit sendBroadcastMessage("changeSelectedEntity",
                              new QString(entities.key(uid)));
  }
}

void StructuralViewPlugin::requestBodyDependence()
{
  QList<Entity*> list = getProject()->getEntitiesbyType("ncl");

  if (list.isEmpty())
  {
    Entity* project = getProject();
    if (project != NULL)
    {
      QMap<QString, QString> attributes;
      emit addEntity("ncl", project->getUniqueId(), attributes, false);
    }
  }
}

void StructuralViewPlugin::textualStartSync(QString, void*)
{
  _synching = true;
}

void StructuralViewPlugin::textualFinishSync(QString, void*)
{
  _synching = false;
  updateFromModel();
}

void StructuralViewPlugin::clearValidationMessages(QString, void *param)
{
  _window->getView()->clearErrors();
}

void StructuralViewPlugin::validationError(QString pluginID, void *param)
{
  if(_synching)
    return;

  if(param)
  {
    pair <QString, QString> *p = (pair <QString, QString> *)param;

    if (entities.contains(p->first))
      _window->getView()->markError(entities.value(p->first), p->second);
  }
}

QString StructuralViewPlugin::getUidById(QString id)
{
  return getUidById(id, getProject());
}

QString StructuralViewPlugin::getUidById(QString id, Entity* entity)
{
  QString uid = "";

  if (entity->getAttribute("id") == id)
    return entity->getUniqueId();

  foreach(Entity* child, entity->getChildren())
  {
    QString result = getUidById(id, child);

    if (result != "")
    {
      uid = result;
      break;
    }
  }

  return uid;
}

QString StructuralViewPlugin::getUidByName(QString name, Entity* entity)
{
  QString uid = "";

  if (entity->getAttribute("name") == name)
    return entity->getUniqueId();

  foreach(Entity* child, entity->getChildren())
  {
    QString result = getUidByName(name, child);

    if (result != "")
    {
      uid = result;
      break;
    }
  }

  return uid;
}

void StructuralViewPlugin::updateConnectorsDataInView()
{
  // TODO: this function should be calling only when a change occurs in
  // <connectorBase>. Currently, this function is calling every time the
  // link's dialog is displayed.
  if (!getProject()->getEntitiesbyType("connectorBase").isEmpty()){
    Entity* connectorBase = getProject()->getEntitiesbyType("connectorBase").first();

    QMap<QString, QVector<QString> > conditions;
    QMap<QString, QVector<QString> > actions;
    QMap<QString, QVector<QString> > params;

    foreach (Entity* e, connectorBase->getChildren()){

      // loading data from local causalConnector
      if (e->getType() == "causalConnector") {
        QString connId;
        QVector<QString> connConditions;
        QVector<QString> connActions;
        QVector<QString> connParams;

        connId = e->getAttribute("id");

        foreach (Entity* ec, e->getChildren()) {
          if (ec->getType() == "simpleCondition") {
            connConditions.append(ec->getAttribute("role"));
          }

          if (ec->getType() == "simpleAction") {
            connActions.append(ec->getAttribute("role"));
          }

          if (ec->getType() == "connectorParam") {
            connParams.append(ec->getAttribute("name"));
          }

          if (ec->getType() == "compoundCondition") {
            foreach (Entity* ecc, ec->getChildren()) {
              if (ecc->getType() == "simpleCondition") {
                connConditions.append(ecc->getAttribute("role"));
              }
            }
          }

          if (ec->getType() == "compoundAction") {
            foreach (Entity* ecc, ec->getChildren()) {
              if (ecc->getType() == "simpleAction") {
                connActions .append(ecc->getAttribute("role"));
              }
            }
          }
        }

        conditions.insert(connId, connConditions);
        actions.insert(connId, connActions);
        params.insert(connId, connParams);

      // loading data from importBases
      }else if (e->getType() == "importBase") {
        QString importAlias;
        QString importURI;

        QString projectURI;
        QString projectSeparator;

        importAlias = e->getAttribute("alias");
        importURI = e->getAttribute("documentURI");

        projectURI = getProject()->getLocation();
        projectSeparator = QDir::separator();

        QFile importFile(projectURI.left(projectURI.lastIndexOf(projectSeparator))+projectSeparator+importURI);

        if (importFile.open(QIODevice::ReadOnly)){
          QDomDocument d;
          if (d.setContent(&importFile)){

            QDomElement r = d.firstChildElement(); // <ncl>
            QDomElement h = r.firstChildElement(); // <head>
            QDomElement b;                         // <connectorBase>

            bool hasConnBase = false;

            QDomNodeList hc = h.childNodes();
            for (unsigned int i = 0; i < hc.length(); i++){
              if (hc.item(i).isElement()){

                if (hc.item(i).nodeName() == "connectorBase"){
                  b = hc.item(i).toElement();
                  hasConnBase = true;
                  break;
                }
              }
            }

            if (hasConnBase){
              QDomNodeList bc = b.childNodes();
              for (unsigned int i = 0; i < bc.length(); i++){
                if (bc.item(i).isElement()){
                  if (bc.item(i).nodeName() == "causalConnector"){
                    QString connId;
                    QVector<QString> connConditions;
                    QVector<QString> connActions;
                    QVector<QString> connParams;

                    if (bc.item(i).attributes().namedItem("id").isNull())
                      break;

                    connId = bc.item(i).attributes().namedItem("id").nodeValue();
                    connId = importAlias+"#"+connId;

                    QDomNodeList bcc = bc.item(i).childNodes();
                    for (unsigned int j = 0; j < bcc.length(); j++){
                      if (bcc.item(j).nodeName() == "simpleCondition"){
                        if (bcc.item(j).attributes().namedItem("role").isNull())
                          break;

                        connConditions.append(bcc.item(j).attributes().namedItem("role").nodeValue());
                      }

                      if (bcc.item(j).nodeName() == "simpleAction"){
                        if (bcc.item(j).attributes().namedItem("role").isNull())
                          break;

                        connActions.append(bcc.item(j).attributes().namedItem("role").nodeValue());
                      }

                      if (bcc.item(j).nodeName() == "connectorParam"){
                        if (bcc.item(j).attributes().namedItem("name").isNull())
                          break;

                        connParams.append(bcc.item(j).attributes().namedItem("name").nodeValue());
                      }

                      if (bcc.item(j).nodeName() == "compoundCondition"){
                        QDomNodeList bccc = bcc.item(j).childNodes();
                        for (unsigned int k = 0; k < bccc.length(); k++){
                          if (bccc.item(k).nodeName() == "simpleCondition"){
                            if (bccc.item(k).attributes().namedItem("role").isNull())
                              break;

                            connConditions.append(bccc.item(k).attributes().namedItem("role").nodeValue());
                          }
                        }
                      }

                      if (bcc.item(j).nodeName() == "compoundAction"){
                        QDomNodeList bccc = bcc.item(j).childNodes();
                        for (unsigned int k = 0; k < bccc.length(); k++){
                          if (bccc.item(k).nodeName() == "simpleAction"){
                            if (bccc.item(k).attributes().namedItem("role").isNull())
                              break;

                            connActions.append(bccc.item(k).attributes().namedItem("role").nodeValue());
                          }
                        }
                      }
                    }

                    conditions.insert(connId, connConditions);
                    actions.insert(connId, connActions);
                    params.insert(connId, connParams);
                  }
                }
              }
            }
          }
        }
      }
    }

    _window->getView()->updateLinkDialog(conditions, actions, params);
  }
}
