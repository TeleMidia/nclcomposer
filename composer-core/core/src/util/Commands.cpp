/* Copyright (c) 2011-2012 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "util/Commands.h"

#include "modules/MessageControl.h"
#include "modules/ProjectControl.h"

namespace composer {
  namespace core {
    namespace util {

EditCommand::EditCommand(Project *project, Entity *entity,
                         QMap <QString, QString> newAttrs, QUndoCommand *parent)
  : QUndoCommand(parent)
{
  this->project = project;

  this->uniqueId = entity->getUniqueId();
  QMap <QString, QString>::iterator begin, end, it;
  entity->getAttributeIterator(begin, end);
  for (it = begin; it != end; ++it)
      attrs[it.key()] = it.value();
  this->newAttrs = newAttrs;

  msgControl = PluginControl::getInstance()->getMessageControl(project);
}

void EditCommand::undo()
{
  msgControl->anonymousChangeEntity(this->uniqueId, this->attrs);
}

void EditCommand::redo()
{
  msgControl->anonymousChangeEntity(this->uniqueId, this->newAttrs);
}

RemoveCommand::RemoveCommand(Project *project, Entity *entity,
                             QUndoCommand *parent)
{
  this->parentUniqueId = entity->getParentUniqueId();
  this->entity = entity;
  this->project = project;

  msgControl = PluginControl::getInstance()->getMessageControl(project);

  first = true;
}

void RemoveCommand::undo()
{
  msgControl->anonymousAddEntity(this->entity, parentUniqueId);

  // I have to do this because the core is responsible to Remove the entity.
  this->entity = entity->cloneEntity();

  first = false;

  // \todo Undo every child of the entity (recursively).
}

void RemoveCommand::redo()
{
  //I have to do this because the core is responsible to Remove the entity.
  Entity *entityTmp = this->entity->cloneEntity();
  if(first)
    project->removeEntity(this->entity, false);
  else
    msgControl->anonymousRemoveEntity(this->entity->getUniqueId());

  this->entity = entityTmp;
}

AddCommand::AddCommand(Project *project, Entity *entity, QString parentUniqueId,
                       QUndoCommand *parent)
{
  this->parentUniqueId = parentUniqueId;
  this->entity = entity;
  this->project = project;

  first = true;

  msgControl = PluginControl::getInstance()->getMessageControl(project);
}

void AddCommand::undo()
{
  this->entity = this->entity->cloneEntity();

  msgControl->anonymousRemoveEntity(this->entity->getUniqueId());

  first = false;
}

void AddCommand::redo()
{
  //I have to do this because the core is responsible to Remove the entity.
  Entity *entityTmp = this->entity->cloneEntity();
  //TODO - calll validator to check
  if(first)
    project->addEntity(this->entity, parentUniqueId);
  else
    msgControl->anonymousAddEntity(this->entity, parentUniqueId);

  this->entity = entityTmp;
}

} } } //end namespace
