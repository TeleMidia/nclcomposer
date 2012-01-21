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

namespace composer {
    namespace plugin {
        namespace layout {

QnstComposerPlugin::QnstComposerPlugin(QObject* parent)
{
    setParent(parent);

    createWidgets();
    createConnections();

    n = 0;

    request = "";
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
    connect(view, SIGNAL(entityAdded(QString,QString,QMap<QString,QString>)), SLOT(requestEntityAddition(QString,QString,QMap<QString,QString>)));
    connect(view, SIGNAL(entityRemoved(QString)), SLOT(requestEntityRemotion(QString)));
    connect(view, SIGNAL(entityChanged(QString,QMap<QString,QString>)), SLOT(requestEntityChange(QString,QMap<QString,QString>)));
    connect(view, SIGNAL(entitySelected(QString)), SLOT(requestEntitySelection(QString)));
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

    while(nextp > 0){
        QString entitypair = mid.mid(lastp, nextp - lastp);

        QStringList list = entitypair.split("=");

        entities[list[0]] = list[1];

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
//    // TODO: clean view data before update.

//    Entity* project = getProject();

//    if (project != NULL){
//        updateChildren(project);
//    }
}

void QnstComposerPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
    if (pluginID != getPluginInstanceID()){
        requestEntityAddition(entity);
    }else{
        if (request != ""){

            entities[entity->getUniqueId()] = request;
        }
    }
}

void QnstComposerPlugin::errorMessage(QString error)
{

}

void QnstComposerPlugin::onEntityChanged(QString pluginID, Entity *entity)
{
    if (pluginID != getPluginInstanceID()){
        requestEntityChange(entity);
    }
}

void QnstComposerPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
    if (pluginID != getPluginInstanceID()){
        requestEntityRemotion(getProject()->getEntityById(entityID));

        entities.remove(entityID);
    }
}

void QnstComposerPlugin::changeSelectedEntity(QString pluginID, void* param)
{
    QString* entityUID = (QString*) param;

    if(entityUID != NULL){
        requestEntitySelection(getProject()->getEntityById(*entityUID));
    }
}

void QnstComposerPlugin::requestEntityAddition(Entity* entity)
{
    // if the entity is of type BODY
    if (entity->getType() == "body"){
        entities[entity->getUniqueId()] = entity->getUniqueId();

        requestBodyAddition(entity);

    // if the entity is of type CONTEXT
    }else if (entity->getType() == "context"){
        entities[entity->getUniqueId()] = entity->getUniqueId();

        requestContextAddition(entity);

    // if the entity is of type SWITCH
    }else if (entity->getType() == "switch"){
        entities[entity->getUniqueId()] = entity->getUniqueId();

        requestSwitchAddition(entity);

    // if the entity is of type MEDIA
    }else if (entity->getType() == "media"){
        entities[entity->getUniqueId()] = entity->getUniqueId();

        requestMediaAddition(entity);

    // if the entity is of type PORT
    }else if (entity->getType() == "port"){
        entities[entity->getUniqueId()] = entity->getUniqueId();

        requestPortAddition(entity);

    // if the entity is of type LINK
    }else if (entity->getType() == "link"){
        entities[entity->getUniqueId()] = entity->getUniqueId();

        requestLinkAddition(entity);

    // if the entity is of type BIND
    }else if (entity->getType() == "bind"){
        entities[entity->getUniqueId()] = entity->getUniqueId();

        requestBindAddition(entity);

    // if the entity is of type CAUSALCONNECTOR
    }else if (entity->getType() == "causalConnector"){
        entities[entity->getUniqueId()] = entity->getUniqueId();

        requestCausalConnectorAddition(entity);

    // if the entity is of type simpleCondition
    }else if (entity->getType() == "simpleCondition"){
        entities[entity->getUniqueId()] = entity->getUniqueId();

        requestSimpleConditionAddition(entity);

    // if the entity is of type simpleAction
    }else if (entity->getType() == "simpleAction"){
        entities[entity->getUniqueId()] = entity->getUniqueId();

        requestSimpleActionAddition(entity);

    // if the entity is of type importBase
    }else if (entity->getType() == "importBase"){
        requestImportBaseAddition(entity);
    }
}

void QnstComposerPlugin::requestEntityRemotion(Entity* entity)
{

    // still missing connector, simpleCondition and simpleAction
    if (entity->getType() == "body" ||
        entity->getType() == "context" ||
        entity->getType() == "media" ||
        entity->getType() == "switch" ||
        entity->getType() == "port" ||
        entity->getType() == "link" ||
        entity->getType() == "bind" ||
        entity->getType() == "causalConnector"){

        view->removeEntity(entities[entity->getUniqueId()]);
    }
}

void QnstComposerPlugin::requestEntityChange(Entity* entity)
{
    // if the entity is of type BODY
    if (entity->getType() == "body"){
        requestBodyChange(entity);

    // if the entity is of type CONTEXT
    }else if (entity->getType() == "context"){
        requestContextChange(entity);

    // if the entity is of type SWITCH
    }else if (entity->getType() == "switch"){
        requestSwitchChange(entity);

    // if the entity is of type MEDIA
    }else if (entity->getType() == "media"){
        requestMediaChange(entity);

    // if the entity is of type PORT
    }else if (entity->getType() == "port"){
        requestPortChange(entity);

    // if the entity is of type LINK
    }else if (entity->getType() == "link"){
        requestLinkChange(entity);

    // if the entity is of type Bind
    }else if (entity->getType() == "bind"){
        requestBindChange(entity);

    // if the entity is of type CausalConnector
    }else if (entity->getType() == "causalConnector"){
        requestCausalConnectorChange(entity);

    // if the entity is of type SimpleCondition
    }else if (entity->getType() == "simpleCondition"){
        requestSimpleConditionChange(entity);

    // if the entity is of type SimpleAction
    }else if (entity->getType() == "simpleAction"){
        requestSimpleActionChange(entity);

    // if the entity is of type ImportBase
    }else if (entity->getType() == "importBase"){
        requestImportBaseChange(entity);
    }
}

void QnstComposerPlugin::requestEntitySelection(Entity* entity)
{
    if (entities.contains(entity->getUniqueId())){
        if (entity->getType() == "body" ||
            entity->getType() == "context" ||
            entity->getType() == "media" ||
            entity->getType() == "switch" ||
            entity->getType() == "port" ||
            entity->getType() == "link"){

            view->selectEntity(entities[entity->getUniqueId()]);
        }
    }
}

void QnstComposerPlugin::requestBodyAddition(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "body";

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    view->addEntity(entity->getUniqueId(), "", properties);
}

void QnstComposerPlugin::requestBodyChange(Entity* entity)
{
    QMap<QString, QString> properties;

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestContextAddition(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "context";

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    if (entity->getAttribute("refer") != ""){
        properties["refer"] = entity->getAttribute("refer");
    }

    view->addEntity(entity->getUniqueId(), entities[entity->getParentUniqueId()], properties);
}

void QnstComposerPlugin::requestContextChange(Entity* entity)
{
    QMap<QString, QString> properties;

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    if (entity->getAttribute("refer") != ""){
        properties["refer"] = entity->getAttribute("refer");
    }

    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestSwitchAddition(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "switch";

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    if (entity->getAttribute("refer") != ""){
        properties["refer"] = entity->getAttribute("refer");
    }

    view->addEntity(entity->getUniqueId(), entities[entity->getParentUniqueId()], properties);
}

void QnstComposerPlugin::requestSwitchChange(Entity* entity)
{
    QMap<QString, QString> properties;

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    if (entity->getAttribute("refer") != ""){
        properties["refer"] = entity->getAttribute("refer");
    }

    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestMediaAddition(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "media";

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

//    if (entity->getAttribute("src") != ""){
        properties["src"] = entity->getAttribute("src");
//    }

    if (entity->getAttribute("refer") != ""){
        properties["refer"] = entity->getAttribute("refer");
    }

    if (entity->getAttribute("instance") != ""){
        properties["instance"] = entity->getAttribute("instance");
    }

    if (entity->getAttribute("type") != ""){
        properties["type"] = entity->getAttribute("type");
    }

    if (entity->getAttribute("descriptor") != ""){
        properties["descriptor"] = entity->getAttribute("descriptor");
    }

    view->addEntity(entity->getUniqueId(), entities[entity->getParentUniqueId()], properties);

    QList<Entity*> list;

    list = getProject()->getEntitiesbyType("port");

    foreach(Entity* e, list){
        if (e->getAttribute("component") != ""){
            if (e->getAttribute("component") == properties["id"]){
                requestPortChange(e);
            }
        }
    }
}

void QnstComposerPlugin::requestMediaChange(Entity* entity)
{
    QMap<QString, QString> properties;

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

//    if (entity->getAttribute("src") != ""){
        properties["src"] = entity->getAttribute("src");
//    }

    if (entity->getAttribute("refer") != ""){
        properties["refer"] = entity->getAttribute("refer");
    }

    if (entity->getAttribute("instance") != ""){
        properties["instance"] = entity->getAttribute("instance");
    }

    if (entity->getAttribute("type") != ""){
        properties["type"] = entity->getAttribute("type");
    }

    if (entity->getAttribute("descriptor") != ""){
        properties["descriptor"] = entity->getAttribute("descriptor");
    }

    view->changeEntity(entities[entity->getUniqueId()], properties);

    QList<Entity*> list;

    list = getProject()->getEntitiesbyType("port");

    foreach(Entity* e, list){
        if (e->getAttribute("component") != ""){
            if (e->getAttribute("component") == properties["id"]){
                requestPortChange(e);
            }
        }
    }
}

void QnstComposerPlugin::requestPortAddition(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "port";

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    if (entity->getAttribute("component") != ""){
        properties["component"] = entity->getAttribute("component");
        properties["componentUid"] = entities[getUidById(properties["component"])];
    }

    if (entity->getAttribute("interface") != ""){
        properties["interface"] = entity->getAttribute("interface");
        properties["interfaceUid"] = entities[getUidById(properties["interface"])];
    }

    view->addEntity(entity->getUniqueId(), entities[entity->getParentUniqueId()], properties);
}

void QnstComposerPlugin::requestPortChange(Entity* entity)
{
    QMap<QString, QString> properties;

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    if (entity->getAttribute("component") != ""){
        properties["component"] = entity->getAttribute("component");
        properties["componentUid"] = entities[getUidById(properties["component"])];
    }

    if (entity->getAttribute("interface") != ""){
        properties["interface"] = entity->getAttribute("interface");
        properties["interfaceUid"] = entities[getUidById(properties["interface"])];
    }

    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestLinkAddition(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "link";

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    if (entity->getAttribute("xconnector") != ""){
        properties["xconnector"] = entity->getAttribute("xconnector");
        properties["xconnectorUID"] = entities[getUidById(properties["xconnector"])];
    }

    view->addEntity(entity->getUniqueId(), entities[entity->getParentUniqueId()], properties);
}

void QnstComposerPlugin::requestLinkChange(Entity* entity)
{
    QMap<QString, QString> properties;

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    if (entity->getAttribute("xconnector") != ""){
        properties["xconnector"] = entity->getAttribute("xconnector");
        properties["xconnectorUID"] = entities[getUidById(properties["xconnector"])];
    }

    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestBindAddition(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "bind";

    if (entity->getAttribute("role") != ""){
        properties["role"] = entity->getAttribute("role");
    }

    if (entity->getAttribute("component") != ""){
        properties["component"] = entity->getAttribute("component");
        properties["componentUid"] = entities[getUidById(properties["component"])];
    }

    if (entity->getAttribute("interface") != ""){
        properties["interface"] = entity->getAttribute("interface");
        properties["interfaceUid"] = entities[getUidById(properties["interface"])];
    }

    view->addEntity(entity->getUniqueId(), entities[entity->getParentUniqueId()], properties);
}

void QnstComposerPlugin::requestBindChange(Entity* entity)
{
    QMap<QString, QString> properties;

    if (entity->getAttribute("role") != ""){
        properties["role"] = entity->getAttribute("role");
    }

    if (entity->getAttribute("component") != ""){
        properties["component"] = entity->getAttribute("component");
        properties["componentUid"] = entities[getUidById(properties["component"])];
    }

    if (entity->getAttribute("interface") != ""){
        properties["interface"] = entity->getAttribute("interface");
        properties["interfaceUid"] = entities[getUidById(properties["interface"])];
    }

    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestImportBaseAddition(Entity* entity)
{
    if (entity != NULL){
        Entity* parent = entity->getParent();

        if (parent != NULL){
            if (parent->getType() == "connectorBase"){
                QMap<QString, QString> properties;

                properties["TYPE"] = "importBase";

                if (entity->getAttribute("documentURI") != ""){
                    properties["projectURI"] = getProject()->getLocation();
                    properties["documentURI"] = entity->getAttribute("documentURI");
                }

                if (entity->getAttribute("alias") != ""){
                    properties["alias"] = entity->getAttribute("alias");
                }

                view->addEntity(entity->getUniqueId(), "",properties);
            }
        }
    }
}

void QnstComposerPlugin::requestImportBaseChange(Entity* entity)
{
    if (entity != NULL){
        Entity* parent = entity->getParent();

        if (parent != NULL){
            if (parent->getType() == "connectorBase"){
                QMap<QString, QString> properties;

                properties["TYPE"] = "importBase";

                if (entity->getAttribute("documentURI") != ""){
                    properties["projectURI"] = getProject()->getLocation();
                    properties["documentURI"] = entity->getAttribute("documentURI");
                }

                if (entity->getAttribute("alias") != ""){
                    properties["alias"] = entity->getAttribute("alias");
                }

                view->changeEntity(entity->getUniqueId(),properties);
            }
        }
    }
}

void QnstComposerPlugin::requestCausalConnectorAddition(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "causalConnector";

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    view->addEntity(entity->getUniqueId(), entities[entity->getParentUniqueId()], properties);
}

void QnstComposerPlugin::requestCausalConnectorChange(Entity* entity)
{
    QMap<QString, QString> properties;

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestSimpleConditionAddition(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "simpleCondition";

    Entity* conn = entity->getParent();

    while (conn->getType() != "causalConnector" && conn != getProject()){
        conn = conn->getParent();
    }

    properties["connector"] = entities[conn->getUniqueId()];

    if (entity->getAttribute("role") != ""){
        properties["role"] = entity->getAttribute("role");
    }

    view->addEntity(entity->getUniqueId(), entities[entity->getParentUniqueId()], properties);
}

void QnstComposerPlugin::requestSimpleConditionChange(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "simpleCondition";

    Entity* conn = entity->getParent();

    while (conn->getType() != "causalConnector" && conn != getProject()){
        conn = conn->getParent();
    }

    properties["connector"] = entities[conn->getUniqueId()];

    if (entity->getAttribute("role") != ""){
        properties["role"] = entity->getAttribute("role");
    }

    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestSimpleActionAddition(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "simpleAction";

    Entity* conn = entity->getParent();

    while (conn->getType() != "causalConnector" && conn != getProject()){
        conn = conn->getParent();
    }

    properties["connector"] = entities[conn->getUniqueId()];

    if (entity->getAttribute("role") != ""){
        properties["role"] = entity->getAttribute("role");
    }

    view->addEntity(entity->getUniqueId(), entities[entity->getParentUniqueId()], properties);
}

void QnstComposerPlugin::requestSimpleActionChange(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "simpleAction";

    Entity* conn = entity->getParent();

    while (conn->getType() != "causalConnector" && conn != getProject()){
        conn = conn->getParent();
    }

    properties["connector"] = entities[conn->getUniqueId()];

    if (entity->getAttribute("role") != ""){
        properties["role"] = entity->getAttribute("role");
    }

    view->changeEntity(entities[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestEntityAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    // if the entity is of type BODY
    if (properties["TYPE"] == "body"){
        requestBodyAddition(uid, parent, properties);

    // if the entity is of type CONTEXT
    }else if (properties["TYPE"] == "context"){
        requestContextAddition(uid, parent, properties);

    // if the entity is of type SWITCH
    }else if (properties["TYPE"] == "switch"){
        requestSwitchAddition(uid, parent, properties);

    // if the entity is of type MEDIA
    }else if (properties["TYPE"] == "media"){
        requestMediaAddition(uid, parent, properties);

    // if the entity is of type PORT
    }else if (properties["TYPE"] == "port"){
        requestPortAddition(uid, parent, properties);

    // if the entity is of type CONNECTOR
    }else if (properties["TYPE"] == "connector"){
        requestConnectorAddition(uid, parent, properties);

    // if the entity is of type LINK
    }else if (properties["TYPE"] == "link"){
        requestLinkAddition(uid, parent, properties);

    // if the entity is of type COMPLEXCONNECTOR
    }else if (properties["TYPE"] == "complex-connector"){
        requestComplexConnectorAddition(uid, parent, properties);

    // if the entity is of type BIND
    }else if (properties["TYPE"] == "bind"){
        requestBindAddition(uid, parent, properties);
    }
}

void QnstComposerPlugin::requestEntityRemotion(const QString uid)
{
    if (entities.key(uid, "nil") != "nil"){
        emit removeEntity(getProject()->getEntityById(entities.key(uid)), false);

        entities.remove(entities.key(uid));
    }
}

void QnstComposerPlugin::requestEntityChange(const QString uid, const QMap<QString, QString> properties)
{
    // if the entity is of type BODY
    if (properties["TYPE"] == "body"){
        requestBodyChange(uid, properties);

    // if the entity is of type CONTEXT
    }else if (properties["TYPE"] == "context"){
        requestContextChange(uid, properties);

    // if the entity is of type SWITCH
    }else if (properties["TYPE"] == "switch"){
        requestSwitchChange(uid, properties);

    // if the entity is of type MEDIA
    }else if (properties["TYPE"] == "media"){
        requestMediaChange(uid, properties);

    // if the entity is of type PORT
    }else if (properties["TYPE"] == "port"){
        requestPortChange(uid, properties);
    }
}

void QnstComposerPlugin::requestEntitySelection(const QString uid)
{
    if (entities.key(uid, "nil") != "nil"){
        emit sendBroadcastMessage("changeSelectedEntity", new QString(entities.key(uid)));
    }
}

void QnstComposerPlugin::requestBodyAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    requestBodyDependence();

    QList<Entity*> list = getProject()->getEntitiesbyType("ncl");

    if (!list.isEmpty()){
        Entity* entity = list.first();

        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        request = uid;

        emit addEntity("body", entity->getUniqueId(), attributes, false);

        request = "";
    }
}

void QnstComposerPlugin::requestBodyDependence()
{
    QList<Entity*> list = getProject()->getEntitiesbyType("ncl");

    if (list.isEmpty()){
        Entity* project = getProject();

        if (project != NULL){
            QMap<QString, QString> attributes;

            emit addEntity("ncl", project->getUniqueId(), attributes, false);
        }
    }
}

void QnstComposerPlugin::requestBodyChange(const QString uid, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entities.key(uid));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        emit setAttributes(entity, attributes, false);
    }
}

void QnstComposerPlugin::requestContextAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entities.key(parent));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        if (properties["refer"] != ""){
            attributes["refer"] = properties["refer"];
        }

        request = uid;

        emit addEntity("context", entity->getUniqueId(), attributes, false);

        request = "";
    }
}

void QnstComposerPlugin::requestContextChange(const QString uid, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entities.key(uid));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        if (properties["refer"] != ""){
            attributes["refer"] = properties["refer"];
        }

        emit setAttributes(entity, attributes, false);
    }
}

void QnstComposerPlugin::requestSwitchAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entities.key(parent));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        if (properties["refer"] != ""){
            attributes["refer"] = properties["refer"];
        }

        request = uid;

        emit addEntity("switch", entity->getUniqueId(), attributes, false);

        request = "";
    }
}

void QnstComposerPlugin::requestSwitchChange(const QString uid, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entities.key(uid));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        if (properties["refer"] != ""){
            attributes["refer"] = properties["refer"];
        }

