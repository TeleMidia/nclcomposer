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

#ifndef COMMANDS_H
#define COMMANDS_H
#include <QUndoCommand>

#include "model/Project.h"

namespace composer {
  namespace core {
    namespace util {

/*!
 * \brief An internal command of Composer Core to edit an Entity.
 */
class EditCommand : public QUndoCommand
{
public:
  EditCommand( Project *project, Entity *entity,
               QMap <QString, QString> newAttrs, QUndoCommand *parent = 0 );

  void undo();
  void redo();

private:
  Project *project;
  QString uniqueId;
  QMap <QString, QString> attrs, newAttrs;
  MessageControl *msgControl;
};

/*!
 * \brief An internal command of Composer Core to remove an Entity.
 */
class RemoveCommand : public QUndoCommand
{
public:
  RemoveCommand(Project *project, Entity *entity, QUndoCommand *parent = 0);

  void undo();
  void redo();

private:
  Project *project;
  Entity *entity;
  QString parentUniqueId;
  MessageControl *msgControl;
  bool first;
};

/*!
 * \brief An internal command of Composer Coreto add an Entity.
 */
class AddCommand : public QUndoCommand
{
public:
  AddCommand(Project *project, Entity *entity, QString parentUniqueId,
             QUndoCommand *parent = 0);

  void undo();
  void redo();

private:
  Project *project;
  Entity *entity;
  QString parentUniqueId;
  MessageControl *msgControl;
  bool first;
};

//QString createCommandString(DiagramItem *item, const QPointF &point);

} } } //end namespace
#endif // COMMANDS_H
