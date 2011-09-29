/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "qnstmedia.h"

QnstMedia::QnstMedia(QnstNode* parent)
    : QnstContent(parent)
{
    setColor("#FF6347");
    setBordertColor("#8E2323");

    createConnection();

    setnstType(Qnst::Media);

    portAction->setEnabled(false);
    propertyAction->setEnabled(false);
}

QnstMedia::~QnstMedia()
{

}

void QnstMedia::addArea()
{
    QnstArea* area = new QnstArea(this);
    area->setParentItem(this);
    area->setTop(0);
    area->setLeft(0);
    area->setWidth(35);
    area->setHeight(35);
    area->setnstParent(this);
    area->adjust();

    connect(area,SIGNAL(entitySelected(QnstEntity*)),
                 SIGNAL(entitySelected(QnstEntity*)));

    connect(area,SIGNAL(entityAdded(QnstEntity*)),
            SIGNAL(entityAdded(QnstEntity*)));

    connect(area,SIGNAL(entityRemoved(QnstEntity*)),
            SIGNAL(entityRemoved(QnstEntity*)));

    emit entityAdded(area);
}

void QnstMedia::createConnection()
{
    connect(areaAction, SIGNAL(triggered()), SLOT(addArea()));
}
