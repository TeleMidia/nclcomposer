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

        entites[list[0]] = list[1];

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
    foreach(QString key, entites.keys()){
        data.append(key+"="+entites[key]+"~");
    }

    data.append("$END_ENTITIES_LINES$");

    emit setPluginData(data);

    return true;
}

void QnstComposerPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
    if (pluginID != getPluginInstanceID()){
        requestEntityAddition(entity);
    }else{
        if (entity->getType() != "ncl" &&
            entity->getType() != "bind" &&
            entity->getType() != "simpleCondition" &&
            entity->getType() != "simpleAction"){

            entites[entity->getUniqueId()] = request;
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

        entites.remove(entityID);
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
        requestBodyAddition(entity);

        entites[entity->getUniqueId()] = entity->getUniqueId();

    // if the entity is of type CONTEXT
    }else if (entity->getType() == "context"){
        requestContextAddition(entity);

        entites[entity->getUniqueId()] = entity->getUniqueId();

    // if the entity is of type SWITCH
    }else if (entity->getType() == "switch"){
        requestSwitchAddition(entity);

        entites[entity->getUniqueId()] = entity->getUniqueId();

    // if the entity is of type MEDIA
    }else if (entity->getType() == "media"){
        requestMediaAddition(entity);

        entites[entity->getUniqueId()] = entity->getUniqueId();

    // if the entity is of type PORT
    }else if (entity->getType() == "port"){
        requestPortAddition(entity);

        entites[entity->getUniqueId()] = entity->getUniqueId();
    }
}

void QnstComposerPlugin::requestEntityRemotion(Entity* entity)
{
    if (entity->getType() == "body" ||
        entity->getType() == "context" ||
        entity->getType() == "media" ||
        entity->getType() == "switch" ||
        entity->getType() == "port"){

        view->removeEntity(entites[entity->getUniqueId()]);
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
    }
}

void QnstComposerPlugin::requestEntitySelection(Entity* entity)
{
    if (entites.contains(entity->getUniqueId())){
        if (entity->getType() == "body" ||
            entity->getType() == "context" ||
            entity->getType() == "media" ||
            entity->getType() == "switch" ||
            entity->getType() == "port"){

            view->selectEntity(entites[entity->getUniqueId()]);
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

    view->changeEntity(entites[entity->getUniqueId()], properties);
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

    view->addEntity(entity->getUniqueId(), entites[entity->getParentUniqueId()], properties);
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

    view->changeEntity(entites[entity->getUniqueId()], properties);
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

    view->addEntity(entity->getUniqueId(), entites[entity->getParentUniqueId()], properties);
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

    view->changeEntity(entites[entity->getUniqueId()], properties);
}

void QnstComposerPlugin::requestMediaAddition(Entity* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "media";

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    if (entity->getAttribute("src") != ""){
        properties["src"] = entity->getAttribute("src");
    }

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

    view->addEntity(entity->getUniqueId(), entites[entity->getParentUniqueId()], properties);
}

void QnstComposerPlugin::requestMediaChange(Entity* entity)
{
    QMap<QString, QString> properties;

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    if (entity->getAttribute("src") != ""){
        properties["src"] = entity->getAttribute("src");
    }

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

    view->changeEntity(entites[entity->getUniqueId()], properties);
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
    }

    if (entity->getAttribute("interface") != ""){
        properties["interface"] = entity->getAttribute("interface");
    }

    view->addEntity(entity->getUniqueId(), entites[entity->getParentUniqueId()], properties);
}

void QnstComposerPlugin::requestPortChange(Entity* entity)
{
    QMap<QString, QString> properties;

    if (entity->getAttribute("id") != ""){
        properties["id"] = entity->getAttribute("id");
    }

    if (entity->getAttribute("component") != ""){
        properties["component"] = entity->getAttribute("component");
    }

    if (entity->getAttribute("interface") != ""){
        properties["interface"] = entity->getAttribute("interface");
    }

    view->changeEntity(entites[entity->getUniqueId()], properties);
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
    }
}

void QnstComposerPlugin::requestEntityRemotion(const QString uid)
{
    if (entites.key(uid, "nil") != "nil"){
        emit removeEntity(getProject()->getEntityById(entites.key(uid)), false);

        entites.remove(entites.key(uid));
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
    if (entites.key(uid, "nil") != "nil"){
        emit sendBroadcastMessage("changeSelectedEntity", new QString(entites.key(uid)));
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
    Entity* entity = getProject()->getEntityById(entites.key(uid));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        request = uid;

        emit setAttributes(entity, attributes, false);
    }
}

void QnstComposerPlugin::requestContextAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entites.key(parent));

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
    }
}

void QnstComposerPlugin::requestContextChange(const QString uid, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entites.key(uid));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        if (properties["refer"] != ""){
            attributes["refer"] = properties["refer"];
        }

        request = uid;

        emit setAttributes(entity, attributes, false);
    }
}

void QnstComposerPlugin::requestSwitchAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entites.key(parent));

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
    }
}

void QnstComposerPlugin::requestSwitchChange(const QString uid, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entites.key(uid));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        if (properties["refer"] != ""){
            attributes["refer"] = properties["refer"];
        }

        request = uid;

        emit setAttributes(entity, attributes, false);
    }
}

void QnstComposerPlugin::requestMediaAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entites.key(parent));

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
    }
}

void QnstComposerPlugin::requestMediaChange(const QString uid, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entites.key(uid));

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

        emit setAttributes(entity, attributes, false);
    }
}

void QnstComposerPlugin::requestPortAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entites.key(parent));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        attributes["id"] = properties["id"];

        attributes["component"] = properties["component"];

        attributes["interface"] = properties["interface"];

        request = uid;

        emit addEntity("port", entity->getUniqueId(), attributes, false);
    }
}

void QnstComposerPlugin::requestPortChange(const QString uid, const QMap<QString, QString> properties)
{
    Entity* entity = getProject()->getEntityById(entites.key(uid));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        attributes["id"] = properties["id"];

        attributes["component"] = properties["component"];

        attributes["interface"] = properties["interface"];

        request = uid;

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

            // condition
            QMap<QString, QString> cattributes;

            cattributes["role"] = properties["condition"];

            emit addEntity("simpleCondition", entites.key(uid), cattributes, false);

            // action
            QMap<QString, QString> aattributes;

            aattributes["role"] = properties["action"].toLower();

            emit addEntity("simpleAction", entites.key(uid), aattributes, false);
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
    Entity* entity = getProject()->getEntityById(entites.key(parent));

    if (entity != NULL){
        QMap<QString, QString> attributes;

        attributes["id"] = properties["id"];
        attributes["xconnector"] = properties["connector"];

        request = uid;

        emit addEntity("link", entity->getUniqueId(), attributes, false);

        Entity* parent = getProject()->getEntityById(entites.key(uid));

        if (parent != NULL){
            // bind condition
            QMap<QString, QString> cattributes;

            cattributes["role"] = properties["condition"];
            cattributes["component"] = properties["componentcondition"];

            request = uid;

            emit addEntity("bind", parent->getUniqueId(), cattributes, false);

            // bind action

            QMap<QString, QString> aattributes;

            aattributes["role"] = properties["action"];
            aattributes["component"] = properties["componentaction"];

            request = uid;

            emit addEntity("bind", parent->getUniqueId(), aattributes, false);
        }
    }
}

} } } // end namespace
