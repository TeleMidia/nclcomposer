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
#include "qnstentity.h"

QnstEntity::QnstEntity()
{
    setUid(QUuid::createUuid().toString());
    setName("");
}

QnstEntity::~QnstEntity()
{
}

QString QnstEntity::getUid() const
{
    return uid;
}

void QnstEntity::setUid(const QString &uid)
{
    this->uid = uid;
}

int QnstEntity::getnstType() const
{
    return nsttype;
}

void QnstEntity::setnstType(int nsttype)
{
    this->nsttype = nsttype;
}

QnstEntity* QnstEntity::getnstParent() const
{
    return nstparent;
}

void QnstEntity::setnstParent(QnstEntity* nstparent)
{
    this->nstparent = nstparent;
}

void QnstEntity::setName(QString name)
{
    this->name = name;
}

QString QnstEntity::getName() const
{
    return name;
}
