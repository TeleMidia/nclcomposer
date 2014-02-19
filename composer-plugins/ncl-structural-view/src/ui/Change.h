#ifndef QNSTCHANGECOMMAND_H
#define QNSTCHANGECOMMAND_H

#include <QUndoCommand>
#include <QMap>

#include "StructuralGraphicsEntity.h"
#include "StructuralView.h"

class StructuralView;

class Change : public QUndoCommand
{
public:
  Change(StructuralView* view, QString uid,
                    const QMap<QString,QString> &properties);

  ~Change();

  virtual void undo();

  virtual void redo();

private:
  bool ignore;

  QString uid;

  StructuralView* view;

  QMap<QString, QString> previous;

  QMap<QString, QString> next;
};
#endif // QNSTCHANGECOMMAND_H
