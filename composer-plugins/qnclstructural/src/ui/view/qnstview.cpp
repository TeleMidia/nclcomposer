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
#include "qnstview.h"

QnstView::QnstView(QWidget* parent)
    : QGraphicsView(parent)
{
    createObjects();
    createConnection();

    n = 0;

    selected = NULL;
}

QnstView::~QnstView()
{

}

QVector<QnstEntity*> QnstView::getRoots() const
{
    return roots;
}

void QnstView::addRoot(QnstEntity* root)
{
    if (root != NULL){
        roots.append(root);
    }
}

void QnstView::removeRoot(QnstEntity* root)
{
    if (root != NULL){
        int index = roots.indexOf(root);

        if (index >= 0){
            roots.remove(index);
        }
    }
}

void QnstView::createObjects()
{
    scene = new QnstScene();
    scene->setParent(this);
    scene->setSceneRect(0, 0, 4000, 4000);

    setScene(scene);
}

void QnstView::createConnection()
{
    connect(scene, SIGNAL(entityAdded(QnstEntity*)), SLOT(requestEntityAddition(QnstEntity*)));
    connect(scene, SIGNAL(entityChanged(QnstEntity*)), SLOT(requestEntityChange(QnstEntity*)));
    connect(scene, SIGNAL(entityRemoved(QnstEntity*)), SLOT(requestEntityRemotion(QnstEntity*)));
    connect(scene, SIGNAL(entitySelected(QnstEntity*)), SLOT(requestEntitySelection(QnstEntity*)));
}

void QnstView::addEntity(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    qDebug() << "[QNST]" << "[" << ++n << "]" << ":" << "Adding entity '"+uid+"'";

    if (!entities.contains(uid)){
        // if the entity is of type BODY
        if (properties["TYPE"] == "body"){
            addBody(uid, parent, properties);

        // if the entity is of type CONTEXT
        }else if (properties["TYPE"] == "context"){
            addContext(uid, parent, properties);

        // if the entity is of type SWITCH
        }else if (properties["TYPE"] == "switch"){
            addSwitch(uid, parent, properties);

        // if the entity is of type MEDIA
        }else if (properties["TYPE"] == "media"){
            addMedia(uid, parent, properties);

        // if the entity is of type PORT
        }else if (properties["TYPE"] == "port"){
            addPort(uid, parent, properties);
        }
    }
}

void QnstView::removeEntity(const QString uid)
{
    qDebug() << "[QNST]" << "[" << ++n << "]" << ":" << "Removing entity '"+uid+"'";

    if (entities.contains(uid)){
        QnstEntity* entity = entities[uid];

        QnstGraphicsComposition* parent =  (QnstGraphicsComposition*) entity->getnstParent();

        if (parent != NULL){
            // if the entity is of type COMPOSITION
            if (entity->getnstType() == Qnst::Body ||
                entity->getnstType() == Qnst::Context ||
                entity->getnstType() == Qnst::Switch){

                deepRemotion2((QnstGraphicsComposition*) entity);

                parent->removencgGraphicsEntity((QnstGraphicsComposition*) entity);

            // if the entity is of type MEDIA
            }else  if (entity->getnstType() == Qnst::Image ||
                   entity->getnstType() == Qnst::Audio ||
                   entity->getnstType() == Qnst::Text ||
                   entity->getnstType() == Qnst::Video ||
                   entity->getnstType() == Qnst::Script ||
                   entity->getnstType() == Qnst::Settings ||
                   entity->getnstType() == Qnst::Media){

                parent->removencgGraphicsEntity((QnstGraphicsMedia*) entity);

            // if the entity is of type INTERFACE
            }else if (entity->getnstType() == Qnst::Port){
                parent->removencgGraphicsEntity((QnstGraphicsInterface*) entity);
            }
        }else{
            deepRemotion2((QnstGraphicsComposition*) entity);

            scene->removeItem((QnstGraphicsComposition*) entity);
        }

        entities.remove(uid);
    }
}

