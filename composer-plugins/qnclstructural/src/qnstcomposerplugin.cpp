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
#include "qnstcomposerplugin.h"

//
// ATTENTION: This code needs a refactoring.
//

QnstComposerPlugin::QnstComposerPlugin(QObject* parent)
{
  setParent(parent);

  createWidgets();
  createConnections();

  n = 0;

  request = "";

  isSyncingFromTextual = false;
}

QnstComposerPlugin::~QnstComposerPlugin()
{
  delete(view);
}

void QnstComposerPlugin::createWidgets()
{
  view = new QnstView();
}

void QnstComposerPlugin::createConnections()
{
  connect(view, SIGNAL(entityAdded(QString,QString,QMap<QString,QString>)),
          SLOT(notifyEntityAddedInView(QString,QString,QMap<QString,QString>)));

  connect(view, SIGNAL(entityRemoved(QString)),
                SLOT(notifyEntityDeletedInView(QString)));

  connect(view, SIGNAL(entityChanged(QString,QMap<QString,QString>)),
                SLOT(notifyEntityChangedInView(QString,QMap<QString,QString>)));

  connect(view, SIGNAL(entitySelected(QString)),
                 SLOT(requestEntitySelection(QString)));

  connect(view, SIGNAL(viewChanged()), SIGNAL(setCurrentProjectAsDirty()));
}

void QnstComposerPlugin::init()
{
  QString data = project->getPluginData("br.puc-rio.telemidia.qnst");

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
        if(entity != NULL)
        {
          entity->getAttributeIterator(begin, end);

          for (it = begin; it != end; ++it)
            userData[it.key()] = it.value();

          nstEntity->setUsrData(userData);
        }
      }
    }

    lastp = nextp+1;
    nextp = mid.indexOf("~", nextp+1);
  }
}

QWidget* QnstComposerPlugin::getWidget()
{
  return view;
}

bool QnstComposerPlugin::saveSubsession()
{
  QByteArray data;

  data.append("$START_ENTITIES_LINES$");

  data.append("~~VIEW-DATA~~");
  data.append(view->serialize());

  data.append("~~PLUGIN-DATA~~");
  foreach(QString key, entities.keys()){
      data.append(key+"="+entities[key]+"~");
  }

  data.append("$END_ENTITIES_LINES$");

  emit setPluginData(data);

  return true;
}

void QnstComposerPlugin::updateFromModel()
{
  QMap <QString, int> top, left, width, height;

  //save previous positions
  foreach (QString key, entities.keys())
  {
    QnstGraphicsEntity *entity =
        dynamic_cast <QnstGraphicsEntity*>(view->getEntity(entities.value(key)));
    if(entity && entity->getnstId() != "")
    {
      top[entity->getnstId()] = entity->getTop();
      left[entity->getnstId()]=  entity->getLeft();
      height[entity->getnstId()] = entity->getHeight();
      width[entity->getnstId()] = entity->getWidth();
    }
  }

  // \todo Keep the previous media position!!
  foreach(QString coreID, entities.keys())
  {
    Entity *entity = project->getEntityById(coreID);
    requestEntityRemotion(entity);
  }

  view->clearAllData();

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
    QnstGraphicsEntity *entity =
        dynamic_cast <QnstGraphicsEntity*>(view->getEntity(entities.value(key)));
    if(entity && entity->getnstId() != "" && top.contains(entity->getnstId()))
    {
      entity->setTop(top[entity->getnstId()]);
      entity->setLeft(left[entity->getnstId()]);
      entity->setWidth(width[entity->getnstId()]);
      entity->setHeight(height[entity->getnstId()]);
    }
  }

  view->adjustAll();
}

void QnstComposerPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
  if(isSyncingFromTextual) return;

  if (pluginID != getPluginInstanceID()){
    requestEntityAddition(entity);
  }
  else
  {
    if (request != "")
    {
      entities[entity->getUniqueId()] = request;
    }
  }

  //Add user data
  if(entities.contains(entity->getUniqueId()))
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

//        qDebug() << "[QNST] QnstComposerPlugin::onEntityAdded" << userData;

    }
    else
      qWarning() << "[QNST] QnsComposerPlugin::onEntityAdded entity "
                 << request << "is not in the view.";
  }
  else
    qWarning() << "[QNST] QnsComposerPlugin::onEntityAdded Entity was not "
               << "added";

  // \todo This must be incremental
  view->clearValidationErrors();
}

void QnstComposerPlugin::errorMessage(QString error)
{

}

