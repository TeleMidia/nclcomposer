#ifndef QNSTREMOVE_H
#define QNSTREMOVE_H

#include "qnstcommand.h"

class QnstRemove : public QnstCommand
{
public:
  QnstRemove(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings, QnstCommand* command = 0);
  virtual ~QnstRemove();

  virtual void undo();
  virtual void redo();

private:
  QString uid;
  QString parent;
  QMap<QString, QString> properties; QMap<QString, QString> settings;
};

#endif // QNSTREMOVE_H
