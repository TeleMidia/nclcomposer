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

#include "model/Node.h"
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

CPR_CORE_END_NAMESPACE
