/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "qnstswitch.h"

QnstSwitch::QnstSwitch(QnstNode* parent)
    : QnstComposite(parent)
{
    setColor("#C6E2FF");
    setBordertColor("#4A708B");

    createConnection();

    setnstType(Qnst::Switch);
}

QnstSwitch::~QnstSwitch()
{

}

void QnstSwitch::addContext()
{
    QnstSwitch* context = new QnstSwitch(this);
    context->setParentItem(this);
    context->setTop(getHeight()/2-35);
    context->setLeft(getWidth()/2-35);
    context->setWidth(100);
    context->setHeight(100);
    context->setnstParent(this);
    context->adjust();

    connect(context,SIGNAL(entitySelected(QnstEntity*)),
                 SIGNAL(entitySelected(QnstEntity*)));

    connect(context,SIGNAL(entityAdded(QnstEntity*)),
            SIGNAL(entityAdded(QnstEntity*)));

    emit entityAdded(context);
}

void QnstSwitch::addSwitch()
{
    QnstSwitch* sswitch = new QnstSwitch(this);
    sswitch->setParentItem(this);
    sswitch->setTop(getHeight()/2-35);
    sswitch->setLeft(getWidth()/2-35);
    sswitch->setWidth(100);
    sswitch->setHeight(100);
    sswitch->setnstParent(this);
    sswitch->adjust();

    connect(sswitch,SIGNAL(entitySelected(QnstEntity*)),
                 SIGNAL(entitySelected(QnstEntity*)));

    connect(sswitch,SIGNAL(entityAdded(QnstEntity*)),
            SIGNAL(entityAdded(QnstEntity*)));

    emit entityAdded(sswitch);
}

void QnstSwitch::addPort()
{
    qDebug() << "add port...";
    QnstPort* port = new QnstPort(this);
    port->setParentItem(this);
    port->setTop(0);
    port->setLeft(0);
    port->setWidth(28);
    port->setHeight(28);
    port->setnstParent(this);
    port->adjust();

    connect(port,SIGNAL(entitySelected(QnstEntity*)),
                 SIGNAL(entitySelected(QnstEntity*)));

    connect(port,SIGNAL(entityAdded(QnstEntity*)),
            SIGNAL(entityAdded(QnstEntity*)));

    emit entityAdded(port);
}

void QnstSwitch::addProperty()
{
    QnstProperty* property = new QnstProperty(this);
    property->setParentItem(this);
    property->setTop(0);
    property->setLeft(0);
    property->setWidth(35);
    property->setHeight(35);
    property->setnstParent(this);
    property->adjust();

    connect(property,SIGNAL(entitySelected(QnstEntity*)),
                 SIGNAL(entitySelected(QnstEntity*)));

    connect(property,SIGNAL(entityAdded(QnstEntity*)),
            SIGNAL(entityAdded(QnstEntity*)));

    emit entityAdded(property);
}

void QnstSwitch::addMedia()
{
    QnstMedia* media = new QnstMedia(this);
    media->setParentItem(this);
    media->setTop(getHeight()/2-35);
    media->setLeft(getWidth()/2-35);
    media->setWidth(35);
    media->setHeight(35);
    media->setnstParent(this);
    media->adjust();

    connect(media,SIGNAL(entitySelected(QnstEntity*)),
                 SIGNAL(entitySelected(QnstEntity*)));

    connect(media,SIGNAL(entityAdded(QnstEntity*)),
            SIGNAL(entityAdded(QnstEntity*)));

    emit entityAdded(media);
}

void QnstSwitch::createConnection()
{
    connect(switchAction, SIGNAL(triggered()), SLOT(addSwitch()));

    connect(contextAction, SIGNAL(triggered()), SLOT(addContext()));

    connect(portAction, SIGNAL(triggered()), SLOT(addPort()));

    connect(propertyAction, SIGNAL(triggered()), SLOT(addProperty()));

    connect(mediaAction, SIGNAL(triggered()), SLOT(addMedia()));
}
