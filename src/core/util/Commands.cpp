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

EditEntityCmd::EditEntityCmd (Project *project, Entity *entity,
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
EditEntityCmd::undo ()
{
  _msgControl->anonymousChangeEntity (this->_uniqueId, this->_attrs);
}

void
EditEntityCmd::redo ()
{
  _msgControl->anonymousChangeEntity (this->_uniqueId, this->_newAttrs);
}

RemoveEntityCmd::RemoveEntityCmd (Project *project, Entity *entity,
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
RemoveEntityCmd::undo ()
{
  _msgControl->anonymousAddEntity (this->_entity, _parentUniqueId);

  // I have to do this because the core is responsible to Remove the entity.
  this->_entity = _entity->clone ();

  _first = false;

  // \todo Undo every child of the entity (recursively).
}

void
RemoveEntityCmd::redo ()
{
  // I have to do this because the core is responsible to Remove the entity.
  Entity *entityTmp = this->_entity->clone ();
  if (_first)
    _project->removeEntity (this->_entity, false);
  else
    _msgControl->anonymousRemoveEntity (this->_entity->getUniqueId ());

  this->_entity = entityTmp;
}

AddEntityCmd::AddEntityCmd (Project *project, Entity *entity,
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
AddEntityCmd::undo ()
{
  this->_entity = this->_entity->clone ();

  _msgControl->anonymousRemoveEntity (this->_entity->getUniqueId ());

  _first = false;
}

void
AddEntityCmd::redo ()
{
  // I have to do this because the core is responsible to Remove the entity.
  Entity *entityTmp = this->_entity->clone ();
  // TODO - calll validator to check
  if (_first)
    _project->addEntity (this->_entity, _parentUniqueId);
  else
    _msgControl->anonymousAddEntity (this->_entity, _parentUniqueId);

  this->_entity = entityTmp;
}

AddCommentCmd::AddCommentCmd (Project *project, Comment *comment,
                              QString parentUniqueId, QUndoCommand *parent)
{
  Q_UNUSED (parent)

  this->_parentUniqueId = parentUniqueId;
  this->_comment = comment;
  this->_project = project;

  _first = true;

  _msgControl = PluginControl::getInstance ()->getMessageControl (project);
}

void
AddCommentCmd::undo ()
{
//  this->_entity = this->_entity->cloneEntity ();
//  _msgControl->anonymousRemoveEntity (this->_entity->getUniqueId ());
//  _first = false;
}

void
AddCommentCmd::redo ()
{
//  // I have to do this because the core is responsible to Remove the entity.
//  Entity *entityTmp = this->_entity->cloneEntity ();
//  // TODO - calll validator to check
//  if (_first)
//    _project->addEntity (this->_comment, _parentUniqueId);
//  else
//    _msgControl->anonymousAddEntity (this->_comment, _parentUniqueId);

//  this->_entity = entityTmp;
}

CPR_CORE_END_NAMESPACE
