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

//
// ATTENTION: This code needs a refactoring.
//

StructuralViewPlugin::StructuralViewPlugin(QObject* parent)
{
  setParent(parent);

  createWidgets();
  createConnections();

  n = 0;

  request = "";

//  isSyncingFromTextual = false;
}

StructuralViewPlugin::~StructuralViewPlugin()
{
  //delete(view);
}

void StructuralViewPlugin::createWidgets()
{
  //view = new View();
  //view->setMinimapVisible(true);

  window = new StructuralWindow();
  view = window->getView();

  isConnector =false;
}

void StructuralViewPlugin::createConnections()
{
  connect(view, SIGNAL(inserted(QString,QString,QMap<QString,QString>,QMap<QString,QString>)),
          SLOT(notifyEntityAddedInView(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));

  connect(view, SIGNAL(removed(QString,QMap<QString,QString>)),
                SLOT(notifyEntityDeletedInView(QString,QMap<QString,QString>)));

  connect(view, SIGNAL(changed(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)),
                SLOT(notifyEntityChangedInView(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));

  connect(view, SIGNAL(selected(QString,QMap<QString,QString>)),
                SLOT(notifyEntitySelectionInView(QString,QMap<QString,QString>)));

  //connect(view, SIGNAL(viewChanged()), SIGNAL(setCurrentProjectAsDirty()));
}

void StructuralViewPlugin::init()
{

  QString data = project->getPluginData("br.puc-rio.telemidia.nclstructural");

  QString startEntitiesSep = "~~VIEW-DATA~~";
  QString endEntitiesSep = "~~PLUGIN-DATA~~";

  int indexOfStartEntities = data.indexOf(startEntitiesSep);
  int indexOfEndEntities = data.indexOf(endEntitiesSep);

  int indexOfStartEntitiesContent = indexOfStartEntities + startEntitiesSep.length();


  QString mid = data.mid(indexOfStartEntitiesContent,
                         indexOfEndEntities - indexOfStartEntitiesContent);

  view->load(mid);

  startEntitiesSep = "~~PLUGIN-DATA~~";
  endEntitiesSep = "$END_ENTITIES_LINES$";

  indexOfStartEntities = data.indexOf(startEntitiesSep);
  indexOfEndEntities = data.indexOf(endEntitiesSep);

  indexOfStartEntitiesContent = indexOfStartEntities + startEntitiesSep.length();


  mid = data.mid(indexOfStartEntitiesContent,
                         indexOfEndEntities - indexOfStartEntitiesContent);

  int lastp = 0;
  int nextp = mid.indexOf("~");

  while(nextp > 0)
  {
    QString entitypair = mid.mid(lastp, nextp - lastp);

    QStringList list = entitypair.split("=");

    QString coreID = list[0], structuralID = list[1];

    entities[coreID] = structuralID;

    /*
    //Update user data
    if(view->hasEntity(structuralID))
    {
      QnstEntity* nstEntity =
          view->getEntity(structuralID);

      if(nstEntity != NULL)
      {
        QMap <QString, QString>::iterator begin, end, it;
        QMap <QString, QString> userData;

        Entity *entity = project->getEntityById(coreID);
        // if(entity != NULL)
        //{
          // entity->getAttributeIterator(begin, end);

          // for (it = begin; it != end; ++it)
            // userData[it.key()] = it.value();

          // nstEntity->setUsrData(userData);
        // }
      }
    }
    */

    lastp = nextp+1;
    nextp = mid.indexOf("~", nextp+1);
  }
}

QWidget* StructuralViewPlugin::getWidget()
{
//  return NULL;
  //return view;
  return window;
}

bool StructuralViewPlugin::saveSubsession()
{
  QByteArray data;

  data.append("$START_ENTITIES_LINES$");

  data.append("~~VIEW-DATA~~");

  QString info; window->getView()->serialize(info);

  data.append(info);

  data.append("~~PLUGIN-DATA~~");
  foreach(QString key, entities.keys()){
      data.append(key+"="+entities[key]+"~");
  }

  data.append("$END_ENTITIES_LINES$");

  emit setPluginData(data);

  return true;
}

void StructuralViewPlugin::updateFromModel()
{
  QMap <QString, int> top, left, width, height;

  //save previous positions
  foreach (QString key, entities.keys())
  {
    //QnstGraphicsEntity *entity =
    //    dynamic_cast <QnstGraphicsEntity*>(view->getEntity(entities.value(key)));

    //if(entity &&
    //   entity->getnstId() != "")
    //{
    //  top[entity->getnstId()] = entity->getTop();
    //  left[entity->getnstId()]=  entity->getLeft();
    //  height[entity->getnstId()] = entity->getHeight();
    //  width[entity->getnstId()] = entity->getWidth();
    //}
  }

  foreach(QString coreID, entities.keys())
  {
    Entity *entity = project->getEntityById(coreID);
    if(entity != NULL)
    {
      requestEntityRemotion(entity);
    }
  }
  entities.clear();

  //view->clearAllData();

  QStack <Entity *> stack;
  stack.push(project);

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

  // \fixme This second step is to ensure that all binds will be correctly drawn
  // it should not be necessary if we fix the order we do the insertions!!
  stack.push(project);
  while(stack.size())
  {
    Entity *current = stack.top();
    stack.pop();

    QVector <Entity *> children = current->getChildren();
    for(int i = 0; i <children.size(); i++)
    {
      requestEntityChange(children[i]);
      stack.push(children[i]);
    }
  }

  // Set saved positions!!
  foreach (QString key, entities.keys())
  {
    //QnstGraphicsEntity *entity =
    //    dynamic_cast <QnstGraphicsEntity*>(view->getEntity(entities.value(key)));
    //if(entity && entity->getnstId() != "" && top.contains(entity->getnstId()))
    //{
    //  entity->setTop(top[entity->getnstId()]);
    //  entity->setLeft(left[entity->getnstId()]);
    //  entity->setWidth(width[entity->getnstId()]);
    //  entity->setHeight(height[entity->getnstId()]);
    //}
  }

  //view->adjustAll();
}

void StructuralViewPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
//  if(isSyncingFromTextual) return;

  if (pluginID != getPluginInstanceID()){
    requestEntityAddition(entity);
  }
  else
  {
    if (request != "")
    {
      if (isConnector)
        xconnector_viewLink[request] = entity->getUniqueId();
      else
        entities[entity->getUniqueId()] = request;

      isConnector = false;
    }
  }

  //Add user data
  /*if(entities.contains(entity->getUniqueId()))
  {
    QString structuralId = entities.value(entity->getUniqueId());

    if(view->hasEntity(structuralId))
    {
      QnstEntity *nsEntity = view->getEntity(structuralId);

      QMap <QString, QString>::iterator begin, end, it;
      QMap <QString, QString> userData = nsEntity->getUsrData();
      entity->getAttributeIterator(begin, end);
      for (it = begin; it != end; ++it)
        userData.insert(it.key(), it.value());

      nsEntity->setUsrData(userData);
    }
    else
      qWarning() << "[QNST] QnsComposerPlugin::onEntityAdded entity "
                 << request << "is not in the view.";
  }
  else
    qWarning() << "[QNST] QnsComposerPlugin::onEntityAdded Entity was not "
               << "added"; */

  // \todo This must be incremental
  //view->clearValidationErrors();
}

void StructuralViewPlugin::errorMessage(QString error)
{

}

void StructuralViewPlugin::onEntityChanged(QString pluginID, Entity *entity)
{
//  if(isSyncingFromTextual)
//  {
//    dirtyEntities.push_back(entity->getUniqueId());
//    return;
//  }

  if (pluginID != getPluginInstanceID() && !pluginID.isEmpty())
    requestEntityChange(entity);

  //Update user data
  if(entities.contains(entity->getUniqueId()))
  {
    QString structuralId = entities.value(entity->getUniqueId());

    //if(view->hasEntity(structuralId))
    //{
    //  QnstEntity *nstEntity = view->getEntity(structuralId);

    //  if(nstEntity != NULL)
    //  {
        /*QMap <QString, QString>::iterator begin, end, it;
        QMap <QString, QString> userData;
        entity->getAttributeIterator(begin, end);
        for (it = begin; it != end; ++it)
          userData[it.key()] = it.value();

        nstEntity->setUsrData(userData);

        qDebug() << "[QNST] QnstComposerPlugin::onEntityChanged" << userData;*/
    //  }
   // }
  }

  // \todo This must be incremental
  //view->clearValidationErrors();
}

void StructuralViewPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
//  if(isSyncingFromTextual) return;

  if (pluginID != getPluginInstanceID())
  {
      requestEntityRemotion(getProject()->getEntityById(entityID));
      entities.remove(entityID);
  }
}

