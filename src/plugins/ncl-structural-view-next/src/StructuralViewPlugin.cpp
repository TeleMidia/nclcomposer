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
  QMap <QString, double> top, left, width, height;
  foreach (QString key, entities.keys())
  {
    StructuralEntity* e = _window->getView()->getEntity(entities.value(key));

    if (e != NULL)
    {
      QString pid = "";

      if (e->getStructuralParent() != NULL)
         pid = e->getStructuralParent()->getStructuralId();

      top.insert(e->getStructuralId()+pid,e->getTop());
      left.insert(e->getStructuralId()+pid,e->getLeft());
      width.insert(e->getStructuralId()+pid,e->getWidth());
      height.insert(e->getStructuralId()+pid,e->getHeight());
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
      requestEntityAddition(children[i]);
      stack.push(children[i]);
    }
  }

  foreach (QString key, entities.keys())
  {
    StructuralEntity* e = _window->getView()->getEntity(entities.value(key));

    if (e != NULL)
    {
      QString pid = "";

      if (e->getStructuralParent() != NULL)
         pid = e->getStructuralParent()->getStructuralId();

      if (top.contains(e->getStructuralId()+pid)){
        QMap<QString, QString> properties;
        properties.insert(PLG_ENTITY_TOP,QString::number(top.value(e->getStructuralId()+pid)));
        properties.insert(PLG_ENTITY_LEFT,QString::number(left.value(e->getStructuralId()+pid)));
        properties.insert(PLG_ENTITY_WIDTH,QString::number(width.value(e->getStructuralId()+pid)));
        properties.insert(PLG_ENTITY_HEIGHT,QString::number(height.value(e->getStructuralId()+pid)));

        QMap<QString, QString> settings = StructuralUtil::createSettings(true, false);

        _window->getView()->change(e->getStructuralUid(),properties,e->getStructuralProperties(),settings);
      }
    }
  }
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

void StructuralViewPlugin::requestEntityAddition(Entity* entity)
{
  QMap<QString, QString> properties;


  Structural::StructuralType type = StructuralUtil::getnstTypeFromStr(entity->getType());
  properties[PLG_ENTITY_TYPE] = QString::number(type);

  QMap<QString, QString> m = StructuralUtil::createCoreTranslationMap(type);

  entities[entity->getUniqueId()] = entity->getUniqueId();


  if (!m.isEmpty()){
  foreach (QString key, m.keys()) {
    if (!entity->getAttribute(key).isEmpty())
      properties.insert(m.value(key),entity->getAttribute(key));
  }


    QMap<QString,QString> settings = StructuralUtil::createSettings(true, false);

    QString parentUID = entity->getParentUniqueId();

    if(type == Structural::Body)
    {
      parentUID = "";
    }

    else
    {
      parentUID = entity->getParentUniqueId();
    }


    _window->getView()->insert(entity->getUniqueId(), parentUID, properties, settings);
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
  properties[PLG_ENTITY_TYPE] = QString::number(type);

  QMap<QString, QString> m = StructuralUtil::createCoreTranslationMap(type);

  foreach (QString key, m.keys()) {
    if (!entity->getAttribute(key).isEmpty())
      properties.insert(m.value(key),entity->getAttribute(key));
  }


    if(entities.contains(entity->getUniqueId()))
    {

    QMap<QString,QString> settings = StructuralUtil::createSettings(true,false);
    settings.insert("LOCATION","REQUEST CHANGE PLUGIN");

     QString entityPlgUID = entities[entity->getUniqueId()];

     if (_window->getView()->hasEntity(entityPlgUID)){
      _window->getView()->change(entityPlgUID, properties, _window->getView()->getEntity(entityPlgUID)->getStructuralProperties(), settings);
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

  Entity* entity = NULL;

  if (!parent.isEmpty())
    entity = getProject()->getEntityById(entities.key(parent));

  else if (type == Structural::Body)
  {
    requestBodyDependence();

    QList<Entity*> list = getProject()->getEntitiesbyType("ncl");
    if (!list.isEmpty())
      entity = list.first();
    else
      return;

  }else
    return;

  QMap<QString, QString> m = StructuralUtil::createViewTranslationMap(type);
  QMap<QString, QString> attributes;

  if(entity != NULL) // Everything is ok, so add my new entity
  {
    foreach (QString key, m.keys()) {
      if (properties.contains(key))
        attributes.insert(m.value(key),properties.value(key));
    }

    _waiting = true;
    _notified = uid;
    emit addEntity(StructuralUtil::getStrFromNstType(type),
                   entity->getUniqueId(), attributes, false);


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
    if (properties.contains(key))
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
