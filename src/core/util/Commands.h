/* Copyright (C) 2011-2018 PUC-Rio/TeleMídia Lab.

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

#ifndef COMMANDS_H
#define COMMANDS_H
#include <QUndoCommand>

#include "model/Project.h"

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief An internal command of Composer Core to edit an Entity.
 */
class EditEntityCmd : public QUndoCommand
{
public:
  EditEntityCmd (Project *project, Entity *entity,
                 QMap<QString, QString> newAttrs, QUndoCommand *parent = 0);

  void undo ();
  void redo ();

private:
  Project *_project;
  QString _uniqueId;
  QMap<QString, QString> _attrs, _newAttrs;
  MessageControl *_msgControl;
};

/*!
 * \brief An internal command of Composer Core to remove an Entity.
 */
class RemoveEntityCmd : public QUndoCommand
{
public:
  RemoveEntityCmd (Project *project, Entity *entity, QUndoCommand *parent = 0);

  void undo ();
  void redo ();

private:
  Project *_project;
  Entity *_entity;
  QString _parentUniqueId;
  MessageControl *_msgControl;
  bool _first;
};

/*!
 * \brief An internal command of Composer Coreto add an Entity.
 */
class AddEntityCmd : public QUndoCommand
{
public:
  AddEntityCmd (Project *_project, Entity *_entity, QString _parentUniqueId,
                QUndoCommand *parent = 0);

  void undo ();
  void redo ();

private:
  Project *_project;
  Entity *_entity;
  QString _parentUniqueId;
  MessageControl *_msgControl;
  bool _first;
};

/*!
 * \brief An internal command of Composer Coreto add an Entity.
 */
class AddCommentCmd : public QUndoCommand
{
  friend class Project;

public:
  AddCommentCmd (Project *_project, Comment *comment, QString parentUniqueId,
                 QUndoCommand *parent = 0);

  void undo ();
  void redo ();

private:
  Project *_project;
  Comment *_comment;
  QString _parentUniqueId;
  MessageControl *_msgControl;
  bool _first;
};

CPR_CORE_END_NAMESPACE

#endif // COMMANDS_H
