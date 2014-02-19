#ifndef QNSTREMOVECOMMAND_H
#define QNSTREMOVECOMMAND_H

#include <QUndoCommand>

#include "qnstgraphicsentity.h"
#include "qnstview.h"

class QnstView;

class QnstRemoveCommand : public QUndoCommand
{
public:
  QnstRemoveCommand(QnstView* view, QnstGraphicsEntity* entity);

  ~QnstRemoveCommand();

  virtual void undo();

  virtual void redo();

private:
  void copy(QnstGraphicsEntity* entity);

  void copy(QnstGraphicsEntity* entity, QnstGraphicsEntity* parent);

  void paste(QnstGraphicsEntity* entity, QnstGraphicsEntity* parent);

  bool ignore;

  QString parent;

  QnstView* view;

  QnstGraphicsEntity* entity;
};


#endif // QNSTREMOVECOMMAND_H
