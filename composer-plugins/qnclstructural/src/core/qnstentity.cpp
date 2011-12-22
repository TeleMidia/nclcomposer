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

QnstEntity::QnstEntity(QnstEntity* parent)
{
    setnstType(Qnst::NoType);

    setnstParent(parent);

    setnstUid((QString) QUuid::createUuid());
}

QnstEntity::~QnstEntity()
{

}

QString QnstEntity::getnstUid() const
{
    return uid;
}

void QnstEntity::setnstUid(QString uid)
{
    this->uid = uid;
}

QString QnstEntity::getnstId() const
{
    return id;
}

void QnstEntity::setnstId(QString id)
{
    this->id = id;
}

QnstType QnstEntity::getnstType() const
{
    return type;
}

void QnstEntity::setnstType(QnstType type)
{
    this->type = type;
}

QnstEntity* QnstEntity::getnstParent() const
{
    return parent;
}

void QnstEntity::setnstParent(QnstEntity* parent)
{
    this->parent = parent;
}

QVector<QnstEntity*> QnstEntity::getnstEntities()
{
    return entities;
}

void QnstEntity::addnstEntity(QnstEntity* entity)
{
    if (entity != NULL){
        entities.append(entity);
    }
}

void QnstEntity::removenstEntity(QnstEntity* entity)
{
    if (entity != NULL){
        int index = entities.indexOf(entity);

        if (index >= 0){
            entities.remove(index);
        }
    }
}