void QnstComposerPlugin::onEntityChanged(QString pluginID, Entity *entity)
{    
  if(isSyncingFromTextual)
  {
    dirtyEntities.push_back(entity->getUniqueId());
    return;
  }

  if (pluginID != getPluginInstanceID())
    requestEntityChange(entity);

  //Update user data
  if(entities.contains(entity->getUniqueId()))
  {
    QString structuralId = entities.value(entity->getUniqueId());

    if(view->hasEntity(structuralId))
    {
      QnstEntity *nstEntity = view->getEntity(structuralId);

      if(nstEntity != NULL)
      {
        QMap <QString, QString>::iterator begin, end, it;
        QMap <QString, QString> userData;
        entity->getAttributeIterator(begin, end);
        for (it = begin; it != end; ++it)
          userData[it.key()] = it.value();

        nstEntity->setUsrData(userData);

        qDebug() << "[QNST] QnstComposerPlugin::onEntityChanged" << userData;
      }
    }
  }

  // \todo This must be incremental
  view->clearValidationErrors();
}

void QnstComposerPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
  if(isSyncingFromTextual) return;

  if (pluginID != getPluginInstanceID())
  {
      requestEntityRemotion(getProject()->getEntityById(entityID));
      entities.remove(entityID);
  }
}

void QnstComposerPlugin::changeSelectedEntity(QString pluginID, void* param)
{
  if(isSyncingFromTextual) return;
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

void QnstComposerPlugin::requestEntityAddition(Entity* entity)
{
  qDebug() << "QnstComposerPlugin::requestEntityAddition" << entity->getType();

  QMap<QString, QString> properties;

  // get the qnst type based on NCL Composer Type
  Qnst::EntityType type = QnstUtil::getnstTypeFromStr(entity->getType());
  properties["TYPE"] = QnstUtil::getStrFromNstType(type);

  bool ok = false;

  switch (type)
  {
    case Qnst::Body:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      if (entity->getAttribute("id") != "")
        properties["id"] = entity->getAttribute("id");

      ok = true;
      break;
    }

    case Qnst::Context:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      if (entity->getAttribute("id") != "")
        properties["id"] = entity->getAttribute("id");

      if (entity->getAttribute("refer") != "")
        properties["refer"] = entity->getAttribute("refer");

      ok = true;

      break;
    }

    case Qnst::Switch:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      if (entity->getAttribute("id") != "")
        properties["id"] = entity->getAttribute("id");

      if (entity->getAttribute("refer") != "")
        properties["refer"] = entity->getAttribute("refer");

      ok = true;

      break;
    }

    case Qnst::Media:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["id"] = entity->getAttribute("id");
      properties["src"] = entity->getAttribute("src");
      properties["refer"] = entity->getAttribute("refer");
      properties["instance"] = entity->getAttribute("instance");
      properties["type"] = entity->getAttribute("type");
      properties["descriptor"] = entity->getAttribute("descriptor");

      if (entity->getAttribute("refer") != "")
      {
        QString referUID = getUidById(properties["refer"]);
        if(entities.contains(referUID))
          properties["referUID"] = entities[referUID];
      }
      else
      {
        properties["referUID"] = "";
      }

      // \fixme This must be made after the switch/case!!
      if(entities.contains(entity->getParentUniqueId()))
      {
        view->addEntity(entity->getUniqueId(),
                        entities[entity->getParentUniqueId()],
                        properties);

        QList<Entity*> list;

        list = getProject()->getEntitiesbyType("port");

        // update properties?
        foreach(Entity* e, list)
        {
          if (e->getAttribute("component") != "" &&
              e->getAttribute("component") == properties["id"])
          {
            requestEntityChange(e);
          }
        }
      }
      break;
    }

    case Qnst::Port:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["id"] = entity->getAttribute("id");

      properties["component"] = entity->getAttribute("component");
      if (entity->getAttribute("component") != "")
      {
        QString componentUID = getUidById(properties["component"]);
        if(entities.contains(componentUID))
          properties["componentUid"] = entities[componentUID];
      }

      properties["interface"] = entity->getAttribute("interface");
      if (entity->getAttribute("interface") != "")
      {
          QString interfaceUID = getUidById(properties["interface"]);
          if(entities.contains(interfaceUID))
            properties["interfaceUid"] = entities[interfaceUID];
      }

      ok = true;

      break;
    }

    case Qnst::Link:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      if (entity->getAttribute("id") != "")
        properties["id"] = entity->getAttribute("id");

      if (entity->getAttribute("xconnector") != "")
      {
        properties["xconnector"] = entity->getAttribute("xconnector");

        QString xconnectorUID = getUidById(properties["xconnector"]);
        if(entities.contains(xconnectorUID))
          properties["xconnectorUID"] = entities[xconnectorUID];
        else
          properties["xconnectorUID"] = "";
      }

      ok = true;

      break;
    }

    case Qnst::Bind:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["role"] = entity->getAttribute("role");
      properties["component"] = entity->getAttribute("component");

      QString comUID = "";
      if (entity->getAttribute("component") != "")
      {
        comUID = getUidById(properties["component"]);
        if(entities.contains(comUID))
          properties["componentUid"] = entities[comUID];
        else
          properties["componentUid"] = "";
      }
      else
      {
        properties["componentUid"] = "";
      }

      properties["interface"] = entity->getAttribute("interface");

      if (entity->getAttribute("interface") != "")
      {
        Entity* cmp = getProject()->getEntityById(comUID);
        if(cmp != NULL)
        {
          QString intUID = "";
          foreach(Entity* c, cmp->getChildren())
          {
            if (c->getAttribute("id") == entity->getAttribute("interface") ||
                c->getAttribute("name") == entity->getAttribute("interface"))
            {
              intUID = c->getUniqueId();
              break;
            }
          }

        if(entities.contains(intUID))
         properties["interfaceUid"] = entities[intUID];
        else
          properties["interfaceUid"] = "";
        }
      }
      else
      {
        properties["interfaceUid"] = "";
      }

      properties["linkUID"] = entities[entity->getParentUniqueId()];

      ok = true;
      break;
    }

    case Qnst::Property:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["id"] = entity->getAttribute("name");

      ok = true;
      break;
    }

    case Qnst::Area:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["id"] = entity->getAttribute("id");

      ok = true;
      break;
    }

    case Qnst::SwitchPort:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["id"] = entity->getAttribute("id");

      ok = true;
      break;
    }

    case Qnst::Mapping:
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      properties["component"] = entity->getAttribute("component");
      if (entity->getAttribute("component") != "")
      {
        QString componentUID = getUidById(properties["component"]);
        if(entities.contains(componentUID))
          properties["componentUid"] = entities[componentUID];
      }

      properties["interface"] = entity->getAttribute("interface");
      if (entity->getAttribute("interface") != "")
      {
        QString interfaceUID = getUidById(properties["interface"]);
        if(entities.contains(interfaceUID))
          properties["interfaceUid"] = entities[interfaceUID];
      }

      ok = true;
      break;
    }

    case Qnst::BindParam:
    {
      // \fixme Why we do not add to entities map ??
      properties["name"] = entity->getAttribute("name");
      properties["value"] = entity->getAttribute("value");

      ok = true;
      break;
    }

    default:
      // do nothing!!
      break;

  }

  if(ok)
  {
    // if it is body we are with no parent
    if(type == Qnst::Body)
    {
      view->addEntity(entity->getUniqueId(), "", properties);
    }
    // otherwise, we need to set add with a parent
    else
    {
      QString parentUID = entity->getParentUniqueId();
      if(entities.contains(parentUID))
        view->addEntity(entity->getUniqueId(), entities[parentUID], properties);
      else
         qWarning() << "QnstPlugin:: You are trying to add a "
                    << entity->getType()
                    << " inside an entity that does not exists.";
    }
  }
  else
  {
    // Special cases!! We do not have Qnst::EntityType defined!!
    if (entity->getType() == "causalConnector")
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      requestCausalConnectorAddition(entity);
    }
    else if (entity->getType() == "simpleCondition")
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      requestSimpleConditionAddition(entity);
    }
    else if (entity->getType() == "simpleAction")
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();

      requestSimpleActionAddition(entity);
    }
    else if (entity->getType() == "importBase")
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();
      requestImportBaseAddition(entity);
    }
    else if (entity->getType() == "connectorParam")
    {
      entities[entity->getUniqueId()] = entity->getUniqueId();
      requestConnectorParamAddition(entity);
    }
  }
}