void QnstView::changeEntity(const QString uid, const QMap<QString, QString> properties)
{
    qDebug() << "[QNST]" << "[" << ++n << "]" << ":" << "Changing entity '"+uid+"'";

    // TODO:
}

void QnstView::selectEntity(const QString uid)
{
    qDebug() << "[QNST]" << "[" << ++n << "]" << ":" << "Selecting entity '"+uid+"'";

    if (entities.contains(uid)){
        if (selected != NULL){
            switch(selected->getnstType()){
            case Qnst::Image:

            case Qnst::Audio:

            case Qnst::Video:

            case Qnst::Text:

            case Qnst::Script:

            case Qnst::Settings:

            case Qnst::Media:
                ((QnstGraphicsContent*) selected)->setSelected(false);
                ((QnstGraphicsContent*) selected)->adjust();

                break;

            case Qnst::Body:

            case Qnst::Switch:

            case Qnst::Context:
                ((QnstGraphicsComposition*) selected)->setSelected(false);
                ((QnstGraphicsComposition*) selected)->adjust();

                break;

            case Qnst::Port:
                ((QnstGraphicsInterface*) selected)->setSelected(false);
                ((QnstGraphicsInterface*) selected)->adjust();

                break;
            }
        }

        QnstEntity* entity = entities[uid];

        switch(entity->getnstType()){
        case Qnst::Image:

        case Qnst::Audio:

        case Qnst::Video:

        case Qnst::Text:

        case Qnst::Script:

        case Qnst::Settings:

        case Qnst::Media:
            ((QnstGraphicsContent*) entity)->setSelected(true);
            ((QnstGraphicsContent*) entity)->setFocus(Qt::MouseFocusReason);
            ((QnstGraphicsContent*) entity)->adjust();

            selected = entity;

            break;

        case Qnst::Body:

        case Qnst::Switch:

        case Qnst::Context:
            ((QnstGraphicsComposition*) entity)->setSelected(true);
            ((QnstGraphicsComposition*) entity)->setFocus(Qt::MouseFocusReason);
            ((QnstGraphicsComposition*) entity)->adjust();

            break;

        case Qnst::Port:
            ((QnstGraphicsInterface*) entity)->setSelected(true);
            ((QnstGraphicsInterface*) entity)->setFocus(Qt::MouseFocusReason);
            ((QnstGraphicsInterface*) entity)->adjust();

            break;
        }

        selected = entity;
    }
}

void QnstView::addBody(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    QnstGraphicsBody* entity = new QnstGraphicsBody();
    entity->setnstUid(uid);

    entity->setTop(scene->height()/2 - 500/2);
    entity->setLeft(scene->width()/2 - 750/2);
    entity->setWidth(750);
    entity->setHeight(500);

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), scene, SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), scene, SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), scene, SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), scene, SIGNAL(entitySelected(QnstEntity*)));

    scene->addItem(entity);

    centerOn(entity);

    entities[uid] = entity;
}

void QnstView::changeBody(const QString uid, const QMap<QString, QString> properties)
{
    // TODO
}

void QnstView::addContext(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    QnstGraphicsComposition* composition = (QnstGraphicsComposition*) entities[parent];

    QnstGraphicsContext* entity = new QnstGraphicsContext(composition);
    entity->setnstUid(uid);

    entity->setTop(composition->getHeight()/2 - 200/2);
    entity->setLeft(composition->getWidth()/2 - 250/2);
    entity->setWidth(250);
    entity->setHeight(200);

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), composition, SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), composition, SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), composition, SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), composition, SIGNAL(entitySelected(QnstEntity*)));

    composition->addncgGraphicsEntity(entity);

    composition->clock(50);
    composition->fit(50);

    composition->attract();
    composition->adjust();

    entities[uid] = entity;
}

