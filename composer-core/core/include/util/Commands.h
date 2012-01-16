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

//! [0]
class EditCommand : public QUndoCommand
{
public:
    EditCommand(Project *project, Entity *entity,
                QMap <QString, QString> newAttrs, QUndoCommand *parent = 0);

    void undo();
    void redo();

private:
    Project *project;
    QString uniqueId;
    QMap <QString, QString> attrs, newAttrs;
    MessageControl *msgControl;
};
//! [0]

//! [1]
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
//! [1]

//! [2]
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
//! [2]

//QString createCommandString(DiagramItem *item, const QPointF &point);

} } } //end namespace
#endif // COMMANDS_H