void StructuralViewPlugin::changeSelectedEntity(QString pluginID, void* param)
{
//  if(isSyncingFromTextual) return;
  QString* entityUID = (QString*) param;

  if(entityUID != NULL)
  {
    Entity *entity = getProject()->getEntityById(*entityUID);
    if(entity != NULL)
    {
      requestEntitySelection(entity);
    }
    else
      qWarning() << "QnstComposerPlugin is trying to select a entity that are not in the core"
                 << *entityUID;
  }
}

void StructuralViewPlugin::requestEntityAddition(Entity* entity)
{
  QMap<QString, QString> properties;

  Structural::EntityName type = StructuralUtil::getnstTypeFromStr(entity->getType());
  properties["LOCAL:NAME"] = QString::number(type);

//  switch (type) {
//    case Structural::Body:
//      entities[entity->getUniqueId()] = entity->getUniqueId();
//      break;

//    default:
//      break;
//  }

//  if (!entity->getAttribute("id").isEmpty())
//  {
//    properties["LOCAL:ID"] = entity->getAttribute("id");
//  }

//  QMap<QString, QString> settings;
//  settings["UNDO"] = "1";
//  settings["NOTIFY"] = "0";
//  settings["CODE"] = QUuid::createUuid().toString();

//  if (type == Structural::Body)
//  {
//    view->insert(entity->getUniqueId(), "", properties, settings);
//  }

  bool ok = false;


  switch (type)
  {
    case Structural::Body:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      if (entity->getAttribute("id") != "")
        properties["LOCAL:ID"] = entity->getAttribute("id");

      ok = true;
      break;
    }

    case Structural::Context:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      if (entity->getAttribute("id") != "")
        properties["LOCAL:ID"] = entity->getAttribute("id");

      if (entity->getAttribute("refer") != "")
      {
        properties["LOCAL:REFER:ID"] = entity->getAttribute("refer");

        QString referUID = getUidById(properties["refer"]);
        if(entities.contains(referUID))
          properties["LOCAL:REFER:UID"] = entities[referUID];
      }

      ok = true;

      break;
    }

    case Structural::Switch:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      if (entity->getAttribute("id") != "")
        properties["LOCAL:ID"] = entity->getAttribute("id");

      if (entity->getAttribute("refer") != "")
      {
        properties["LOCAL:REFER:ID"] = entity->getAttribute("refer");

        QString referUID = getUidById(properties["refer"]);
        if(entities.contains(referUID))
          properties["LOCAL:REFER:UID"] = entities[referUID];
      }

      ok = true;

      break;
    }

    case Structural::Media:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["LOCAL:ID"] = entity->getAttribute("id");
      properties["LOCAL:SRC"] = entity->getAttribute("src");
      properties["LOCAL:REFER:ID"] = entity->getAttribute("refer");
      properties["LOCAL:INSTANCE"] = entity->getAttribute("instance");
      properties["CPR:TYPE"] = entity->getAttribute("type");
      properties["LOCAL:DESCRIPTOR"] = entity->getAttribute("descriptor");

      if (entity->getAttribute("refer") != "")
      {
        QString referUID = getUidById(properties["refer"]);
        if(entities.contains(referUID))
          properties["LOCAL:REFER:UID"] = entities[referUID];
      }
      else
      {
        properties["LOCAL:REFER:UID"] = "";
      }

      // \fixme This must be made after the switch/case!!
//      if(entities.contains(entity->getParentUniqueId()))
//      {
//        //view->ref_addEntity(entity->getUniqueId(),
//        //                entities[entity->getParentUniqueId()],
//        //                properties);

//        QList<Entity*> list;

//        list = getProject()->getEntitiesbyType("port");

//        // update properties?
//        foreach(Entity* e, list)
//        {
//          if (e->getAttribute("component") != "" &&
//              e->getAttribute("component") == properties["id"])
//          {
//            requestEntityChange(e);
//          }
//        }
//      }

      ok =true;
      break;
    }

    case Structural::Port:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["LOCAL:ID"] = entity->getAttribute("id");

      properties["LOCAL:COMPONENT:ID"] = entity->getAttribute("component");
      if (entity->getAttribute("component") != "")
      {
        QString componentUID = getUidById(properties["component"]);
        if(entities.contains(componentUID))
          properties["LOCAL:COMPONENT:UID"] = entities[componentUID];
      }

      properties["LOCAL:INTERFACE:ID"] = entity->getAttribute("interface");
      if (entity->getAttribute("interface") != "")
      {
          QString interfaceUID = getUidById(properties["interface"]);
          if(entities.contains(interfaceUID))
            properties["LOCAL:INTERFACE:UID"] = entities[interfaceUID];
      }

      ok = true;

      break;
    }

//    case Structural::Link:
//    {
//      entities[entity->getUniqueId()] = entity->getUniqueId();

//      if (entity->getAttribute("id") != "")
//        properties["LOCAL:ID"] = entity->getAttribute("id");

//      if (entity->getAttribute("xconnector") != "")
//      {
//        properties["LOCAL:XCONNECTOR:ID"] = entity->getAttribute("xconnector");

//        QString xconnectorUID = getUidById(properties["xconnector"]);
//        if(entities.contains(xconnectorUID))
//          properties["LOCAL:XCONNECTOR:UID"] = entities[xconnectorUID];
//        else
//          properties["LOCAL:XCONNECTOR:UID"] = "";
//      }

//      ok = true;

//      break;
//    }

//    case Structural::Bind:
//    {
//      entities[entity->getUniqueId()] = entity->getUniqueId();

//      properties["role"] = entity->getAttribute("role");
//      properties["component"] = entity->getAttribute("component");

//      QString comUID = "";
//      if (entity->getAttribute("component") != "")
//      {
//        comUID = getUidById(properties["component"]);
//        if(entities.contains(comUID))
//          properties["componentUid"] = entities[comUID];
//        else
//          properties["componentUid"] = "";
//      }
//      else
//      {
//        properties["componentUid"] = "";
//      }

//      properties["interface"] = entity->getAttribute("interface");