void QnstComposerPlugin::requestEntityRemotion(Entity* entity)
{
  // \fixme It is still missing connector, simpleCondition and simpleAction
  if (isEntityHandled(entity))
  {
    QString entityID = entity->getUniqueId();
    if(entities.contains(entityID))
    {
      view->removeEntity(entities[entityID]);
      entities.remove(entityID);
    }
    else
      qWarning() << "You are trying to remove an entity that are not in the QnstComposerPlugin.";
  }
}

void QnstComposerPlugin::requestEntityChange(Entity* entity)
{
  QMap<QString, QString> properties;

  // get the qnst type based on NCL Composer Type
  int type = QnstUtil::getnstTypeFromStr(entity->getType());

  bool ok = false;

  switch (type)
  {
    case Qnst::Body:
    {
      if (entity->getAttribute("id") != "")
        properties["id"] = entity->getAttribute("id");

      ok = true;
      break;
    }
    case Qnst::Context:
    case Qnst::Switch:
    {
      if (entity->getAttribute("id") != "")
        properties["id"] = entity->getAttribute("id");

      if (entity->getAttribute("refer") != "")
        properties["refer"] = entity->getAttribute("refer");

      ok = true;
      break;
    }

    case Qnst::Media:
    {
      properties["id"] = entity->getAttribute("id");
      properties["src"] = entity->getAttribute("src");
      properties["refer"] = entity->getAttribute("refer");
      properties["instance"] = entity->getAttribute("instance");
      properties["type"] = entity->getAttribute("type");
      properties["descriptor"] = entity->getAttribute("descriptor");

      if(properties["refer"] != "")
      {
        QString referUID = getUidById(properties["refer"]);
        if(entities.contains(referUID))
          properties["referUID"] = entities[referUID];
      }
      else
      {
        properties["referUID"] = "";
      }

      if(entities.contains(entity->getUniqueId()))
        view->changeEntity( entities[entity->getUniqueId()],
                            properties);

      QList<Entity*> list;
      list = getProject()->getEntitiesbyType("port");

      foreach(Entity* e, list){
        if (e->getAttribute("component") != "")
        {
          if (e->getAttribute("component") == properties["id"])
          {
            requestEntityChange(e);
          }
        }
      }

      ok = true;
      break;
    }

    case Qnst::Port:
    {
      properties["id"] = entity->getAttribute("id");
      properties["component"] = entity->getAttribute("component");
      if (entity->getAttribute("component") != "")
      {
        QString componentUID = getUidById(properties["component"]);
        if(entities.contains(componentUID))
          properties["componentUid"] = entities[componentUID];
      }

      properties["interface"] = entity->getAttribute("interface");
      if (entity->getAttribute("interface") != "")
      {
        QString interfaceUID = getUidById(properties["interface"]);
        if(entities.contains(interfaceUID))
          properties["interfaceUid"] = entities[interfaceUID];
      }

      ok = true;
      break;
    }

    case Qnst::Link:
    {
      if (entity->getAttribute("id") != "")
        properties["id"] = entity->getAttribute("id");

      if (entity->getAttribute("xconnector") != "")
      {
        properties["xconnector"] = entity->getAttribute("xconnector");
        if(entities.contains(getUidById(properties["xconnector"])))
        {
          properties["xconnectorUID"] =
              entities[getUidById(properties["xconnector"])];
        }
        else
          properties["xconnectorUID"] = "";
      }

      ok = true;
      break;
    }

    case Qnst::Bind:
    case Qnst::Action:
    case Qnst::Condition:
    {
      qWarning() << "[QNST] Changing bind on Plugin!!!";
      properties["TYPE"] = "bind";
      properties["role"] = entity->getAttribute("role");
      properties["component"] = entity->getAttribute("component");

      QString comUID = "";
      if (entity->getAttribute("component") != "")
      {
        comUID = getUidById(properties["component"]);
        if(entities.contains(comUID))
          properties["componentUid"] = entities[comUID];
        else
          properties["componentUid"] = "";
      }
      else
      {
        properties["componentUid"] = "";
      }

      properties["interface"] = entity->getAttribute("interface");
      if (entity->getAttribute("interface") != "")
      {
        Entity* cmp = getProject()->getEntityById(comUID);

        if(cmp != NULL)
        {
          QString intUID = "";
          foreach(Entity* c, cmp->getChildren())
          {
            if (c->getAttribute("id") == entity->getAttribute("interface") ||
                c->getAttribute("name") == entity->getAttribute("interface"))
            {
              intUID = c->getUniqueId();
              break;
            }
          }

          if(entities.contains(intUID))
            properties["interfaceUid"] = entities[intUID];
          else
            properties["interfaceUid"] = "";
        }
      }
      else
      {
        properties["interfaceUid"] = "";
      }

      // get the parent
      properties["linkUID"] = entities[entity->getParentUniqueId()];

      ok = true;
      break;
    }

    case Qnst::Area:
    {
      properties["TYPE"] = "area"; // Do I need that?
      properties["id"] = entity->getAttribute("id");

      ok = true;
      break;
    }

    case Qnst::Property:
    {
      properties["TYPE"] = "property"; // Do I need that?
      properties["id"] = entity->getAttribute("name");

      ok = true;
      break;
    }

    case Qnst::SwitchPort:
    {
      properties["id"] = entity->getAttribute("id");
      ok = true;
      break;
    }

    case Qnst::Mapping:
    {
      properties["component"] = entity->getAttribute("component");
      if (entity->getAttribute("component") != "")
      {
        QString componentUID = getUidById(properties["component"]);
        if(entities.contains(componentUID))
          properties["componentUid"] = entities[componentUID];
      }

      properties["interface"] = entity->getAttribute("interface");
      if (entity->getAttribute("interface") != "")
      {
        QString interfaceUID = getUidById(properties["interface"]);
        if(entities.contains(interfaceUID))
          properties["interfaceUid"] = entities[interfaceUID];
      }

      ok = true;
      break;
    }

    case Qnst::BindParam:
    {
      properties["TYPE"] = "bindParam";

      properties["name"] = entity->getAttribute("name");
      properties["value"] = entity->getAttribute("value");
      properties["parent"] = entities[entity->getParentUniqueId()];

      ok = true;
      break;
    }
  }

  if(ok)
  {
    if(entities.contains(entity->getUniqueId()))
      view->changeEntity(entities[entity->getUniqueId()], properties);
    else
      qWarning() << "QnstPlugin:: You are trying to EDIT an entity that does not exists.";
  }
  else
  {
    // Special cases!! We do not have Qnst::EntityType defined!!
    if (entity->getType() == "causalConnector")
    {
      requestCausalConnectorChange(entity);
    }
    // if the entity is of type SimpleCondition
    else if (entity->getType() == "simpleCondition")
    {
      requestSimpleConditionChange(entity);
    }
    // if the entity is of type SimpleAction
    else if (entity->getType() == "simpleAction")
    {
      requestSimpleActionChange(entity);
    }
    // if the entity is of type ImportBase
    else if (entity->getType() == "importBase")
    {
      requestImportBaseChange(entity);
    }
    else if (entity->getType() == "connectorParam")
    {
      // \fixme Do what ?
    }
  }
}