        emit setAttributes(entity, attributes, false);
    }
}

void QnstComposerPlugin::requestMediaAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entities.key(parent));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        if (properties["src"] != ""){
            attributes["src"] = properties["src"];
        }

        if (properties["refer"] != ""){
            attributes["refer"] = properties["refer"];
        }

        if (properties["instance"] != ""){
            attributes["instance"] = properties["instance"];
        }

        if (properties["type"] != ""){
            attributes["type"] = properties["type"];
        }

        if (properties["descriptor"] != ""){
            attributes["descriptor"] = properties["descriptor"];
        }

        request = uid;

        emit addEntity("media", entity->getUniqueId(), attributes, false);

        request = "";
    }
}

void QnstComposerPlugin::requestMediaChange(const QString uid, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entities.key(uid));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        if (properties["src"] != ""){
            attributes["src"] = properties["src"];
        }

        if (properties["refer"] != ""){
            attributes["refer"] = properties["refer"];
        }

        if (properties["instance"] != ""){
            attributes["instance"] = properties["instance"];
        }

        if (properties["type"] != ""){
            attributes["type"] = properties["type"];
        }

        if (properties["descriptor"] != ""){
            attributes["descriptor"] = properties["descriptor"];
        }

        emit setAttributes(entity, attributes, false);
    }
}