//      if (entity->getAttribute("interface") != "")
//      {
//        Entity* cmp = getProject()->getEntityById(comUID);
//        if(cmp != NULL)
//        {
//          QString intUID = "";
//          foreach(Entity* c, cmp->getChildren())
//          {
//            if (c->getAttribute("id") == entity->getAttribute("interface") ||
//                c->getAttribute("name") == entity->getAttribute("interface"))
//            {
//              intUID = c->getUniqueId();
//              break;
//            }
//          }

//        if(entities.contains(intUID))
//         properties["interfaceUid"] = entities[intUID];
//        else
//          properties["interfaceUid"] = "";
//        }
//      }
//      else
//      {
//        properties["interfaceUid"] = "";
//      }

//      properties["linkUID"] = entities[entity->getParentUniqueId()];

//      ok = true;
//      break;
//    }

    case Structural::Property:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["LOCAL:ID"] = entity->getAttribute("name");

      ok = true;
      break;
    }

    case Structural::Area:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["LOCAL:ID"] = entity->getAttribute("id");

      ok = true;
      break;
    }

    case Structural::SwitchPort:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["LOCAL:ID"] = entity->getAttribute("id");

      ok = true;
      break;
    }

//    case Structural::Mapping:
//    {
//      entities[entity->getUniqueId()] = entity->getUniqueId();

//      properties["component"] = entity->getAttribute("component");
//      if (entity->getAttribute("component") != "")
//      {
//        QString componentUID = getUidById(properties["component"]);
//        if(entities.contains(componentUID))
//          properties["componentUid"] = entities[componentUID];
//      }

//      properties["interface"] = entity->getAttribute("interface");
//      if (entity->getAttribute("interface") != "")
//      {
//        QString interfaceUID = getUidById(properties["interface"]);
//        if(entities.contains(interfaceUID))
//          properties["interfaceUid"] = entities[interfaceUID];
//      }

//      ok = true;
//      break;
//    }

//    case Structural::BindParam:
//    {
//      // \fixme Why we do not add to entities map ??
//      properties["name"] = entity->getAttribute("name");
//      properties["value"] = entity->getAttribute("value");

//      ok = true;
//      break;
//    }

    default:
      // do nothing!!
      break;

  }

  if(ok)
  {
    QMap<QString,QString> settings;
    settings["UNDO"] = "1";
    settings["NOTIFY"] = "0";
    settings["CODE"] = StructuralUtil::createUid();

    QString parentUID = entity->getParentUniqueId();

    // if it is body we are with no parent
    if(type == Structural::Body)
    {
      parentUID = "";
    }
    // otherwise, we need to set add with a parent
    else
    {
      parentUID = entity->getParentUniqueId();
    }

    view->insert(entity->getUniqueId(), parentUID, properties, settings);
  }
//  else
//  {
//    // Special cases!! We do not have Qnst::EntityType defined!!
//    if (entity->getType() == "causalConnector")
//    {
//      entities[entity->getUniqueId()] = entity->getUniqueId();

//      requestCausalConnectorAddition(entity);
//    }
//    else if (entity->getType() == "simpleCondition")
//    {
//      entities[entity->getUniqueId()] = entity->getUniqueId();

//      requestSimpleConditionAddition(entity);
//    }
//    else if (entity->getType() == "simpleAction")
//    {
//      entities[entity->getUniqueId()] = entity->getUniqueId();

//      requestSimpleActionAddition(entity);
//    }
//    else if (entity->getType() == "importBase")
//    {
//      entities[entity->getUniqueId()] = entity->getUniqueId();
//      requestImportBaseAddition(entity);
//    }
//    else if (entity->getType() == "connectorParam")
//    {
//      entities[entity->getUniqueId()] = entity->getUniqueId();
//      requestConnectorParamAddition(entity);
//    }
//  }
}

void StructuralViewPlugin::requestEntityRemotion(Entity* entity)
{
  if (isEntityHandled(entity))
  {
    QString entityID = entity->getUniqueId();
    if(entities.contains(entityID))
    {
      QMap<QString,QString> settings;
      settings["UNDO"] = "1";
      settings["NOTIFY"] = "0";
      settings["CODE"] = StructuralUtil::createUid();

      view->remove(entities[entityID], settings);
      entities.remove(entityID);
    }
    else
      qWarning() << "You are trying to remove an entity that are not in the QnstComposerPlugin.";
  }
}