void QnstView::changeContext(const QString uid, const QMap<QString, QString> properties)
{
    // TODO
}

void QnstView::addSwitch(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    QnstGraphicsComposition* composition = (QnstGraphicsComposition*) entities[parent];

    QnstGraphicsSwitch* entity = new QnstGraphicsSwitch(composition);
    entity->setnstUid(uid);

    entity->setTop(composition->getHeight()/2 - 200/2);
    entity->setLeft(composition->getWidth()/2 - 250/2);
    entity->setWidth(250);
    entity->setHeight(200);

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), composition, SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), composition, SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), composition, SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), composition, SIGNAL(entitySelected(QnstEntity*)));

    composition->addncgGraphicsEntity(entity);

    composition->clock(50);
    composition->fit(50);

    composition->attract();
    composition->adjust();

    entities[uid] = entity;
}

void QnstView::changeSwitch(const QString uid, const QMap<QString, QString> properties)
{
    // TODO
}

void QnstView::addMedia(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    QnstGraphicsComposition* composition = (QnstGraphicsComposition*) entities[parent];

    QnstGraphicsMedia* entity = new QnstGraphicsMedia(composition);
    entity->setnstUid(uid);

    entity->setTop(composition->getHeight()/2 - 56/2);
    entity->setLeft(composition->getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(56);

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), composition, SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), composition, SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), composition, SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), composition, SIGNAL(entitySelected(QnstEntity*)));

    composition->addncgGraphicsEntity(entity);

    composition->clock(50);
    composition->fit(50);

    composition->attract();
    composition->adjust();

    entities[uid] = entity;
}

void QnstView::changeMedia(const QString uid, const QMap<QString, QString> properties)
{
    // TODO
}

void QnstView::addPort(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    QnstGraphicsComposition* composition = (QnstGraphicsComposition*) entities[parent];

    srand(time(NULL));

    int w = rand() % ((int) composition->getWidth());
    int h = rand() % ((int) composition->getHeight());

    QnstGraphicsPort* entity = new QnstGraphicsPort(composition);
    entity->setnstUid(uid);

    entity->setTop(h);
    entity->setLeft(w);
    entity->setWidth(32);
    entity->setHeight(32);

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), composition, SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), composition, SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), composition, SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), composition, SIGNAL(entitySelected(QnstEntity*)));

    composition->addncgGraphicsEntity(entity);

    entity->adjust();

    entities[uid] = entity;
}

void QnstView::changePort(const QString uid, const QMap<QString, QString> properties)
{
    // TODO
}

void QnstView::requestEntityAddition(QnstEntity* entity)
{
    qDebug() << "[QNST]" << "[" << ++n << "]" << ":" << "Requesting entity addition '"+entity->getnstUid()+"'";

    entities[entity->getnstUid()] = entity;

    // if the entity is of type BODY
    if (entity->getnstType() == Qnst::Body){
        requestBodyAddition((QnstGraphicsBody*) entity);

    // if the entity is of type CONTEXT
    }else  if (entity->getnstType() == Qnst::Context){
        requestContextAddition((QnstGraphicsContext*) entity);

    // if the entity is of type SWITCH
    }else  if (entity->getnstType() == Qnst::Switch){
        requestSwitchAddition((QnstGraphicsSwitch*) entity);

    // if the entity is of type MEDIA
    }else  if (entity->getnstType() == Qnst::Image ||
           entity->getnstType() == Qnst::Audio ||
           entity->getnstType() == Qnst::Text ||
           entity->getnstType() == Qnst::Video ||
           entity->getnstType() == Qnst::Script ||
           entity->getnstType() == Qnst::Settings){

        requestMediaAddition((QnstGraphicsMedia*) entity);

    }else if (entity->getnstType() == Qnst::Port){
        requestPortAddition((QnstGraphicsPort*) entity);
    }
}

