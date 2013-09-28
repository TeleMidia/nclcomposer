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

#include <QDebug>

QnstEntity::QnstEntity(QnstEntity* parent)
{
  setnstType(Qnst::NoType);

  setnstParent(parent);

  if(parent)
    parent->addnstEntity(this);

  setnstUid(QUuid::createUuid().toString());
}

QnstEntity::~QnstEntity()
{

}

QString QnstEntity::getnstUid() const
{
  return uid;
}

void QnstEntity::setnstUid(const QString &uid)
{
  this->uid = uid;
}

QString QnstEntity::getnstId() const
{
  return id;
}

void QnstEntity::setnstId(const QString &id)
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

bool QnstEntity::isMedia()
{
  return ( (this->type >= Qnst::Media) &&
           (this->type <= Qnst::Settings) );
}

QMap <QString, QString> QnstEntity::getUsrData()
{
  return userData;
}

void QnstEntity::setUsrData(QMap <QString, QString> newUserData)
{
  userData = newUserData;
}

QnstEntity* QnstEntity::getnstParent() const
{
  return parent;
}

void QnstEntity::setnstParent(QnstEntity* parent)
{
  this->parent = parent;
}

QSet <QnstEntity*> QnstEntity::getnstEntities()
{
  return entities;
}

void QnstEntity::addnstEntity(QnstEntity* entity)
{
  if (entity != NULL)
  {
    // \fixme This is only an additional check. Maybe, the better way to do
    // that could be using a set instead of a vector to entities.
    if(entities.contains(entity))

#ifdef Q_WS_MAC
    qWarning() << "[QNST] Warning! You are adding the same entity twice as \
      child of " << this << __FILE__ << __LINE__;
#else
//    qWarning() << "[QNST] Warning! You are adding the same entity twice as \
//      child of " << (int) this << __FILE__ << __LINE__;
#endif

    entity->setnstParent(this);
    entities.insert(entity);
  }
}

void QnstEntity::removenstEntity(QnstEntity* entity)
{
  if (entity != NULL)
  {
    entities.remove(entity);
    entity->setnstParent(NULL);
  }
}