void QnstComposerPlugin::requestEntitySelection(Entity* entity)
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
      view->selectEntity(entities[entity->getUniqueId()]);
    }
  }
}

void QnstComposerPlugin::requestImportBaseAddition(Entity* entity)
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
        view->addEntity(entity->getUniqueId(), "",properties);
      }
    }
  }
}

void QnstComposerPlugin::requestImportBaseChange(Entity* entity)
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

        view->changeEntity(entity->getUniqueId(),properties);
      }
    }
  }
}

void QnstComposerPlugin::requestCausalConnectorAddition(Entity* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "causalConnector";

  if (entity->getAttribute("id") != "")
    properties["id"] = entity->getAttribute("id");

  if(entity->getParent()->getType() == "importBase")
    view->addEntity(entity->getUniqueId(), entity->getParentUniqueId(),
                    properties);
  else
    view->addEntity(entity->getUniqueId(),"", properties);
}

void QnstComposerPlugin::requestCausalConnectorChange(Entity* entity)
{
  QMap<QString, QString> properties;

  if (entity->getAttribute("id") != "")
    properties["id"] = entity->getAttribute("id");

  if(entities.contains(entity->getUniqueId()))
    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestSimpleConditionAddition(Entity* entity)
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

  if(entities.contains(entity->getParentUniqueId()))
    view->addEntity(entity->getUniqueId(),
                    entities[entity->getParentUniqueId()], properties);
  else
    view->addEntity(entity->getUniqueId(), "", properties);
}