void QnstView::requestEntityRemotion(QnstEntity* entity)
{
    qDebug() << "[QNST]" << "[" << ++n << "]" << ":" << "Requesting entity remotion '"+entity->getnstUid()+"'";

    QString uid = entity->getnstUid();

    QnstGraphicsComposition* parent =  (QnstGraphicsComposition*) entity->getnstParent();

    if (parent != NULL){
        // if the entity is of type COMPOSITION
        if (entity->getnstType() == Qnst::Body ||
            entity->getnstType() == Qnst::Context ||
            entity->getnstType() == Qnst::Switch){

            deepRemotion((QnstGraphicsComposition*) entity);

            parent->removencgGraphicsEntity((QnstGraphicsComposition*) entity);

        // if the entity is of type MEDIA
        }else  if (entity->getnstType() == Qnst::Image ||
               entity->getnstType() == Qnst::Audio ||
               entity->getnstType() == Qnst::Text ||
               entity->getnstType() == Qnst::Video ||
               entity->getnstType() == Qnst::Script ||
               entity->getnstType() == Qnst::Settings ||
               entity->getnstType() == Qnst::Media){

            parent->removencgGraphicsEntity((QnstGraphicsMedia*) entity);

        // if the entity is of type INTERFACE
        }else if (entity->getnstType() == Qnst::Port){
            parent->removencgGraphicsEntity((QnstGraphicsInterface*) entity);
        }
    }else{
        deepRemotion((QnstGraphicsComposition*) entity);

        scene->removeItem((QnstGraphicsComposition*) entity);
    }

    entities.remove(uid);

    emit entityRemoved(uid);
}

void QnstView::requestEntityChange(QnstEntity* entity)
{
    qDebug() << "[QNST]" << "[" << ++n << "]" << ":" << "Requesting entity change '"+entity->getnstUid()+"'";   

    // if the entity is of type BODY
    if (entity->getnstType() == Qnst::Body){
        requestBodyChange((QnstGraphicsBody*) entity);

    // if the entity is of type CONTEXT
    }else  if (entity->getnstType() == Qnst::Context){
        requestContextChange((QnstGraphicsContext*) entity);

    // if the entity is of type SWITCH
    }else  if (entity->getnstType() == Qnst::Switch){
        requestSwitchChange((QnstGraphicsSwitch*) entity);

    // if the entity is of type MEDIA
    }else  if (entity->getnstType() == Qnst::Media ||
               entity->getnstType() == Qnst::Image ||
               entity->getnstType() == Qnst::Audio ||
               entity->getnstType() == Qnst::Text ||
               entity->getnstType() == Qnst::Video ||
               entity->getnstType() == Qnst::Script ||
               entity->getnstType() == Qnst::Settings){

        requestMediaChange((QnstGraphicsMedia*) entity);

    // if the entity is of type INTERFACE
    }else if (entity->getnstType() == Qnst::Port){
        requestPortChange((QnstGraphicsPort*) entity);
    }
}

void QnstView::requestEntitySelection(QnstEntity* entity)
{
    qDebug() << "[QNST]" << "[" << ++n << "]" << ":" << "Requesting entity selection '"+entity->getnstUid()+"'";
    // TODO:

    if (selected != NULL){
        switch(selected->getnstType()){
        case Qnst::Image:

        case Qnst::Audio:

        case Qnst::Video:

        case Qnst::Text:

        case Qnst::Script:

        case Qnst::Settings:

        case Qnst::Media:
            ((QnstGraphicsContent*) selected)->setSelected(false);
            ((QnstGraphicsContent*) selected)->adjust();

            break;

        case Qnst::Body:

        case Qnst::Switch:

        case Qnst::Context:
            ((QnstGraphicsComposition*) selected)->setSelected(false);
            ((QnstGraphicsComposition*) selected)->adjust();

            break;

        case Qnst::Port:
            ((QnstGraphicsInterface*) selected)->setSelected(false);
            ((QnstGraphicsInterface*) selected)->adjust();

            break;
        }
    }

    selected = entity;

    emit entitySelected(entity->getnstUid());
}

