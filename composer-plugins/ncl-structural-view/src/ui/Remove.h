#ifndef QNSTREMOVECOMMAND_H
#define QNSTREMOVECOMMAND_H

#include <QUndoCommand>

#include "StructuralGraphicsEntity.h"
#include "StructuralView.h"

class StructuralView;

class Remove : public QUndoCommand
{
public:
  Remove(StructuralView* view, StructuralGraphicsEntity* entity);

  ~Remove();

  virtual void undo();

  virtual void redo();

private:
  void copy(StructuralGraphicsEntity* entity);

  void copy(StructuralGraphicsEntity* entity, StructuralGraphicsEntity* parent);

  void paste(StructuralGraphicsEntity* entity, StructuralGraphicsEntity* parent);

  bool ignore;

  QString parent;

  StructuralView* view;

  StructuralGraphicsEntity* entity;
};


#endif // QNSTREMOVECOMMAND_H
