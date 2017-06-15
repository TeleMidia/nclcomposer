/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#include "model/Entity.h"

#include <assert.h>

CPR_CORE_BEGIN_NAMESPACE

Entity::Entity(QDomDocument &doc, Entity *parent) :
  QObject()
{
  setUniqueID(QUuid::createUuid().toString());

  this->_doc = doc;
  _element = doc.createElement("empty");

  this->parent = parent;
  this->deleteChildren = true;
}

Entity::Entity(const QMap<QString,QString> &atts, QDomDocument &doc,
               Entity *parent) :
    QObject()
{
  setUniqueID(QUuid::createUuid().toString());

  this->_doc = doc;
  _element = doc.createElement("empty");

  foreach (const QString &att, atts.keys())
    _element.setAttribute(att, atts[att]);

  this->parent = parent;
  this->deleteChildren = true;
}

Entity::Entity(const QString &uniqueId,
               const QString &type,
               const QMap<QString, QString> &atts,
               QDomDocument &doc,
               Entity *parent) :
  QObject()
{
  this->_id = uniqueId;
  this->_doc = doc;
  _element = doc.createElement(type);

  foreach (const QString &att, atts.keys())
    _element.setAttribute(att, atts[att]);

  this->parent = parent;
  this->deleteChildren = true;
  this->setType(type);
}

Entity::~Entity()
{
  if (deleteChildren)
  {
    while (children.size())
    {
      Entity *child = children.at(0);
      delete child;
      children.pop_front();
    }
  }
}

void Entity::setAttribute(const QString &name, const QString &value)
{
  _element.setAttribute(name, value);
}

void Entity::setAtrributes(const QMap<QString,QString> &newatts)
{
  foreach (const QString &att, newatts.keys())
    _element.setAttribute(att, newatts[att]);
}

void Entity::setType(const QString &type)
{
  _element.setTagName(type);
}

void Entity::setUniqueID(const QString &uniqueId)
{
  this->_id = uniqueId;
}

void Entity::setParent(Entity *parent)
{
  this->parent = parent;
}

bool Entity::addChild(Entity *entity, int pos)
{
  assert(entity != nullptr);
  QString _id = entity->getUniqueId();

  // Check if the entity is already children of this entity
  // TODO: THIS CAN BE IMPROVED!! Maybe checking if the parentUniqueID.
  for(int i = 0; i < children.size(); i++)
  {
    if(children.at(i)->getUniqueId() == _id )
      return false;
  }

  children.insert(pos >= 0 ? pos: children.size(), entity);
  entity->setParent(this);
  return true;
}

//! Deletes the child and its children in a recursive way
bool Entity::deleteChild(Entity *entity)
{
  assert(entity != nullptr);

  entity->setDeleteChildren(true);
  for(int i = 0; i < children.size(); i++)
  {
    if(children.at(i) == entity)
    {
      children.remove(i);
    }
  }

  delete entity;

  return true;
}

QString Entity::getAttribute(const QString &name) const
{
  return _element.attribute(name);
}

QMap <QString, QString> Entity::getAttributes () const
{
  QMap <QString, QString> attrs;
  QDomNamedNodeMap domAttrs = _element.attributes();
  for (int i = 0; i < domAttrs.length(); i++)
  {
    QDomAttr item = domAttrs.item(i).toAttr();
    attrs[item.name()] = item.value();
  }

  return attrs;
}

bool Entity::hasAttribute(const QString &name) const
{
  return _element.attributes().contains(name);
}

QString Entity::getUniqueId() const
{
  return _id;
}

QString Entity::getType() const
{
  return _element.tagName();
}

Entity* Entity::getParent() const
{
  return parent;
}

QString Entity::getParentUniqueId() const
{
  return parent->getUniqueId();
}

void Entity::setDeleteChildren(bool _delete)
{
  this->deleteChildren = _delete;
}

QVector <Entity *> Entity::getChildren() const
{
  return this->children;
}

//! Prints the Entity and its children
void Entity::print()
{
  for (int i = 0; i < children.size(); i++)
  {
    Entity *child = children.at(i);
    child->print();
  }
}

QString Entity::toString(int ntab, bool writeuid)
{
  QString out = "";
  for(int i = 0; i < ntab; i++)
    out += "\t";

  out += "<";
  out.append(getType().toLatin1());
  QMap <QString, QString> atts = getAttributes();
  for(const QString &attr: atts.keys())
  {
    out += " ";
    out.append(attr);
    out += "=\"";
    out += atts.value(attr);
    out += "\"";
  }
  if(writeuid)
  {
    out += " uniqueEntityId=\"";
    out += getUniqueId();
    out += "\"";
  }
  out += ">\n";

  for (int i = 0; i < children.size(); i++)
  {
    Entity *child = children.at(i);
    out += child->toString(ntab+1, writeuid);
  }
  for(int i = 0; i < ntab; i++)
    out += "\t";
  out += "</";
  out += getType();
  out += ">\n";
  return out;
}

Entity *Entity::cloneEntity()
{
  return new Entity(
        getUniqueId(), getType(), getAttributes(), this->_doc, this->parent);
}

CPR_CORE_END_NAMESPACE