void QnstComposerPlugin::requestSimpleConditionChange(Entity* entity)
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

  if(entities.contains(entity->getUniqueId()))
    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestSimpleActionAddition(Entity* entity)
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

  if(entities.contains(entity->getParentUniqueId()))
    view->addEntity(entity->getUniqueId(),
                    entities[entity->getParentUniqueId()], properties);
  else
    view->addEntity(entity->getUniqueId(), "", properties);
}

void QnstComposerPlugin::requestSimpleActionChange(Entity* entity)
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

  if(entities.contains(entity->getUniqueId()))
    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestConnectorParamAddition(Entity* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "connectorParam";

  properties["name"] = entity->getAttribute("name");

  QString parentUID = entity->getParentUniqueId();
  if(entities.contains(parentUID))
    view->addEntity(entity->getUniqueId(), entities[parentUID], properties);
  else
    qWarning() << "QnstPlugin:: You are trying to add an CONNECTORPARAM as child of an entity that does not exists.";

}

// \fixme this methos is not being called anywhere!!!
void QnstComposerPlugin::requestConnectorParamChange(Entity* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "connectorParam";

  properties["name"] = entity->getAttribute("name");
  properties["parent"] = entities[entity->getParentUniqueId()];

  if(entities.contains(entity->getUniqueId()))
    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::notifyEntityAddedInView(const QString uid,
                                                 const QString parent,
                                              QMap<QString, QString> properties)
{
  Qnst::EntityType type = QnstUtil::getnstTypeFromStr(properties["TYPE"]);

  Entity* entity = NULL;

  QMap<QString, QString> attributes;
  bool mustEmitAddEntity = false;

  if(type != Qnst::Body)
  {
    entity = getProject()->getEntityById(entities.key(parent));

    if(entity == NULL)
    {
      qWarning() << "[QNST] Error: trying to add an entity as child of an inexistent Entity";
      return;
    }

  }

  // Load specific properties for each type (and handle dependencies)
  switch(type)
  {
    case Qnst::Body:
    {
      requestBodyDependence();
      QList<Entity*> list = getProject()->getEntitiesbyType("ncl");

      if (!list.isEmpty())
      {
        entity = list.first();

        if (properties["id"] != "")
          attributes["id"] = properties["id"];

        mustEmitAddEntity = true;
      }
      break;
    }

    case Qnst::Context:
    {
      if (properties["id"] != "")
        attributes["id"] = properties["id"];

      if (properties["refer"] != "")
        attributes["refer"] = properties["refer"];

      mustEmitAddEntity = true;
      break;
    }

    case Qnst::Switch:
    {
      if (properties["id"] != "")
        attributes["id"] = properties["id"];

      if (properties["refer"] != "")
        attributes["refer"] = properties["refer"];

      mustEmitAddEntity = true;
      break;
    }

    case Qnst::Media:
    {
      if (properties["id"] != "")
        attributes["id"] = properties["id"];

      if (properties["src"] != "")
      {
        QString src = properties["src"];
        try
        {
          attributes["src"] = Utilities::relativePath(project->getLocation(),
                                                        src, true);
        }
        catch(...)
        {
          attributes["src"] = properties["src"];
        }
      }

      if (properties["refer"] != "")
        attributes["refer"] = properties["refer"];

      if (properties["instance"] != "")
        attributes["instance"] = properties["instance"];

      if (properties["type"] != "")
        attributes["type"] = properties["type"];

      if (properties["descriptor"] != "")
        attributes["descriptor"] = properties["descriptor"];

      mustEmitAddEntity = true;
      break;
    }
    case Qnst::Port:
    {
      attributes["id"] = properties["id"];
      attributes["component"] = properties["component"];

      if(properties["interface"] != "")
        attributes["interface"] = properties["interface"];

      mustEmitAddEntity = true;
      break;
    }

    case Qnst::Link:
    {
      attributes["id"] = properties["id"];
      attributes["xconnector"] = properties["xconnector"];

      mustEmitAddEntity = true;
      request = uid;
      break;
    }

    case Qnst::Bind:
    {
      requestBindAddition(uid, parent, properties);
      mustEmitAddEntity = false; // the add is made inside requestBindAddition!!!
      break;
    }

    case Qnst::Area:
    {
      attributes["id"] = properties["id"];
      mustEmitAddEntity = true;
      break;
    }

    case Qnst::Property:
    {
      attributes["name"] = properties["id"];

      mustEmitAddEntity = true;
      break;
    }

    case Qnst::SwitchPort:
    {
      if (properties["id"] != "")
        attributes["id"] = properties["id"];

      mustEmitAddEntity = true;
      break;
    }

    case Qnst::Mapping:
    {
      if (properties["component"] != "")
        attributes["component"] = properties["component"];

      if (properties["interface"] != "")
        attributes["interface"] = properties["interface"];

      mustEmitAddEntity = true;
      break;
    }

    case Qnst::BindParam:
    {
      if (properties["name"] != "")
        attributes["name"] = properties["name"];

      if (properties["value"] != "")
        attributes["value"] = properties["value"];

      mustEmitAddEntity= true;
      break;
    }

    default:
      // do nothing
      break;
  }

  if(mustEmitAddEntity && entity != NULL) // Everything is ok, so add my new entity
  {
    request = uid;
    emit addEntity(properties["TYPE"],
                   entity->getUniqueId(), attributes, false);
    request = "";

  }
  else
  {
    // Special cases!! We do not have Qnst::EntityType defined!!
    if (properties["TYPE"] == "connector")
      requestConnectorAddition(uid, parent, properties);
    else if (properties["TYPE"] == "complex-connector")
      requestComplexConnectorAddition(uid, parent, properties);
  }
}

void QnstComposerPlugin::notifyEntityDeletedInView(const QString uid)
{
  if (entities.key(uid, "nil") != "nil")
  {
    emit removeEntity(getProject()->getEntityById(entities.key(uid)), false);
    entities.remove(entities.key(uid));
  }
}

void QnstComposerPlugin::notifyEntityChangedInView(const QString uid,
                                              QMap<QString, QString> properties)
{
  Qnst::EntityType type = QnstUtil::getnstTypeFromStr(properties["TYPE"]);

  QMap<QString, QString> attributes;
  Entity* entity = getProject()->getEntityById(entities.key(uid));

  bool mustSetAttributes = false;

  if(entity == NULL)
  {
    qWarning() << "[QNST] Error!!! Trying to change a entity that does not exist in the model";
  }

  switch (type)
  {
    case Qnst::Body:
    {
      if (properties["id"] != "")
        attributes["id"] = properties["id"];

      mustSetAttributes = true;
      break;
    }

    case Qnst::Context:
    case Qnst::Switch:
    {
      if (properties["id"] != "")
        attributes["id"] = properties["id"];

      if (properties["refer"] != "")
        attributes["refer"] = properties["refer"];

      mustSetAttributes = true;
      break;
    }

    case Qnst::Media:
    {
      if (properties["id"] != "")
        attributes["id"] = properties["id"];

      if (properties["src"] != "")
        attributes["src"] = properties["src"];

      if (properties["refer"] != "")
        attributes["refer"] = properties["refer"];

      if (properties["instance"] != "")
        attributes["instance"] = properties["instance"];

      if (properties["type"] != "")
        attributes["type"] = properties["type"];

      if (properties["descriptor"] != "")
        attributes["descriptor"] = properties["descriptor"];

      mustSetAttributes = true;
      break;
    }

    case Qnst::Port:
    {
      attributes["id"] = properties["id"];

      if(properties["component"] != "")
          attributes["component"] = properties["component"];

      if(properties["interface"] != "")
          attributes["interface"] = properties["interface"];

      mustSetAttributes = true;
      break;
    }

    case Qnst::BindParam:
    {
      qWarning() << "Changing bindParam";
      if (properties["name"] != "")
        attributes["name"] = properties["name"];

      if (properties["value"] != "")
        attributes["value"] = properties["value"];

      mustSetAttributes = true;
      break;
    }

    case Qnst::Mapping:
    {
      if (properties["component"] != "")
        attributes["component"] = properties["component"];

      if (properties["interface"] != "")
        attributes["interface"] = properties["interface"];

      mustSetAttributes = true;
      break;
    }

    case Qnst::SwitchPort:
    {

      if (properties["id"] != "")
        attributes["id"] = properties["id"];

      mustSetAttributes = true;
      break;
    }
    case Qnst::Area:
    case Qnst::Property:
      // \todo Changing of property and area.
      break;

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
  QnstEntity *qnstEntity = view->entities.value(uid);
  if(qnstEntity != NULL)
  {
    emit setAttributes(project->getEntityById(coreID),
                       qnstEntity->getUsrData(), false);
  }
}

void QnstComposerPlugin::requestEntitySelection(const QString uid)
{
  if (entities.key(uid, "nil") != "nil")
  {
    lastSelected = entities.key(uid);

    emit sendBroadcastMessage("changeSelectedEntity",
                              new QString(entities.key(uid)));
  }
}

void QnstComposerPlugin::requestBodyDependence()
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

void QnstComposerPlugin::requestConnectorAddition(const QString uid,
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

      // condition
      QMap<QString, QString> cattributes;
      cattributes["role"] = properties["condition"];
      cattributes["max"] = "unbounded";

      emit addEntity("simpleCondition", entities.key(uid), cattributes, false);

      // action
      QMap<QString, QString> aattributes;
      aattributes["role"] = properties["action"].toLower();
      aattributes["max"] = "unbounded";

      emit addEntity("simpleAction", entities.key(uid), aattributes, false);
    }
  }
}