void StructuralViewPlugin::requestEntityChange(Entity* entity)
{
  QMap<QString, QString> properties;


  // get the qnst type based on NCL Composer Type
  int type = StructuralUtil::getnstTypeFromStr(entity->getType());

  bool ok = false;

  switch (type)
  {
    case Structural::Body:
    {
      if (entity->getAttribute("id") != "")
        properties["LOCAL:ID"] = entity->getAttribute("id");

      ok = true;
      break;
    }
    case Structural::Context:
    case Structural::Switch:
    {
      if (entity->getAttribute("id") != "")
        properties["LOCAL:ID"] = entity->getAttribute("id");

      if (entity->getAttribute("refer") != "")
      {
        properties["LOCAL:REFER:ID"] = entity->getAttribute("refer");

        QString referUID = getUidById(properties["refer"]);
        if(entities.contains(referUID))
          properties["LOCAL:REFER:UID"] = entities[referUID];
      }

      ok = true;
      break;
    }

    case Structural::Media:
    {
      properties["LOCAL:ID"] = entity->getAttribute("id");
      properties["LOCAL:SRC"] = entity->getAttribute("src");
      properties["LOCAL:REFER:ID"] = entity->getAttribute("refer");
      properties["LOCAL:INSTANCE"] = entity->getAttribute("instance");
      properties["CPR:TYPE"] = entity->getAttribute("type");
      properties["LOCAL:DESCRIPTOR"] = entity->getAttribute("descriptor");

      if (entity->getAttribute("refer") != "")
      {
        QString referUID = getUidById(properties["refer"]);
        if(entities.contains(referUID))
          properties["LOCAL:REFER:UID"] = entities[referUID];
      }
      else
      {
        properties["LOCAL:REFER:UID"] = "";
      }

      //if(entities.contains(entity->getUniqueId()))
        //view->ref_changeEntity( entities[entity->getUniqueId()],
        //                    properties);

//      QList<Entity*> list;
//      list = getProject()->getEntitiesbyType("port");

//      foreach(Entity* e, list){
//        if (e->getAttribute("component") != "")
//        {
//          if (e->getAttribute("component") == properties["id"])
//          {
//            requestEntityChange(e);
//          }
//        }
//      }

      ok = true;
      break;
    }

    case Structural::Port:
    {
      properties["LOCAL:ID"] = entity->getAttribute("id");

      properties["LOCAL:COMPONENT:ID"] = entity->getAttribute("component");
      if (entity->getAttribute("component") != "")
      {
        QString componentUID = getUidById(properties["component"]);
        if(entities.contains(componentUID))
          properties["LOCAL:COMPONENT:UID"] = entities[componentUID];
      }

      properties["LOCAL:INTERFACE:ID"] = entity->getAttribute("interface");
      if (entity->getAttribute("interface") != "")
      {
          QString interfaceUID = getUidById(properties["interface"]);
          if(entities.contains(interfaceUID))
            properties["LOCAL:INTERFACE:UID"] = entities[interfaceUID];
      }

      ok = true;
      break;
    }

//    case Structural::Link:
//    {
//      if (entity->getAttribute("id") != "")
//        properties["id"] = entity->getAttribute("id");

//      if (entity->getAttribute("xconnector") != "")
//      {
//        properties["xconnector"] = entity->getAttribute("xconnector");
//        if(entities.contains(getUidById(properties["xconnector"])))
//        {
//          properties["xconnectorUID"] =
//              entities[getUidById(properties["xconnector"])];
//        }
//        else
//          properties["xconnectorUID"] = "";
//      }

//      ok = true;
//      break;
//    }

//    case Structural::Bind:
//    case Structural::Action:
//    case Structural::Condition:
//    {
//      properties["TYPE"] = "bind";
//      properties["role"] = entity->getAttribute("role");
//      properties["component"] = entity->getAttribute("component");

//      QString comUID = "";
//      if (entity->getAttribute("component") != "")
//      {
//        comUID = getUidById(properties["component"]);
//        if(entities.contains(comUID))
//          properties["componentUid"] = entities[comUID];
//        else
//          properties["componentUid"] = "";
//      }
//      else
//      {
//        properties["componentUid"] = "";
//      }

//      properties["interface"] = entity->getAttribute("interface");
//      if (entity->getAttribute("interface") != "")
//      {
//        Entity* cmp = getProject()->getEntityById(comUID);

//        if(cmp != NULL)
//        {
//          QString intUID = "";
//          foreach(Entity* c, cmp->getChildren())
//          {
//            if (c->getAttribute("id") == entity->getAttribute("interface") ||
//                c->getAttribute("name") == entity->getAttribute("interface"))
//            {
//              intUID = c->getUniqueId();
//              break;
//            }
//          }

//          if(entities.contains(intUID))
//            properties["interfaceUid"] = entities[intUID];
//          else
//            properties["interfaceUid"] = "";
//        }
//      }
//      else
//      {
//        properties["interfaceUid"] = "";
//      }

//      // get the parent
//      properties["linkUID"] = entities[entity->getParentUniqueId()];

//      ok = true;
//      break;
//    }

    case Structural::Area:
    {
      properties["LOCAL:ID"] = entity->getAttribute("id");

      ok = true;
      break;
    }

    case Structural::Property:
    {
      properties["LOCAL:ID"] = entity->getAttribute("name");

      ok = true;
      break;
    }

    case Structural::SwitchPort:
    {
      properties["LOCAL:ID"] = entity->getAttribute("id");
      ok = true;
      break;
    }

//    case Structural::Mapping:
//    {
//      properties["component"] = entity->getAttribute("component");
//      if (entity->getAttribute("component") != "")
//      {
//        QString componentUID = getUidById(properties["component"]);
//        if(entities.contains(componentUID))
//          properties["componentUid"] = entities[componentUID];
//      }

//      properties["interface"] = entity->getAttribute("interface");
//      if (entity->getAttribute("interface") != "")
//      {
//        QString interfaceUID = getUidById(properties["interface"]);
//        if(entities.contains(interfaceUID))
//          properties["interfaceUid"] = entities[interfaceUID];
//      }

//      ok = true;
//      break;
//    }

//    case Structural::BindParam:
//    {
//      properties["TYPE"] = "bindParam";

//      properties["name"] = entity->getAttribute("name");
//      properties["value"] = entity->getAttribute("value");
//      properties["parent"] = entities[entity->getParentUniqueId()];

//      ok = true;
//      break;
//    }
  }

  if(ok)
  {
    if(entities.contains(entity->getUniqueId()))
    {

    QMap<QString,QString> settings;
    settings["UNDO"] = "1";
    settings["NOTIFY"] = "0";
    settings["CODE"] = StructuralUtil::createUid();


    view->change(entities[entity->getUniqueId()], properties, view->getEntity(entities[entity->getUniqueId()])->getLocalProperties(), settings);
    }
  }
//  else
//  {
//    // Special cases!! We do not have Qnst::EntityType defined!!
//    if (entity->getType() == "causalConnector")
//    {
//      requestCausalConnectorChange(entity);
//    }
//    // if the entity is of type SimpleCondition
//    else if (entity->getType() == "simpleCondition")
//    {
//      requestSimpleConditionChange(entity);
//    }
//    // if the entity is of type SimpleAction
//    else if (entity->getType() == "simpleAction")
//    {
//      requestSimpleActionChange(entity);
//    }
//    // if the entity is of type ImportBase
//    else if (entity->getType() == "importBase")
//    {
//      requestImportBaseChange(entity);
//    }
//    else if (entity->getType() == "connectorParam")
//    {
//      // \fixme Do what ?
//    }
//  }
}

void StructuralViewPlugin::requestEntitySelection(Entity* entity)
{
  if (entities.contains(entity->getUniqueId()) &&
      lastSelected != entity->getUniqueId())
  {
    if (entity->getType() == "body" ||
        entity->getType() == "context" ||
        entity->getType() == "media" ||
        entity->getType() == "switch" ||
        entity->getType() == "port" ||
        entity->getType() == "link" ||
        entity->getType() == "area" ||
        entity->getType() == "mapping" ||
        entity->getType() == "bind" ||
        entity->getType() == "switchPort" ||
        entity->getType() == "property")
    {
      QMap<QString,QString> settings;
      settings["UNDO"] = "0";
      settings["NOTIFY"] = "0";
      settings["CODE"] = StructuralUtil::createUid();

      view->select(entities[entity->getUniqueId()], settings);
    }

  }
  else
  {
    view->unSelect();
  }
}

void StructuralViewPlugin::requestImportBaseAddition(Entity* entity)
{
  qWarning() << "Adding requestImportBaseAddition " << entity;
  if (entity != NULL)
  {
    Entity* parent = entity->getParent();

    if (parent != NULL)
    {
      if (parent->getType() == "connectorBase")
      {
        QMap<QString, QString> properties;

        properties["TYPE"] = "importBase";

        if (entity->getAttribute("documentURI") != "")
        {
          properties["projectURI"] = getProject()->getLocation();
          properties["documentURI"] = entity->getAttribute("documentURI");
        }

        if (entity->getAttribute("alias") != "")
          properties["alias"] = entity->getAttribute("alias");

        //                if(entities.contains(entity->getUniqueId()))
        //view->ref_addEntity(  entity->getUniqueId(),
        //                  parent->getUniqueId(),
        //                 properties );
      }
    }
  }
}

void StructuralViewPlugin::requestImportBaseChange(Entity* entity)
{
  if (entity != NULL)
  {
    Entity* parent = entity->getParent();

    if (parent != NULL)
    {
      if (parent->getType() == "connectorBase")
      {
        QMap<QString, QString> properties;

        properties["TYPE"] = "importBase";

        if (entity->getAttribute("documentURI") != "")
        {
          properties["projectURI"] = getProject()->getLocation();
          properties["documentURI"] = entity->getAttribute("documentURI");
        }

        if (entity->getAttribute("alias") != "")
          properties["alias"] = entity->getAttribute("alias");

        //                if(entities.contains(entity->getUniqueId()))

        //view->ref_changeEntity(entity->getUniqueId(),properties);
      }
    }
  }
}

void StructuralViewPlugin::requestCausalConnectorAddition(Entity* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "causalConnector";

  if (entity->getAttribute("id") != "")
    properties["id"] = entity->getAttribute("id");

 // if(entity->getParent()->getType() == "importBase")
    //view->ref_addEntity(entity->getUniqueId(), entity->getParentUniqueId(),
    //                properties);
  //else
    //view->ref_addEntity(entity->getUniqueId(),"", properties);
}

