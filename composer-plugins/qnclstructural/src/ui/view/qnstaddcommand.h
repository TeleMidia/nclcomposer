#ifndef QNSTADDCOMMAND_H
#define QNSTADDCOMMAND_H

#include <QUndoCommand>

#include "qnstgraphicsentity.h"
#include "qnstview.h"

class QnstView;

class QnstAddCommand : public QUndoCommand
{
public:
  QnstAddCommand(QnstView* view, QnstGraphicsEntity* entity);

  ~QnstAddCommand();

  virtual void undo();

  virtual void redo();

private:
  void copy(QnstGraphicsEntity* entity);

  bool ignore;

  QString parent;

  QnstView* view;

  QnstGraphicsEntity* entity;
};

#endif // QNSTADDCOMMAND_H
