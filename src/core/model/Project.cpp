/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public
 License along with Foobar.  If not, see <http://www.gnu.org/licenses/>. */

#include "model/Project.h"

#include <QReadWriteLock>

#include <assert.h>

namespace composer {
  namespace core {
    namespace model {

Project::Project(QObject *parent) :
  Entity(parent)
{
  init();
}

Project::Project(const QMap<QString,QString> &atts, QObject *parent) :
  Entity(atts, parent)
{
  init();
}

Project::Project( const QString &uniqueId,
                  const QMap<QString,QString> &atts,
                  QObject *parent) :
  Entity(uniqueId, "document", atts, parent)
{
  init();
}

void Project::init()
{
  setType("document");
  dirty = false;

  this->dirty = false;
  this->entities[this->getUniqueId()] = this;

  lockEntities = new QMutex();
}

Project::~Project()
{
// QMutexLocker locker(lockEntities);
  entities.clear();
  delete lockEntities;
}

LanguageType Project::getProjectType()
{
  return this->projectType;
}

void Project::setProjectType(LanguageType type)
{
  this->projectType = type;
}

Entity* Project::getEntityById(const QString &id)
{
  QMutexLocker locker(lockEntities);
  return entities.contains(id) ? entities[id] : nullptr;
}

QList<Entity*> Project::getEntitiesbyType(const QString &type)
{
  QMutexLocker locker(lockEntities);
  QMapIterator<QString, Entity*> it(entities);
  QList<Entity*> listRet;

  while(it.hasNext())
  {
    it.next();
    Entity* ent = it.value();
    if (ent->getType() == type)
        listRet.append(ent);
  }
  return listRet;
}

QString Project::getLocation()
{
  // QMutexLocker locker(&lockLocation);
  return this->projectLocation;
}

void Project::setLocation(const QString &location)
{
  // QMutexLocker locker(&lockLocation);
  this->projectLocation = location;
}

bool Project::addEntity(Entity* entity, const QString &parentId)
    throw (EntityNotFound, ParentNotFound)
{
  assert(entity != nullptr);

  QMutexLocker locker(lockEntities);
  if (!entities.contains(parentId))
  {
    throw ParentNotFound(entity->getType(), entity->getType(), parentId);
    return false;
  }

  if (entities.contains(entity->getUniqueId()))
  {
    throw EntityNotFound(entity->getType(), entity->getUniqueId());
    return false;
  }

  Entity *parent = entities[parentId];
  parent->addChild(entity);
  entities[entity->getUniqueId()] = entity;

  setDirty(true);
  return true;
}

bool Project::removeEntity(Entity* entity, bool appendChild)
     throw (EntityNotFound)
{
  Q_UNUSED(appendChild)

  assert(entity != nullptr);

  QMutexLocker locker(lockEntities);
  if (entities.contains(entity->getUniqueId()))
  {
    Entity *parent = entity->getParent();
    if (parent)
    {
      /*
      if (appendChild)
        parent->removeChildAppendChildren(entity);
      else
      */
      QStack <Entity*> stack;
      //remove all children
      stack.push(entity);
      while(stack.size())
      {
        Entity *currentEntity = stack.top();
        stack.pop();
        entities.remove(currentEntity->getUniqueId());

        QVector <Entity *> children = currentEntity->getChildren();
        for(int i = 0; i < children.size(); i++)
          stack.push(children.at(i));
      }

      // DELETE the entity and its children recursivelly
      parent->deleteChild(entity);
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
    throw EntityNotFound(entity->getType(), entity->getUniqueId());
    return false; // entity does not exist in the model
  }

  setDirty(true);
  return true;
}

/** \todo Return a string to be saved in hard disk. */
QString Project::toString()
{
  QString result = "";
  result += "#COMPOSER_PROJECT name=\"" + this->projectName
         + "\" version=\"0.1\"#\n";

  result += "#COMPOSER_MODEL#\n";
  result += Entity::toString(0);
  result += "#END_COMPOSER_MODEL#\n";

  for(const QString &key: pluginData.keys())
  {
   result += "#COMPOSER_PLUGIN_DATA "+ key + "#\n";
   result += pluginData[key];
   result += "\n#END_COMPOSER_PLUGIN_DATA#\n";
  }
  return result;
}

bool Project::setPluginData(const QString &pluginId,
                            const QByteArray &data)
{
  this->pluginData[pluginId] = data;

  setDirty(true);
  return true;
}

QByteArray Project::getPluginData(const QString &pluginId)
{
  if(pluginData.contains(pluginId))
  {
    return this->pluginData[pluginId];
  }
  return QByteArray();
}

bool Project::isDirty()
{
  return dirty;
}

void Project::setDirty(bool isDirty)
{
  // if(dirty != isDirty) {
    dirty = isDirty;
    emit dirtyProject(isDirty);
  // }
}

QString Project::generateUniqueNCLId(const QString &tagname)
{
  QList <Entity*> elements = getEntitiesbyType(tagname);
  QList <QString> currentElementsNCLID;
  for(int i = 0; i < elements.size(); i++)
  {
    if(elements.at(i)->hasAttribute("id"))
      currentElementsNCLID.push_back(elements.at(i)->getAttribute("id"));
  }

  for(int i = 1; ; i++)
  {
    QString retNCLID = tagname + QString::number(i);
    if(!currentElementsNCLID.contains(retNCLID))
      return retNCLID;
  }
}

QList<Entity*> Project::getEntityByAttrId(const QString &id)
{
  QMutexLocker locker(lockEntities);
  QMapIterator<QString, Entity*> it(entities);
  QList<Entity*> listRet;
  qCDebug(CPR_CORE) << "Project::getEntitiesbyType " << type;

  while(it.hasNext())
  {
    it.next();
    Entity* ent = it.value();
    if(ent->hasAttribute("id") && ent->getAttribute("id") == id)
      listRet.append(ent);
  }
  return listRet;
}

} } } //end namespace