void QnstComposerPlugin::requestPortAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entities.key(parent));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        attributes["id"] = properties["id"];

        attributes["component"] = properties["component"];

        attributes["interface"] = properties["interface"];

        request = uid;

        emit addEntity("port", entity->getUniqueId(), attributes, false);

        request = "";
    }
}

void QnstComposerPlugin::requestPortChange(const QString uid, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entities.key(uid));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        attributes["id"] = properties["id"];

        attributes["component"] = properties["component"];

        attributes["interface"] = properties["interface"];

        emit setAttributes(entity, attributes, false);
    }
}

void QnstComposerPlugin::requestConnectorAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    requestConnectorDependence();

    QList<Entity*> list = getProject()->getEntitiesbyType("connectorBase");

    if (!list.isEmpty()){
        Entity* entity = list.first();

        if (entity != NULL){
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

void QnstComposerPlugin::requestComplexConnectorAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    requestConnectorDependence();

    QList<Entity*> list = getProject()->getEntitiesbyType("connectorBase");

    if (!list.isEmpty()){
        Entity* entity = list.first();

        if (entity != NULL){
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

            foreach(Entity* child, getProject()->getEntityById(entities.key(uid))->getChildren()){

                if (child->getType() == "compoundCondition"){
                    cpcUID = child->getUniqueId();
                }

                if (child->getType() == "compoundAction"){
                    cpaUID = child->getUniqueId();
                }
            }

            // condition
            if (properties["condition"] != "" && cpcUID != ""){
                QMap<QString, QString> cattributes;

                cattributes["role"] = properties["condition"];
                cattributes["max"] = "unbounded";

                emit addEntity("simpleCondition", cpcUID, cattributes, false);
            }

            // condition
            if (properties["action"] != ""  && cpaUID != ""){
                // action
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

    if (list.isEmpty()){
        Entity* parent;

        if (!getProject()->getEntitiesbyType("head").isEmpty()){
            parent = getProject()->getEntitiesbyType("head").first();
        }

        if (parent != NULL){
            QMap<QString, QString> attributes;

            emit addEntity("connectorBase", parent->getUniqueId(), attributes, false);
        }
    }
}

void QnstComposerPlugin::requestConnectorBaseDependence()
{
    QList<Entity*> list = getProject()->getEntitiesbyType("head");

    if (list.isEmpty()){
        Entity* parent;

        if (!getProject()->getEntitiesbyType("ncl").isEmpty()){
            parent = getProject()->getEntitiesbyType("ncl").first();
        }

        if (parent != NULL){
            QMap<QString, QString> attributes;

            emit addEntity("head", parent->getUniqueId(), attributes, false);
        }
    }
}

void QnstComposerPlugin::requestLinkAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entities.key(parent));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        attributes["id"] = properties["id"];
        attributes["xconnector"] = properties["connector"];

        request = uid;

        emit addEntity("link", entity->getUniqueId(), attributes, false);

        request = "";

        Entity* parent = getProject()->getEntityById(entities.key(uid));

        if (parent != NULL){
            // bind condition
            QMap<QString, QString> cattributes;

            cattributes["role"] = properties["condition"];
            cattributes["component"] = properties["componentcondition"];

            request = uid;

            emit addEntity("bind", parent->getUniqueId(), cattributes, false);

            request = "";

            // bind action

            QMap<QString, QString> aattributes;

            aattributes["role"] = properties["action"];
            aattributes["component"] = properties["componentaction"];

            request = uid;

            emit addEntity("bind", parent->getUniqueId(), aattributes, false);

            request = "";
        }
    }
}

void QnstComposerPlugin::requestBindAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    // checking connector
    QList<Entity*> connectors = getProject()->getEntitiesbyType("causalConnector");

    QString connUID = "";

    foreach(Entity* conn, connectors){
        if (conn->getAttribute("id") == properties["connector"]){
            connUID = conn->getUniqueId();

            break;
        }
    }

    QString cpcUID = "";
    QString cpaUID = "";

    if (connUID != ""){
        foreach(Entity* child, getProject()->getEntityById(connUID)->getChildren()){

            if (child->getType() == "compoundCondition"){
                cpcUID = child->getUniqueId();
            }

            if (child->getType() == "compoundAction"){
                cpaUID = child->getUniqueId();
            }
        }
    }

    if (cpcUID != "" && cpaUID != ""){
        if (properties["condition"] != ""){
            bool hasCond = false;

            foreach(Entity* child, getProject()->getEntityById(cpcUID)->getChildren()){
                if (child->getAttribute("role") == properties["condition"]){
                    hasCond = true;
                    break;
                }
            }

            if (!hasCond){
                QMap<QString, QString> cattributes;

                cattributes["role"] = properties["condition"];

                emit addEntity("simpleCondition", cpcUID, cattributes, false);
            }
        }

        if (properties["action"] != ""){
            bool hasAct = false;

            foreach(Entity* child, getProject()->getEntityById(cpaUID)->getChildren()){
                if (child->getAttribute("role") == properties["action"].toLower()){
                    hasAct = true;
                    break;
                }
            }

            if (!hasAct){
                QMap<QString, QString> aattributes;

                aattributes["role"] = properties["action"].toLower();

                emit addEntity("simpleAction", cpaUID, aattributes, false);
            }
        }
    }

    //checking link
    QList<Entity*> links = getProject()->getEntitiesbyType("link");

    QString liknUID = "";

    foreach(Entity* link, links){
        if (link->getAttribute("id") == properties["link"]){
            liknUID = link->getUniqueId();

            break;
        }
    }

    if (liknUID == ""){
        QMap<QString, QString> linkattributes;

        linkattributes["id"] = properties["link"];
        linkattributes["xconnector"] = properties["connector"];

        if (properties["linkUID"] != ""){
            request = properties["linkUID"];
        }else{
            request = QUuid::createUuid().toString();
        }

        emit addEntity("link", getProject()->getEntityById(entities.key(parent))->getUniqueId(), linkattributes, false);

        request = "";

        liknUID = getUidById(linkattributes["id"]);
    }

    // add bind
    if (properties["condition"] != ""){
        // bind condition
        QMap<QString, QString> cattributes;

        cattributes["role"] = properties["condition"];
        cattributes["component"] = properties["component"];

        if (properties["interface"] != ""){
            cattributes["interface"] = properties["interface"];
        }

        request = uid;

        emit addEntity("bind", liknUID, cattributes, false);

        request = "";

    }else if (properties["action"] != ""){
        // bind action
        QMap<QString, QString> aattributes;

        aattributes["role"] = properties["action"].toLower();
        aattributes["component"] = properties["component"];

        if (properties["interface"] != ""){
            aattributes["interface"] = properties["interface"];
        }

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

    if (entity->getAttribute("id") == id){
        return entity->getUniqueId();
    }

    foreach(Entity* child, entity->getChildren()){
        QString result = getUidById(id, child);

        if (result != ""){
            uid = result; break;
        }
    }

    return uid;
}

} } } // end namespace
