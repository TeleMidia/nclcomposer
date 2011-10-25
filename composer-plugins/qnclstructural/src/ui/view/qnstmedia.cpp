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
