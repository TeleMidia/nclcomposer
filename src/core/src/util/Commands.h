/* Copyright (c) 2011-2012 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
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
