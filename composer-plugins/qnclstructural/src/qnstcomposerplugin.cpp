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

}

QWidget* QnstComposerPlugin::getWidget()
{
    return view;
}

bool QnstComposerPlugin::saveSubsession()
{

}

void QnstComposerPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
    if (pluginID != getPluginInstanceID()){
        requestEntityAddition(entity);

        entites[entity->getUniqueId()] = entity->getUniqueId();

    }else{
        entites[entity->getUniqueId()] = request;
    }
}

void QnstComposerPlugin::errorMessage(QString error)
{

}

void QnstComposerPlugin::onEntityChanged(QString pluginID, Entity *entity)
{

}

void QnstComposerPlugin::onEntityRemoved(QString pluginID, QString entityID)
{

}

void QnstComposerPlugin::changeSelectedEntity(QString pluginID, void* param)
{

}

void QnstComposerPlugin::requestEntityAddition(Entity* entity)
{
    // if the entity is of type BODY
    if (entity->getType() == "body"){
        requestBodyAddition(entity);

    // if the entity is of type CONTEXT
    }else if (entity->getType() == "context"){
        requestContextAddition(entity);

    // if the entity is of type SWITCH
    }else if (entity->getType() == "switch"){


    // if the entity is of type MEDIA
    }else if (entity->getType() == "media"){
        requestMediaAddition(entity);

    }
}

void QnstComposerPlugin::requestEntityRemotion(Entity* entity)
{

}

void QnstComposerPlugin::requestEntityChange(Entity* entity)
{

}

void QnstComposerPlugin::requestEntitySelection(Entity* entity)
{

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
    // TODO
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
    // TODO
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
    // TODO
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

    // if the entity is of type SWITCH
    }else if (properties["TYPE"] == "media"){
        requestMediaAddition(uid, parent, properties);
    }
}

void QnstComposerPlugin::requestEntityRemotion(const QString uid)
{
    // TODO
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
    }
}

void QnstComposerPlugin::requestEntitySelection(const QString uid)
{
    // TODO
}

void QnstComposerPlugin::requestBodyAddition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    requestBodyDependence();

    Entity* entity = getProject()->getEntitiesbyType("ncl").first();

    if (entity != NULL){
        QMap<QString, QString> attributes;

        if (properties["id"] != ""){
            attributes["id"] = properties["id"];
        }

        request = uid;

        emit addEntity("body", entity->getUniqueId(), attributes, false);
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

void QnstComposerPlugin::requestBodyDependence()
{
    Entity* entity = getProject();

    if (entity != NULL){
        QMap<QString, QString> attributes;

        emit addEntity("ncl", getProject()->getUniqueId(), attributes, false);
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

} } } // end namespace