void StructuralViewPlugin::requestCausalConnectorChange(Entity* entity)
{
  QMap<QString, QString> properties;

  if (entity->getAttribute("id") != "")
    properties["id"] = entity->getAttribute("id");

  //if(entities.contains(entity->getUniqueId()))
    //view->ref_changeEntity(entities[entity->getUniqueId()], properties);
}

void StructuralViewPlugin::requestSimpleConditionAddition(Entity* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "simpleCondition";
  Entity* conn = entity->getParent();
  while (conn->getType() != "causalConnector" && conn != getProject())
  {
    conn = conn->getParent();
  }

  if(entities.contains(conn->getUniqueId()))
    properties["connector"] = entities[conn->getUniqueId()];
  else
    properties["connector"] = "";

  if (entity->getAttribute("role") != "")
  {
    properties["role"] = entity->getAttribute("role");
  }

  QMap<QString, QString>::iterator begin;
  QMap<QString, QString>::iterator end;

  entity->getAttributeIterator(begin,end);

  int n = 0;

  while(begin != end)
  {
    QString name(begin.key());
    QString value(begin.value());

    if (value.startsWith("$"))
    {
      properties["attr"+QString::number(n)] = name;
      properties["value"+QString::number(n)] = value.replace('$',"");
      n++;
    }

    begin++;
  }

  //if(entities.contains(conn->getUniqueId()))
  //  view->ref_addEntity(entity->getUniqueId(),
  //                  entities[conn->getUniqueId()], properties);
  //else
  //  view->ref_addEntity(entity->getUniqueId(), "", properties);
}

void StructuralViewPlugin::requestSimpleConditionChange(Entity* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "simpleCondition";

  Entity* conn = entity->getParent();
  while (conn->getType() != "causalConnector" && conn != getProject())
  {
    conn = conn->getParent();
  }

  if(entities.contains(conn->getUniqueId()))
    properties["connector"] = entities[conn->getUniqueId()];
  else
    properties["connector"] = "";

  if (entity->getAttribute("role") != "")
    properties["role"] = entity->getAttribute("role");

  QMap<QString, QString>::iterator begin;
  QMap<QString, QString>::iterator end;

  entity->getAttributeIterator(begin,end);

  int n = 0;

  while(begin != end)
  {
    QString name(begin.key());
    QString value(begin.value());

    if (value.startsWith("$"))
    {
      properties["attr"+QString::number(n)] = name;
      properties["value"+QString::number(n)] = value.replace('$',"");
      n++;
    }

    begin++;
  }

  //if(entities.contains(entity->getUniqueId()))
  // view->ref_changeEntity(entities[entity->getUniqueId()], properties);
}

void StructuralViewPlugin::requestSimpleActionAddition(Entity* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "simpleAction";

  Entity* conn = entity->getParent();
  while (conn->getType() != "causalConnector" && conn != getProject())
  {
    conn = conn->getParent();
  }

  if(entities.contains(conn->getUniqueId()))
    properties["connector"] = entities[conn->getUniqueId()];
  else
    properties["connector"] = "";

  if (entity->getAttribute("role") != "")
  {
    properties["role"] = entity->getAttribute("role");
  }

  QMap<QString, QString>::iterator begin;
  QMap<QString, QString>::iterator end;

  entity->getAttributeIterator(begin,end);

  int n = 0;

  while(begin != end)
  {
    QString name(begin.key());
    QString value(begin.value());

    if (value.startsWith("$"))
    {
      properties["attr"+QString::number(n)] = name;
      properties["value"+QString::number(n)] = value.replace('$',"");
      n++;
    }

    begin++;
  }

  //if(entities.contains(conn->getUniqueId()))
  //  view->ref_addEntity(entity->getUniqueId(),
  //                  entities[conn->getUniqueId()], properties);
  //else
  //  view->ref_addEntity(entity->getUniqueId(), "", properties);
}

void StructuralViewPlugin::requestSimpleActionChange(Entity* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "simpleAction";

  Entity* conn = entity->getParent();
  while (conn->getType() != "causalConnector" && conn != getProject())
  {
    conn = conn->getParent();
  }

  if(entities.contains(conn->getUniqueId()))
    properties["connector"] = entities[conn->getUniqueId()];
  else
    properties["connector"] = "";

  if (entity->getAttribute("role") != "")
    properties["role"] = entity->getAttribute("role");

  QMap<QString, QString>::iterator begin;
  QMap<QString, QString>::iterator end;

  entity->getAttributeIterator(begin,end);

  int n = 0;

  while(begin != end)
  {
    QString name(begin.key());
    QString value(begin.value());

    if (value.startsWith("$"))
    {
      properties["attr"+QString::number(n)] = name;
      properties["value"+QString::number(n)] = value.replace('$',"");
      n++;
    }

    begin++;
  }

  //if(entities.contains(entity->getUniqueId()))
  //  view->ref_changeEntity(entities[entity->getUniqueId()], properties);
}

void StructuralViewPlugin::requestConnectorParamAddition(Entity* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "connectorParam";

  properties["name"] = entity->getAttribute("name");

  QString parentUID = entity->getParentUniqueId();
 // if(entities.contains(parentUID))
 //   view->ref_addEntity(entity->getUniqueId(), entities[parentUID], properties);
 // else
 //   qWarning() << "QnstPlugin:: You are trying to add an CONNECTORPARAM as child of an entity that does not exists.";

}

// \fixme this methos is not being called anywhere!!!
void StructuralViewPlugin::requestConnectorParamChange(Entity* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "connectorParam";

  properties["name"] = entity->getAttribute("name");
  properties["parent"] = entities[entity->getParentUniqueId()];

  //if(entities.contains(entity->getUniqueId()))
  //  view->ref_changeEntity(entities[entity->getUniqueId()], properties);
}