void QnstComposerPlugin::requestComplexConnectorAddition(const QString uid,
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

      emit addEntity("compoundCondition", entities.key(uid), fakemap,false);
      emit addEntity("compoundAction", entities.key(uid), fakemap,false);

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

        emit addEntity("simpleCondition", cpcUID, cattributes, false);
      }

      // action
      if (properties["action"] != ""  && cpaUID != "")
      {
        QMap<QString, QString> aattributes;
        aattributes["role"] = properties["action"].toLower();
        aattributes["max"] = "unbounded";

        emit addEntity("simpleAction", cpaUID, aattributes, false);
      }
    }
  }
}

void QnstComposerPlugin::requestConnectorDependence()
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

void QnstComposerPlugin::requestConnectorBaseDependence()
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

void QnstComposerPlugin::requestBindAddition(const QString uid,
                                             const QString parent,
                                       const QMap<QString, QString> &properties)
{
  qWarning() <<  "[QNST] requestBindAddition " << properties;
  // checking connector
  QList<Entity*> connectors = getProject()->getEntitiesbyType("causalConnector");

  QString connUID = "";

  foreach(Entity* conn, connectors)
  {
    if (conn->getAttribute("id") == properties["connector"])
    {
      connUID = conn->getUniqueId();
      break;
    }
  }

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
    if (properties["condition"] != "")
    {
      bool hasCond = false;
      Entity *cmp = getProject()->getEntityById(cpcUID);
      if(cmp != NULL)
      {
        foreach(Entity* child, cmp->getChildren())
        {
          if (child->getAttribute("role") == properties["condition"])
          {
            hasCond = true;
            break;
          }
        }
      }

      if (!hasCond)
      {
        QMap<QString, QString> cattributes;

        cattributes["role"] = properties["condition"];

        emit addEntity("simpleCondition", cpcUID, cattributes, false);
      }
    }

    if (properties["action"] != "")
    {
      bool hasAct = false;

      Entity *cmp = getProject()->getEntityById(cpaUID);
      if(cmp != NULL)
      {
        foreach(Entity* child, cmp->getChildren())
        {
          if (child->getAttribute("role") == properties["action"].toLower())
          {
            hasAct = true;
            break;
          }
        }
      }

      if (!hasAct)
      {
        QMap<QString, QString> aattributes;
        aattributes["role"] = properties["action"].toLower();

        emit addEntity("simpleAction", cpaUID, aattributes, false);
      }
    }
  }

  //checking link
  QList<Entity*> links = getProject()->getEntitiesbyType("link");

  QString liknUID = "";

  foreach(Entity* link, links)
  {
    if (link->getAttribute("id") == properties["link"])
    {
      liknUID = link->getUniqueId();
      break;
    }
  }

  if (liknUID == "")
  {
    QMap<QString, QString> linkattributes;

    linkattributes["id"] = properties["link"];
    linkattributes["xconnector"] = properties["connector"];

    if (properties["linkUID"] != "")
      request = properties["linkUID"];
    else
      request = QUuid::createUuid().toString();

    // \todo This could results in a Null pointer.
    emit addEntity("link",
                   getProject()->getEntityById(entities.key(parent))->getUniqueId(),
                   linkattributes, false);
    request = "";

    liknUID = getUidById(linkattributes["id"]);
  }

  // add bind
  if (properties["condition"] != "")
  {
    // bind condition
    QMap<QString, QString> cattributes;

    cattributes["role"] = properties["condition"];
    cattributes["component"] = properties["component"];

    if (properties["interface"] != "")
      cattributes["interface"] = properties["interface"];

    request = uid;
    emit addEntity("bind", liknUID, cattributes, false);
    request = "";

  }
  else if (properties["action"] != "")
  {
    // bind action
    QMap<QString, QString> aattributes;

    aattributes["role"] = properties["action"].toLower();
    aattributes["component"] = properties["component"];

    if (properties["interface"] != "")
      aattributes["interface"] = properties["interface"];

    request = uid;
    emit addEntity("bind", liknUID, aattributes, false);
    request = "";
  }
}