void QnstView::requestBodyAddition(QnstGraphicsBody* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "body";

    properties["id"] = "TODO";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());

    emit entityAdded(entity->getnstUid(), "", properties);
}

void QnstView::requestBodyChange(QnstGraphicsBody* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "body";

    properties["id"] = "TODO";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());

    emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestContextAddition(QnstGraphicsContext* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "context";

    properties["id"] = "TODO";
    properties["refer"] = "TODO";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());

    emit entityAdded(entity->getnstUid(), entity->getnstParent()->getnstUid(), properties);
}

void QnstView::requestContextChange(QnstGraphicsContext* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "context";

    properties["id"] = "TODO";
    properties["refer"] = "TODO";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());

    emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestSwitchAddition(QnstGraphicsSwitch* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "switch";

    properties["id"] = "TODO";
    properties["refer"] = "TODO";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());

    emit entityAdded(entity->getnstUid(), entity->getnstParent()->getnstUid(), properties);
}

void QnstView::requestSwitchChange(QnstGraphicsSwitch* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "switch";

    properties["id"] = "TODO";
    properties["refer"] = "TODO";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());

    emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestMediaAddition(QnstGraphicsMedia* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "media";

    properties["id"] = "TODO";
    properties["src"] = "TODO";
    properties["refer"] = "TODO";
    properties["instance"] = "TODO";
    properties["type"] = "TODO";
    properties["descriptor"] = "TODO";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());

    emit entityAdded(entity->getnstUid(), entity->getnstParent()->getnstUid(), properties);
}

void QnstView::requestMediaChange(QnstGraphicsMedia* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "media";

    properties["id"] = "TODO";
    properties["src"] = "TODO";
    properties["refer"] = "TODO";
    properties["instance"] = "TODO";
    properties["type"] = "TODO";
    properties["descriptor"] = "TODO";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());

    emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestPortAddition(QnstGraphicsPort* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "port";

    properties["id"] = "TODO";
    properties["component"] = "TODO";
    properties["interface"] = "TODO";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());

    emit entityAdded(entity->getnstUid(), entity->getnstParent()->getnstUid(), properties);
}

void QnstView::requestPortChange(QnstGraphicsPort* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "port";

    properties["id"] = "TODO";
    properties["component"] = "TODO";
    properties["interface"] = "TODO";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());

    emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::deepRemotion(QnstGraphicsComposition* composition)
{
    foreach(QncgGraphicsEntity* entity, composition->getncgGraphicsEntities()){
        QnstEntity* e;

        if (entity->getncgType() == Qncg::Node){
            e = (QnstGraphicsNode*) entity;

        }else if (entity->getncgType() == Qncg::Interface){
            e = (QnstGraphicsInterface*) entity;
        }

        requestEntityRemotion(e);
    }
}

void QnstView::deepRemotion2(QnstGraphicsComposition* composition)
{
    foreach(QncgGraphicsEntity* entity, composition->getncgGraphicsEntities()){
        QnstEntity* e;

        if (entity->getncgType() == Qncg::Node){
            e = (QnstGraphicsNode*) entity;

        }else if (entity->getncgType() == Qncg::Interface){
            e = (QnstGraphicsInterface*) entity;
        }

        removeEntity(e->getnstUid());
    }
}

void QnstView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);

    if (!event->isAccepted()){
        if (event->button() == Qt::LeftButton){
            if (selected != NULL){
                if (selected->getnstType() == Qnst::Body){
                    ((QnstGraphicsBody*) selected)->setSelected(false);
                    ((QnstGraphicsBody*) selected)->adjust();
                }

                selected = NULL;
            }
        }

        event->accept();
    }
}

void QnstView::keyPressEvent(QKeyEvent* event)
{
    QGraphicsView::keyPressEvent(event);
}
