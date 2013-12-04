#ifndef QNSTINSERT_H
#define QNSTINSERT_H

#include "qnstcommand.h"

class QnstInsert : public QnstCommand
{
public:
  QnstInsert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  virtual ~QnstInsert();

  virtual void undo();
  virtual void redo();

private:
  QString uid;
  QString parent;
  QMap<QString, QString> properties;

  QMap<QString, QString> settings;
};


#endif // QNSTINSERT_H