QString QnstComposerPlugin::getUidById(QString id)
{
  return getUidById(id, getProject());
}

QString QnstComposerPlugin::getUidById(QString id, Entity* entity)
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

void QnstComposerPlugin::cacheNCLIds()
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

QString QnstComposerPlugin::insertNCLIDIfEmpty(Entity *entity)
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

QString QnstComposerPlugin::getNCLIdFromEntity(Entity *entity)
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

bool QnstComposerPlugin::isEntityHandled(Entity *entity)
{
  if(entity != NULL)
  {
    QString type = entity->getType();

    // \todo This could be a map or an array
    if(type == "body" || type == "context" || type == "media" ||
       type == "switch" || type == "port" || type == "link" || type == "bind" ||
       type == "area" || type == "property" || type == "causalConnector" ||
       type == "switchPort" || type == "mapping" || type == "importBase"||
       type == "bindParam" || type == "connectorParam"  )

      return true;
  }
  return false;
}

void QnstComposerPlugin::syncNCLIdsWithStructuralIds()
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
      view->removeEntity(structuralID);
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

void QnstComposerPlugin::textualStartSync(QString, void*)
{
  // qDebug() << "QnstComposerPlugin::textualStartSync";
  cacheNCLIds();
  dirtyEntities.clear();
  isSyncingFromTextual = true;
}

void QnstComposerPlugin::textualFinishSync(QString, void*)
{
  qDebug() << "QnstComposerPlugin::textualFinishSync";
  isSyncingFromTextual = false;
  syncNCLIdsWithStructuralIds();
}

void QnstComposerPlugin::clearValidationError(QString, void *param)
{
  view->clearValidationErrors();
}

void QnstComposerPlugin::validationError(QString pluginID, void *param)
{
  if(isSyncingFromTextual)
    return;

  if(param)
  {
    pair <QString, QString> *p = (pair <QString, QString> *)param;
    view->markError(p->first);
  }
}
