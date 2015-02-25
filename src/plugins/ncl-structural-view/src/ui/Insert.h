#ifndef QNSTADDCOMMAND_H
#define QNSTADDCOMMAND_H

#include <QUndoCommand>

#include "StructuralGraphicsEntity.h"
#include "StructuralView.h"

class StructuralView;

class Insert : public QUndoCommand
{
public:
  Insert(StructuralView* view, StructuralGraphicsEntity* entity);

  ~Insert();

  virtual void undo();

  virtual void redo();

private:
  void copy(StructuralGraphicsEntity* entity);

  bool ignore;

  QString parent;

  StructuralView* view;

  StructuralGraphicsEntity* entity;
};

#endif // QNSTADDCOMMAND_H
