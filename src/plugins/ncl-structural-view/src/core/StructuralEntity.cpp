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
#include "StructuralEntity.h"

#include <QDebug>

StructuralEntity::StructuralEntity(StructuralEntity* parent)
{
  setnstType(Structural::NoType);

  setnstParent(parent);

  if(parent)
    parent->addnstEntity(this);

  setnstUid(QUuid::createUuid().toString());
}

StructuralEntity::~StructuralEntity()
{

}

QString StructuralEntity::getnstUid() const
{
  return uid;
}

void StructuralEntity::setnstUid(const QString &uid)
{
  this->uid = uid;
}

QString StructuralEntity::getnstId() const
{
  return id;
}

void StructuralEntity::setnstId(const QString &id)
{
  this->id = id;
}

QnstType StructuralEntity::getnstType() const
{
  return type;
}

void StructuralEntity::setnstType(QnstType type)
{
  this->type = type;
}

bool StructuralEntity::isMedia()
{
  return ( (this->type >= Structural::Media) &&
           (this->type <= Structural::Settings) );
}

QMap <QString, QString> StructuralEntity::getUsrData()
{
  return userData;
}

void StructuralEntity::setUsrData(QMap <QString, QString> newUserData)
{
  userData = newUserData;
}

StructuralEntity* StructuralEntity::getnstParent() const
{
  return parent;
}

void StructuralEntity::setnstParent(StructuralEntity* parent)
{
  this->parent = parent;
}

QSet <StructuralEntity*> StructuralEntity::getnstEntities()
{
  return entities;
}

void StructuralEntity::addnstEntity(StructuralEntity* entity)
{
  if (entity != NULL)
  {
    // \fixme This is only an additional check. Maybe, the better way to do
    // that could be using a set instead of a vector to entities.
    if(entities.contains(entity))

#ifdef Q_OS_MAC
    qWarning() << "[QNST] Warning! You are adding the same entity twice as \
      child of " << this << __FILE__ << __LINE__;
#else
//    qWarning() << "[QNST] Warning! You are adding the same entity twice as
//      child of " << (int) this << __FILE__ << __LINE__;
#endif

    entity->setnstParent(this);
    entities.insert(entity);
  }
}

void StructuralEntity::removenstEntity(StructuralEntity* entity)
{
  if (entity != NULL)
  {
    entities.remove(entity);
    entity->setnstParent(NULL);
  }
}