void StructuralViewPlugin::notifyEntityAddedInView(const QString uid,
                                                 const QString parent,
                                              QMap<QString, QString> properties,
                                              QMap<QString, QString> settings)
{
  LocalName type = (LocalName) properties["LOCAL:NAME"].toInt();

  Entity* entity = NULL;

  QMap<QString, QString> attributes;
  bool mustEmitAddEntity = false;

  if(type != Structural::Body)
  {
    entity = getProject()->getEntityById(entities.key(parent));

//    if(entity == NULL)
//    {
//      qWarning() << "[QNST] Warning: trying to add an entity as child of an inexistent Entity";
//      if(properties["TYPE"] == "importBase" ||
//         properties["TYPE"] == "causalConnector" ||
//         properties["TYPE"] == "connector" ||
//         properties["TYPE"] == "complex-connector")
//      { // In these cases we do not need a parent.
//        qWarning() << "[QNST] " << properties["TYPE"] << " is a special case. So, we will not need a parent.";
//      }
//      else
//      {
//        qWarning() << "[QNST] The entityAdd is beging ignored!";
//        return;
//      }
//    }
  }

  // Load specific properties for each type (and handle dependencies)
  switch(type)
  {
    case Structural::Body:
    {
      requestBodyDependence();
      QList<Entity*> list = getProject()->getEntitiesbyType("ncl");

      if (!list.isEmpty())
      {
        entity = list.first();

        if (!properties.value("LOCAL:ID").isEmpty())
          attributes["id"] = properties["LOCAL:ID"];

        mustEmitAddEntity = true;
      }
      break;
    }

    case Structural::Context:
    {
      if (!properties.value("LOCAL:ID").isEmpty())
        attributes["id"] = properties["LOCAL:ID"];

      if (!properties.value("LOCAL:REFER").isEmpty())
        attributes["refer"] = properties["LOCAL:REFER"];

      mustEmitAddEntity = true;
      break;
    }

    case Structural::Switch:
    {
      if (!properties.value("LOCAL:ID").isEmpty())
          attributes["id"] = properties["LOCAL:ID"];

      if (!properties.value("LOCAL:REFER").isEmpty())
        attributes["refer"] = properties["LOCAL:REFER"];

      mustEmitAddEntity = true;
      break;
    }

    case Structural::Media:
    {
      if (!properties.value("LOCAL:ID").isEmpty())
        attributes["id"] = properties["LOCAL:ID"];


      if (!properties.value("LOCAL:SRC").isEmpty())
      {
        QString src = properties["LOCAL:SRC"];
        try
        {
          attributes["src"] = Utilities::relativePath(project->getLocation(),
                                                        src, true);
        }
        catch(...)
        {
          attributes["src"] = properties["LOCAL:SRC"];
        }
      }

      if (!properties.value("LOCAL:REFER").isEmpty())
        attributes["refer"] = properties["LOCAL:REFER"];

      if (!properties.value("LOCAL:INSTANCE").isEmpty())
        attributes["instance"] = properties["LOCAL:INSTANCE"];

      if (!properties.value("CPR:TYPE").isEmpty())
        attributes["type"] = properties["CPR:TYPE"];

      if (!properties.value("CPR:DESCRIPTOR").isEmpty())
        attributes["descriptor"] = properties["CPR:DESCRIPTOR"];

      mustEmitAddEntity = true;
      break;
    }

    case Structural::Port:
    {
      attributes["id"] = properties["LOCAL:ID"];
      attributes["component"] = properties["CPR:COMPONENT"];

      if (!properties.value("CPR:INTERFACE").isEmpty())
        attributes["interface"] = properties["CPR:INTERFACE"];

      mustEmitAddEntity = true;
      break;
    }

    case Structural::Link:
    {
      attributes["id"] = properties["LOCAL:ID"];


      requestConnectorAddition(uid, parent, properties);

      attributes["xconnector"] = "conn_"+properties["LOCAL:ID"];

      mustEmitAddEntity = true;
      request = uid;
      break;
    }

    case Structural::Bind:
    {
      requestBindAddition(uid, parent, properties);
      mustEmitAddEntity = false; // the add is made inside requestBindAddition!!!
      break;
    }

    case Structural::Area:
    {
      if (!properties.value("LOCAL:ID").isEmpty())
        attributes["id"] = properties["LOCAL:ID"];

      mustEmitAddEntity = true;
      break;
    }

    case Structural::Property:
    {
      if (!properties.value("LOCAL:ID").isEmpty())
        attributes["name"] = properties["LOCAL:ID"];

      mustEmitAddEntity = true;
      break;
    }

    case Structural::SwitchPort:
    {
      if (!properties.value("LOCAL:ID").isEmpty())
        attributes["name"] = properties["LOCAL:ID"];

      mustEmitAddEntity = true;
      break;
    }

//    case Qnst::Mapping:
//    {
//      if (properties["component"] != "")
//        attributes["component"] = properties["component"];

//      if (properties["interface"] != "")
//        attributes["interface"] = properties["interface"];

//      mustEmitAddEntity = true;
//      break;
//    }

//    case Qnst::BindParam:
//    {
//      if (properties["name"] != "")
//        attributes["name"] = properties["name"];

//      if (properties["value"] != "")
//        attributes["value"] = properties["value"];

//      mustEmitAddEntity= true;
//      break;
//    }

    default:
      // do nothing
      break;
  }

  if(mustEmitAddEntity && entity != NULL) // Everything is ok, so add my new entity
  {
    request = uid;
    emit addEntity(StructuralUtil::getStrFromNstType((Structural::EntityName) properties["LOCAL:NAME"].toInt()),
                   entity->getUniqueId(), attributes, false);
    request = "";

  }
//  else
//  {
//    // Special cases!! We do not have Qnst::EntityType defined!!
//    if (properties["TYPE"] == "connector")
//      requestConnectorAddition(uid, parent, properties);
//    else if (properties["TYPE"] == "complex-connector")
//      requestComplexConnectorAddition(uid, parent, properties);
//  }

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

  Structural::EntityName type = (Structural::EntityName) properties["LOCAL:NAME"].toInt();

  QMap<QString, QString> attributes;
  Entity* entity = getProject()->getEntityById(entities.key(uid));

  bool mustSetAttributes = false;

  if(entity == NULL)
  {
    qWarning() << "[QNST] Error!!! Trying to change a entity that does not exist in the model";
  }

  switch (type)
  {
    case Structural::Body:
    {
      if (properties["LOCAL:ID"] != "")
        attributes["id"] = properties["LOCAL:ID"];

      mustSetAttributes = true;
      break;
    }

    case Structural::Context:
    case Structural::Switch:
    {
      if (properties["LOCAL:ID"] != "")
        attributes["id"] = properties["LOCAL:ID"];

      if (properties["LOCAL:REFER"] != "")
        attributes["refer"] = properties["LOCAL:REFER"];

      mustSetAttributes = true;
      break;
    }

    case Structural::Media:
    {
      if (properties["LOCAL:ID"] != "")
        attributes["id"] = properties["LOCAL:ID"];

      if (properties["LOCAL:SRC"] != "")
        attributes["src"] = properties["LOCAL:SRC"];

      if (properties["LOCAL:REFER"] != "")
        attributes["refer"] = properties["LOCAL:REFER"];

      if (properties["LOCAL:INSTANCE"] != "")
        attributes["instance"] = properties["LOCAL:INSTANCE"];

      if (properties["CPR:TYPE"] != "")
        attributes["type"] = properties["CPR:TYPE"];

      if (properties["CPR:DESCRIPTOR"] != "")
        attributes["descriptor"] = properties["CPR:DESCRIPTOR"];

      mustSetAttributes = true;
      break;
    }

    case Structural::Port:
    {
      attributes["id"] = properties["LOCAL:ID"];

      if(properties["component"] != "")
          attributes["component"] = properties["CPR:COMPONENT"];

      if(properties["interface"] != "")
          attributes["interface"] = properties["CPR:INTERFACE"];

      mustSetAttributes = true;
      break;
    }

//    case Structural::BindParam:
//    {
//      if (properties["name"] != "")
//        attributes["name"] = properties["name"];

//      if (properties["value"] != "")
//        attributes["value"] = properties["value"];

//      mustSetAttributes = true;
//      break;
//    }

//    case Structural::Mapping:
//    {
//      if (properties["component"] != "")
//        attributes["component"] = properties["component"];

//      if (properties["interface"] != "")
//        attributes["interface"] = properties["interface"];

//      mustSetAttributes = true;
//      break;
//    }

    case Structural::SwitchPort:
    {

      if (properties["LOCAL:ID"] != "")
        attributes["id"] = properties["LOCAL:ID"];

      mustSetAttributes = true;
      break;
    }

    case Structural::Area:
    {
      if (properties["LOCAL:ID"] != "")
        attributes["id"] = properties["LOCAL:ID"];

      mustSetAttributes = true;
      break;
    }

    case Structural::Property:
    {
      if (properties["LOCAL:ID"] != "")
        attributes["name"] = properties["LOCAL:ID"];

      mustSetAttributes = true;
      break;
    }

    default:
      break;
  }

  if(mustSetAttributes)
  {
    emit setAttributes(entity, attributes, false);
  }

  // \fixme By now, we are also setting additional data, but maybe this is not
  //        necessary.
  QString coreID = entities.key(uid);
  //QnstEntity *qnstEntity = view->entities.value(uid);
  //if(qnstEntity != NULL)
  //{
    //Disabled for now
    //QMap <QString, QString> props;
    //qnstEntity->getProperties(props);
    //emit setAttributes(project->getEntityById(coreID),
    //                   props, false);
  //}

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

void StructuralViewPlugin::requestConnectorAddition(const QString uid,
                                                  const QString parent,
                                       const QMap<QString, QString> &properties)
{
  requestConnectorDependence();

  qWarning() << "QnstComposerPlugin::requestConnectorAddition";

  QList<Entity*> list = getProject()->getEntitiesbyType("connectorBase");

  if (!list.isEmpty())
  {
    Entity* entity = list.first();

    if (entity != NULL)
    {
      QMap<QString, QString> attributes;

      attributes["id"] = "conn_"+properties["LOCAL:ID"];

      isConnector = true;
      request = uid;
      emit addEntity("causalConnector", entity->getUniqueId(), attributes, false);
      request = "";

      QMap<QString, QString> attrs;
      attrs["operator"] = "and";

      emit addEntity("compoundCondition", xconnector_viewLink.value(uid), attrs, false);

      attrs["operator"] = "par";
      emit addEntity("compoundAction", xconnector_viewLink.value(uid), attrs, false);

    }
  }
}

void StructuralViewPlugin::requestComplexConnectorAddition(const QString uid,
                                                         const QString parent,
                                       const QMap<QString, QString> &properties)
{
  requestConnectorDependence();

  QList<Entity*> list = getProject()->getEntitiesbyType("connectorBase");

  if (!list.isEmpty())
  {
    Entity* entity = list.first();

    if (entity != NULL)
    {
      QMap<QString, QString> attributes;

      attributes["id"] = properties["id"];

      request = uid;
      emit addEntity("causalConnector", entity->getUniqueId(), attributes, false);
      request = "";

      QMap<QString, QString> fakemap;
      fakemap["operator"] = "or";
      emit addEntity("compoundCondition", entities.key(uid), fakemap, false);

      fakemap["operator"] = "par";
      emit addEntity("compoundAction", entities.key(uid), fakemap, false);

      QString cpcUID;
      QString cpaUID;

      Entity *cmp = getProject()->getEntityById(entities.key(uid));
      if(cmp != NULL)
      {
        foreach(Entity* child, cmp->getChildren())
        {
          if (child->getType() == "compoundCondition")
            cpcUID = child->getUniqueId();

          if (child->getType() == "compoundAction")
            cpaUID = child->getUniqueId();
        }
      }

      // condition
      if (properties["condition"] != "" && cpcUID != "")
      {
        QMap<QString, QString> cattributes;
        cattributes["role"] = properties["condition"];
        cattributes["max"] = "unbounded";

        request = properties["conditionUID"];
        emit addEntity("simpleCondition", cpcUID, cattributes, false);
        request = "";
      }

      // action
      if (properties["action"] != ""  && cpaUID != "")
      {
        QMap<QString, QString> aattributes;
        aattributes["role"] = properties["action"].toLower();
        aattributes["max"] = "unbounded";

        request = properties["actionUID"];
        emit addEntity("simpleAction", cpaUID, aattributes, false);
        request = "";
      }
    }
  }
}

void StructuralViewPlugin::requestConnectorDependence()
{
  requestConnectorBaseDependence();

  QList<Entity*> list = getProject()->getEntitiesbyType("connectorBase");

  if (list.isEmpty())
  {
    Entity* parent = NULL;

    if (!getProject()->getEntitiesbyType("head").isEmpty())
      parent = getProject()->getEntitiesbyType("head").first();

    if (parent != NULL)
    {
      QMap<QString, QString> attributes;

      emit addEntity("connectorBase", parent->getUniqueId(), attributes, false);
    }
  }
}

void StructuralViewPlugin::requestConnectorBaseDependence()
{
  QList<Entity*> list = getProject()->getEntitiesbyType("head");

  if (list.isEmpty())
  {
    Entity* parent = NULL;

    if (!getProject()->getEntitiesbyType("ncl").isEmpty())
      parent = getProject()->getEntitiesbyType("ncl").first();

    if (parent != NULL)
    {
      QMap<QString, QString> attributes;

      emit addEntity("head", parent->getUniqueId(), attributes, false);
    }
  }
}

void StructuralViewPlugin::requestBindAddition(const QString uid,
                                             const QString parent,
                                       const QMap<QString, QString> &properties)
{

  QString connUID = xconnector_viewLink.value(properties["LOCAL:LINK:UID"]);

  QString cpcUID = "";
  QString cpaUID = "";

  if (connUID != "")
  {
    Entity *cmp = getProject()->getEntityById(connUID);
    if(cmp != NULL)
    {
      foreach(Entity* child, cmp->getChildren())
      {
        if (child->getType() == "compoundCondition")
          cpcUID = child->getUniqueId();

        if (child->getType() == "compoundAction")
          cpaUID = child->getUniqueId();
      }
    }
  }

  if (cpcUID != "" && cpaUID != "")
  {

    QMap<QString, QString> cattributes;
    cattributes["max"] = "unbounded";
    cattributes["role"] = StructuralUtil::getStrFromBindType((Structural::BindType) properties["LOCAL:BIND"].toInt());

    if (StructuralUtil::isCondition((Structural::BindType) properties["LOCAL:BIND"].toInt()))
    {
      Entity* cpc_entity = getProject()->getEntityById(cpcUID);

      bool alreadyHad = false;

      foreach (Entity *e_child, cpc_entity->getChildren()) {
        if (e_child->getAttribute("role") == cattributes["role"])
        {
          alreadyHad = true; break;
        }
      }

      if (!alreadyHad)
        emit addEntity("simpleCondition", cpcUID, cattributes, false);

      QMap<QString, QString> battributes;

      battributes["role"] = cattributes["role"];
      battributes["component"] = properties["LOCAL:COMPONENT:ID"];

      if (properties["LOCAL:INTERFACE:ID"] != "")
        battributes["interface"] = properties["LOCAL:INTERFACE:ID"];

      request = uid;
      emit addEntity("bind", entities.key(properties["LOCAL:LINK:UID"]), battributes, false);
      request = "";
    }
    else
    {
      Entity* cpa_entity = getProject()->getEntityById(cpaUID);

      bool alreadyHad = false;

      foreach (Entity *e_child, cpa_entity->getChildren()) {
        if (e_child->getAttribute("role") == cattributes["role"])
        {
          alreadyHad = true; break;
        }
      }

      if (!alreadyHad)
        emit addEntity("simpleAction", cpaUID, cattributes, false);

      QMap<QString, QString> battributes;

      battributes["role"] = cattributes["role"];
      battributes["component"] = properties["LOCAL:COMPONENT:ID"];

      if (properties["LOCAL:INTERFACE:ID"] != "")
        battributes["interface"] = properties["LOCAL:INTERFACE:ID"];

      request = uid;
      emit addEntity("bind", entities.key(properties["LOCAL:LINK:UID"]), battributes, false);
      request = "";
    }
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

/*
void StructuralViewPlugin::cacheNCLIds()
{
  Entity *current;
  QString coreUID, structuralID, nclID;

  nclIDtoStructural.clear();
  previousCoreID.clear();
  foreach(coreUID, entities.keys())
  {
    structuralID = entities.value(coreUID);
    current = project->getEntityById(coreUID);
    if(current != NULL)
    {
      nclID = getNCLIdFromEntity(current);
      if(!nclID.isEmpty() && !nclID.isNull())
      {
        nclIDtoStructural.insert(nclID, structuralID);
        previousCoreID.push_back(coreUID);
      }
    }
  }
}

QString StructuralViewPlugin::insertNCLIDIfEmpty(Entity *entity)
{
  const QString defaultID = "idBody000";
  QString id = entity->getAttribute("id");
  if(id.isEmpty() || id.isNull())
  {
    QMap <QString, QString> attrsTmp;
    QMap <QString, QString>::iterator begin, end, it;
    entity->getAttributeIterator(begin, end);
    for (it = begin; it != end; ++it)
    {
      attrsTmp.insert(it.key(), it.value());
    }
    attrsTmp.insert("id", defaultID); // \fixme this id should be unique
    emit setAttributes(entity, attrsTmp, false);
    return defaultID;
  }
  return id;
}

QString StructuralViewPlugin::getNCLIdFromEntity(Entity *entity)
{
  QString nclID;
  if(entity != NULL)
  {
    if(entity->getType() == "body") // forces an ID if it not exists for body!
      nclID = insertNCLIDIfEmpty(entity);

    if(entity->hasAttribute("id"))
      nclID = entity->getAttribute("id");
    else if(entity->hasAttribute("name"))
      nclID = entity->getAttribute("name");

    else if(entity->hasAttribute("alias"))
        nclID = entity->getAttribute("alias");
    else
      nclID = QUuid::createUuid().toString();

    // This nclId will contains its id (or name) and all the perspective of an
    // element.
    Entity *parent = entity->getParent();
    while(parent != NULL && parent->hasAttribute("id"))
    {
      nclID = parent->getAttribute("id") + "#" + nclID;
      parent = parent->getParent();
    }
  }
  return nclID;
}
*/
bool StructuralViewPlugin::isEntityHandled(Entity *entity)
{
  if(entity != NULL)
  {
    QString type = entity->getType();

    // \todo This could be a map or an array
    if(type == "body" || type == "context" || type == "media" ||
       type == "switch" || type == "port" || type == "link" || type == "bind" ||
       type == "area" || type == "property" || type == "causalConnector" ||
       type == "switchPort" || type == "mapping" || type == "importBase"||
       type == "bindParam" || type == "connectorParam" ||
       type == "simpleCondition" || type == "simpleAction")

      return true;
  }
  return false;
}
/*
void StructuralViewPlugin::syncNCLIdsWithStructuralIds()
{
  QMap <QString, QString> nclIDtoCoreID;
  QList <QString> nclCoreID_Ordered, alreadyUpdatedCoreId;
  QStack <Entity*> stack;
  Entity *currentEntity;
  QString currentNCLId;

  QString nclID, structuralID, coreID;

  // I will keep all nclIDs -> coreIDs that I'm interested in nclIDtoCoreID map.
  stack.push(project);
  while(stack.size())
  {
    currentEntity = stack.front();
    stack.pop_front();

    currentNCLId = getNCLIdFromEntity(currentEntity);

    if(isEntityHandled(currentEntity) &&
       !currentNCLId.isEmpty() && !currentNCLId.isNull())
    {
      nclIDtoCoreID.insert(currentNCLId, currentEntity->getUniqueId());
      nclCoreID_Ordered.push_back(currentEntity->getUniqueId());
    }

    QVector<Entity *> children = currentEntity->getChildren();
    for(int i = 0; i < children.size(); i++)
    {
      stack.push_back(children.at(i));
    }
  }

//  qDebug() << "#### nclIDtoCoreID " << nclCoreID_Ordered;
//  qDebug() << "#### previousCoreID " << previousCoreID;

  foreach(coreID, nclCoreID_Ordered)
  {
    if(previousCoreID.contains(coreID)) // if previousCoreID is still in the core
    {
      currentEntity = project->getEntityById(coreID);
      if(currentEntity != NULL)
      {
//        if(dirtyEntities.contains(currentEntity->getUniqueId()))
          requestEntityChange(currentEntity); //just update it

        alreadyUpdatedCoreId.push_back(coreID);
      }
    }
  }

// qDebug() << "Entities before sync" << entities;
// qDebug() << "nclIDtoCoreID" << nclIDtoCoreID.keys();
// qDebug() << "nclIDtoStructural" << nclIDtoStructural.keys();
  //for each old entity that are in the structural (not necessarily in the core)
  foreach (nclID, nclIDtoStructural.keys())
  {
    structuralID = nclIDtoStructural.value(nclID);
    if(nclIDtoCoreID.contains(nclID))
    {
      // gets the new coreId for currentStructuralId
      coreID = nclIDtoCoreID.value(nclID);

      if(alreadyUpdatedCoreId.contains(coreID))// this entity is already up to date.
        continue;

      // remove the previous mention to coreID
      entities.remove(entities.key(structuralID));
      // update the map with the new coreID
      entities[coreID] = structuralID;
      currentEntity = project->getEntityById(coreID);
      if(currentEntity != NULL)
      {
        requestEntityChange(currentEntity);
        alreadyUpdatedCoreId.push_back(coreID);
      }
    }
    else
    {
      if(alreadyUpdatedCoreId.contains(entities.key(structuralID)))
        continue;

      // keep track of all entities that must be removed from structural
      //view->ref_removeEntity(structuralID);
      entities.remove(entities.key(structuralID));
    }
  }

  // search for the entities that are in the core, but not in the structural
  // view
  for(int i = 0; i < nclCoreID_Ordered.size(); i++)
  {
    coreID = nclCoreID_Ordered.at(i);
    if(alreadyUpdatedCoreId.contains(coreID))// this entity is already up to date.
      continue;

    currentEntity = project->getEntityById(coreID);
    if(!entities.contains(coreID) && currentEntity != NULL)
    {
      requestEntityAddition(currentEntity);
    }
  }

// qDebug() << "Entities after sync" << entities;
}

void StructuralViewPlugin::textualStartSync(QString, void*)
{
  // qDebug() << "QnstComposerPlugin::textualStartSync";
  // cacheNCLIds();
  dirtyEntities.clear();
  isSyncingFromTextual = true;
}

void StructuralViewPlugin::textualFinishSync(QString, void*)
{
  qDebug() << "QnstComposerPlugin::textualFinishSync";
  isSyncingFromTextual = false;
  // syncNCLIdsWithStructuralIds();
  updateFromModel();
}
*/
void StructuralViewPlugin::clearValidationError(QString, void *param)
{
 // view->clearValidationErrors();
}

void StructuralViewPlugin::validationError(QString pluginID, void *param)
{
//  if(isSyncingFromTextual)
//    return;

  if(param)
  {
    pair <QString, QString> *p = (pair <QString, QString> *)param;

    //if (entities.contains(p->first))
      //view->markError(entities.value(p->first), p->second);
  }
}
