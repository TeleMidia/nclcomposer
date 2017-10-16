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

#include "model/Project.h"

#include <QReadWriteLock>

#include <assert.h>

CPR_CORE_BEGIN_NAMESPACE

Project::Project (QDomDocument &doc) : Entity (doc)
{
  init ();
}

Project::Project (const QMap<QString, QString> &atts, QDomDocument &doc)
    : Entity (atts, doc)
{
  init ();
}

Project::Project (const QString &uniqueId, const QMap<QString, QString> &atts,
                  QDomDocument &doc)
    : Entity (uniqueId, "document", atts, doc)
{
  init ();
}

void
Project::init ()
{
  setType ("document");
  dirty = false;

  this->dirty = false;
  this->_nodes[this->getUniqueId ()] = this;

  _lockEntities = new QMutex ();
}

Project::~Project ()
{
  _nodes.clear ();
  delete _lockEntities;
}

LanguageType
Project::getProjectType ()
{
  return this->_projectType;
}

void
Project::setProjectType (LanguageType type)
{
  this->_projectType = type;
}

Entity *
Project::getEntityById (const QString &id)
{
  QMutexLocker locker (_lockEntities);
  Node *node = _nodes.contains (id) ? _nodes[id] : nullptr;

  return dynamic_cast <Entity *> (node);
}

QList<Entity *>
Project::getEntitiesbyType (const QString &type)
{
  QMutexLocker locker (_lockEntities);
  QMapIterator<QString, Node *> it (_nodes);
  QList<Entity *> listRet;

  while (it.hasNext ())
  {
    it.next ();
    Entity *ent = dynamic_cast <Entity *> (it.value());
    if (ent && ent->getType () == type)
      listRet.append (ent);
  }
  return listRet;
}

QString
Project::getLocation ()
{
  return this->_projectLocation;
}

void
Project::setLocation (const QString &location)
{
  this->_projectLocation = location;
}

bool
Project::addEntity (Entity *entity,
                    const QString &parentId) throw (EntityNotFound,
                                                    ParentNotFound)
{
  assert (entity != nullptr);

  QMutexLocker locker (_lockEntities);
  if (!_nodes.contains (parentId))
  {
    throw ParentNotFound (entity->getType (), entity->getType (), parentId);
    return false;
  }

  if (_nodes.contains (entity->getUniqueId ()))
  {
    throw EntityNotFound (entity->getType (), entity->getUniqueId ());
    return false;
  }

  Node *parent = _nodes[parentId];
  parent->addChild (entity);
  _nodes[entity->getUniqueId ()] = entity;

  setDirty (true);
  return true;
}

bool
Project::addComment (Comment *comment,
                     const QString &parentId) throw (EntityNotFound,
                                                    ParentNotFound)
{
  assert (comment != nullptr);

  QMutexLocker locker (_lockEntities);
  if (!_nodes.contains (parentId))
  {
    throw ParentNotFound ("comment", "comment", parentId);
    return false;
  }

  if (_nodes.contains (comment->getUniqueId ()))
  {
    throw EntityNotFound ("comment", comment->getUniqueId ());
    return false;
  }

  Node *parent = _nodes[parentId];
  parent->addChild (comment);
  _nodes[comment->getUniqueId ()] = comment;

  setDirty (true);
  return true;
}

bool
Project::removeEntity (Entity *entity, bool appendChild) throw (EntityNotFound)
{
  Q_UNUSED (appendChild)

  assert (entity != nullptr);

  QMutexLocker locker (_lockEntities);
  if (_nodes.contains (entity->getUniqueId ()))
  {
    Node *parent = entity->getParent ();
    if (parent)
    {
      QStack<Node *> stack;
      // remove all children
      stack.push (entity);
      while (stack.size ())
      {
        Node *currentEntity = stack.top ();
        stack.pop ();
        _nodes.remove (currentEntity->getUniqueId ());

        QList <Node *> children = currentEntity->getChildren ();
        for (auto i = children.begin(); i != children.end (); ++i)
          stack.push (*i);
      }

      // DELETE the entity and its children recursivelly
      parent->deleteChild (entity);
    }
    else
    {
      // does not have a parent, so don't append
      delete entity;
      entity = nullptr;
    }
  }
  else
  {
    throw EntityNotFound (entity->getType (), entity->getUniqueId ());
    return false; // entity does not exist in the model
  }

  setDirty (true);
  return true;
}

/** \todo Returns a string to be saved in hard disk. */
QString
Project::toString (int ntabs, bool writeuid)
{
  Q_UNUSED (ntabs);
  Q_UNUSED (writeuid);

  QString result = "";
  result += "#COMPOSER_PROJECT name=\"" + this->_projectName
            + "\" version=\"" + NCLCOMPOSER_CORE_VERSION + "\"#\n";

  result += "#COMPOSER_MODEL#\n";
  result += Entity::toString (0);
  result += "#END_COMPOSER_MODEL#\n";

  for (const QString &key : _pluginData.keys ())
  {
    result += "#COMPOSER_PLUGIN_DATA " + key + "#\n";
    result += _pluginData[key];
    result += "\n#END_COMPOSER_PLUGIN_DATA#\n";
  }

  return result;
}

bool
Project::setPluginData (const QString &pluginId, const QByteArray &data)
{
  this->_pluginData[pluginId] = data;

  setDirty (true);
  return true;
}

QByteArray
Project::getPluginData (const QString &pluginId)
{
  if (_pluginData.contains (pluginId))
  {
    return this->_pluginData[pluginId];
  }
  return QByteArray ();
}

bool
Project::isDirty ()
{
  return dirty;
}

void
Project::setDirty (bool isDirty)
{
  dirty = isDirty;
  emit dirtyProject (isDirty);
}

QString
Project::generateUniqueAttrId (const QString &tagname)
{
  QList<Entity *> elements = getEntitiesbyType (tagname);
  QList<QString> currentElementsNCLID;
  for (int i = 0; i < elements.size (); i++)
  {
    if (elements.at (i)->hasAttribute ("id"))
      currentElementsNCLID.push_back (elements.at (i)->getAttribute ("id"));
  }

  for (int i = 1;; i++)
  {
    QString retNCLID = tagname + QString::number (i);
    if (!currentElementsNCLID.contains (retNCLID))
      return retNCLID;
  }
}

QList<Entity *>
Project::getEntityByAttrId (const QString &id)
{
  QMutexLocker locker (_lockEntities);
  QMapIterator<QString, Node *> it (_nodes);
  QList<Entity *> listRet;

  while (it.hasNext ())
  {
    it.next ();
    Entity *ent = dynamic_cast <Entity *> (it.value ());
    if (ent && ent->hasAttribute ("id") && ent->getAttribute ("id") == id)
      listRet.append (ent);
  }
  return listRet;
}

CPR_CORE_END_NAMESPACE
