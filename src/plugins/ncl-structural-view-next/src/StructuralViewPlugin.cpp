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

      prop.insert(e->getStructuralId()+pid, e->getStructuralProperties());
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

        _window->getView()->change(e->getStructuralUid(),cacheprop,e->getStructuralProperties(),settings);

      }else{

        // the code below ensure that new ports draws its references
        // correctly
        if (e->getStructuralType() == Structural::Port){
          QMap<QString, QString> cprop = e->getStructuralProperties();

          bool haschange = false;

          if (cprop.contains(PLG_ENTITY_COMPONENT_ID)){
            QString coreUID = getUidById(cprop.value(PLG_ENTITY_COMPONENT_ID));

            if (entities.contains(coreUID)) {

              if (e->getStructuralProperty(PLG_ENTITY_COMPONENT_UID) != entities.value(coreUID)){
                cprop.insert(PLG_ENTITY_COMPONENT_UID, entities.value(coreUID));
                haschange = true;
              }
            }
          }

          if (cprop.contains(PLG_ENTITY_INTERFACE_ID)){
            QString coreUID = getUidById(cprop.value(PLG_ENTITY_INTERFACE_ID));

            if (entities.contains(coreUID)){
              if (e->getStructuralProperty(PLG_ENTITY_INTERFACE_UID) != entities.value(coreUID)){
                cprop.insert(PLG_ENTITY_INTERFACE_UID, entities.value(coreUID));
                haschange = true;
              }
            }
          }

          if (haschange)
            _window->getView()->change(e->getStructuralUid(),cprop,e->getStructuralProperties(),settings);
        }
      }

      foreach (StructuralEntity* c, e->getStructuralEntities()) {
          s.push(c);
      }
    }
  }

  _window->getView()->undoStateChange(false);
  _window->getView()->redoStateChange(false);
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

      if (type == Structural::Link)
      {
        // TODO: Load connector properties in order to create
        // binds correctly
      }

      _window->getView()->insert(entity->getUniqueId(), parentUID, properties, settings);
    }
  }
}

void StructuralViewPlugin::requestEntityRemotion(Entity* entity, bool enableUndo)
{

    QString entityID = entity->getUniqueId();
    if(entities.contains(entityID))
    {
      QMap<QString,QString> settings = StructuralUtil::createSettings(enableUndo,false);

      _window->getView()->remove(entities[entityID], settings);
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

      QMap<QString,QString> settings = StructuralUtil::createSettings(true,false);

      QString entityPlgUID = entities[entity->getUniqueId()];

     if (_window->getView()->hasEntity(entityPlgUID)){
      _window->getView()->change(entityPlgUID, properties, _window->getView()->getEntity(entityPlgUID)->getStructuralProperties(), settings);
     }
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

