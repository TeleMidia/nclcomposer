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

#include "util/Commands.h"

#include "modules/MessageControl.h"
#include "modules/ProjectControl.h"

CPR_CORE_BEGIN_NAMESPACE

EditCommand::EditCommand (Project *project, Entity *entity,
                          QMap<QString, QString> newAttrs,
                          QUndoCommand *parent)
    : QUndoCommand (parent)
{
  this->_project = project;

  this->_uniqueId = entity->getUniqueId ();
  _attrs = entity->getAttributes ();
  this->_newAttrs = newAttrs;

  _msgControl = PluginControl::getInstance ()->getMessageControl (project);
}

void
EditCommand::undo ()
{
  _msgControl->anonymousChangeEntity (this->_uniqueId, this->_attrs);
}

void
EditCommand::redo ()
{
  _msgControl->anonymousChangeEntity (this->_uniqueId, this->_newAttrs);
}

RemoveCommand::RemoveCommand (Project *project, Entity *entity,
                              QUndoCommand *parent)
{
  Q_UNUSED (parent)

  this->_parentUniqueId = entity->getParentUniqueId ();
  this->_entity = entity;
  this->_project = project;

  _msgControl = PluginControl::getInstance ()->getMessageControl (project);

  _first = true;
}

void
RemoveCommand::undo ()
{
  _msgControl->anonymousAddEntity (this->_entity, _parentUniqueId);

  // I have to do this because the core is responsible to Remove the entity.
  this->_entity = _entity->cloneEntity ();

  _first = false;

  // \todo Undo every child of the entity (recursively).
}

void
RemoveCommand::redo ()
{
  // I have to do this because the core is responsible to Remove the entity.
  Entity *entityTmp = this->_entity->cloneEntity ();
  if (_first)
    _project->removeEntity (this->_entity, false);
  else
    _msgControl->anonymousRemoveEntity (this->_entity->getUniqueId ());

  this->_entity = entityTmp;
}

AddCommand::AddCommand (Project *project, Entity *entity,
                        QString parentUniqueId, QUndoCommand *parent)
{
  Q_UNUSED (parent)

  this->_parentUniqueId = parentUniqueId;
  this->_entity = entity;
  this->_project = project;

  _first = true;

  _msgControl = PluginControl::getInstance ()->getMessageControl (project);
}

void
AddCommand::undo ()
{
  this->_entity = this->_entity->cloneEntity ();

  _msgControl->anonymousRemoveEntity (this->_entity->getUniqueId ());

  _first = false;
}

void
AddCommand::redo ()
{
  // I have to do this because the core is responsible to Remove the entity.
  Entity *entityTmp = this->_entity->cloneEntity ();
  // TODO - calll validator to check
  if (_first)
    _project->addEntity (this->_entity, _parentUniqueId);
  else
    _msgControl->anonymousAddEntity (this->_entity, _parentUniqueId);

  this->_entity = entityTmp;
}

CPR_CORE_END_NAMESPACE
