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

Node::Node (QDomDocument &doc, Node *parent) : QObject ()
{
  setUniqueID (QUuid::createUuid ().toString ());
  this->_doc = doc;
  this->_parent = parent;
}

Node::Node (const QString &uniqueId, QDomDocument &doc, Node *parent)
{
  setUniqueID (uniqueId);
  this->_doc = doc;
  this->_parent = parent;
}

Node::~Node ()
{
  if (_deleteChildren)
  {
    while (_children.size ())
    {
      Node *child = _children.at (0);
      delete child;
      _children.pop_front ();
    }
  }
}

void
Node::setUniqueID (const QString &uniqueId)
{
  this->_uid = uniqueId;
}

QString
Node::getUniqueId () const
{
  return this->_uid;
}

void
Node::setParent (Node *parent)
{
  this->_parent = parent;
}

bool
Node::addChild (Node *entity, int pos)
{
  assert (entity != nullptr);
  QString _id = entity->getUniqueId ();

  // Check if the entity is already children of this entity
  // TODO: THIS CAN BE IMPROVED!! Maybe checking if the parentUniqueID.
  for (int i = 0; i < _children.size (); i++)
  {
    if (_children.at (i)->getUniqueId () == _id)
      return false;
  }

  _children.insert (pos >= 0 ? pos : _children.size (), entity);
  entity->setParent (this);
  return true;
}

//! Deletes the child and its children in a recursive way
bool
Node::deleteChild (Node *node)
{
  assert (node != nullptr);

  node->setDeleteChildren (true);
  for (int i = 0; i < _children.size (); i++)
  {
    if (_children.at (i) == node)
    {
      _children.removeAt (i);
    }
  }

  delete node;

  return true;
}

Node *
Node::getParent () const
{
  return _parent;
}

QString
Node::getParentUniqueId () const
{
  return _parent->getUniqueId ();
}

void
Node::setDeleteChildren (bool _del)
{
  this->_deleteChildren = _del;
}

QList<Node *>
Node::getChildren () const
{
  return this->_children;
}

QList<Entity *>
Node::getEntityChildren () const
{
  QList<Entity *> children;
  for (auto i = _children.begin (); i != _children.end (); ++i)
  {
    Entity *ent = dynamic_cast<Entity *> (*i);
    if (ent)
      children.append (ent);
  }
  return children;
}

// Entity
Entity::Entity (QDomDocument &doc, Entity *parent) : Node (doc, parent)
{
  this->_doc = doc;
  _domNode = _domNode = doc.createElement ("empty");

  this->_parent = parent;
  this->setType ("empty");
}

Entity::Entity (const QMap<QString, QString> &atts, QDomDocument &doc,
                Entity *parent)
    : Node (doc, parent)
{
  this->_doc = doc;
  _domNode = doc.createElement ("empty");

  foreach (const QString &att, atts.keys ())
    _domNode.toElement ().setAttribute (att, atts[att]);

  this->_parent = parent;
  this->setType ("empty");
}

Entity::Entity (const QString &uniqueId, const QString &type,
                const QMap<QString, QString> &atts, QDomDocument &doc,
                Entity *parent)
    : Node (uniqueId, doc, parent)
{
  this->_doc = doc;
  _domNode = doc.createElement (type);

  foreach (const QString &att, atts.keys ())
    _domNode.toElement ().setAttribute (att, atts[att]);

  this->_parent = parent;
  this->setType (type);
}

Entity::~Entity () {}

void
Entity::setAttr (const QString &name, const QString &value)
{
  _domNode.toElement ().setAttribute (name, value);
}

void
Entity::setAtrrs (const QMap<QString, QString> &newatts)
{
  // clear previous attributes
  QStringList oldAttrs;
  QDomNamedNodeMap domAttrs = _domNode.toElement ().attributes ();
  for (int i = 0; i < domAttrs.length (); i++)
    oldAttrs << domAttrs.item (i).toAttr ().name();

  foreach (const QString &att, oldAttrs)
    _domNode.toElement().removeAttribute(att);

  // set new attributes
  foreach (const QString &att, newatts.keys ())
    _domNode.toElement ().setAttribute (att, newatts[att]);
}

void
Entity::setType (const QString &type)
{
  _domNode.toElement ().setTagName (type);
}

QString
Entity::getAttribute (const QString &name) const
{
  return _domNode.toElement ().attribute (name);
}

QMap<QString, QString>
Entity::getAttributes () const
{
  QMap<QString, QString> attrs;
  QDomNamedNodeMap domAttrs = _domNode.toElement ().attributes ();
  for (int i = 0; i < domAttrs.length (); i++)
  {
    QDomAttr item = domAttrs.item (i).toAttr ();
    attrs[item.name ()] = item.value ();
  }

  return attrs;
}

bool
Entity::hasAttribute (const QString &name) const
{
  return _domNode.toElement ().attributes ().contains (name);
}

QString
Entity::getType () const
{
  return _domNode.toElement ().tagName ();
}

//! Prints the Entity and its children
// void
// Entity::print ()
//{
//  for (int i = 0; i < _children.size (); i++)
//  {
//    Entity *child = _children.at (i);
//    child->print ();
//  }
//}

QString
Entity::toString (int ntab, bool writeuid)
{
  QString out = "";
  for (int i = 0; i < ntab; i++)
    out += "\t";

  out += "<";
  out.append (getType ().toLatin1 ());
  QMap<QString, QString> atts = getAttributes ();
  for (const QString &attr : atts.keys ())
  {
    out += " ";
    out.append (attr);
    out += "=\"";
    out += atts.value (attr);
    out += "\"";
  }

  if (writeuid)
  {
    out += " uniqueEntityId=\"";
    out += getUniqueId ();
    out += "\"";
  }
  out += ">\n";

  for (int i = 0; i < _children.size (); i++)
  {
    Node *child = _children.at (i);
    out += child->toString (ntab + 1, writeuid);
  }

  for (int i = 0; i < ntab; i++)
    out += "\t";

  out += "</";
  out += getType ();
  out += ">\n";
  return out;
}

Entity *
Entity::clone ()
{
  return new Entity (getUniqueId (), getType (), getAttributes (), this->_doc,
                     (Entity *)this->_parent);
}

CPR_CORE_END_NAMESPACE
