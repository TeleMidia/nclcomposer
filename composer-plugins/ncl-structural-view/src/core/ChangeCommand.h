#ifndef QNSTCHANGE_H
#define QNSTCHANGE_H

#include "Command.h"

class QnstChange : public QnstCommand
{
public:
  QnstChange(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  virtual ~QnstChange();

  virtual void undo();
  virtual void redo();

private:
  QString uid;
  QString parent;
  QMap<QString, QString> properties; QMap<QString, QString> previous; QMap<QString, QString> settings;
};

#endif // QNSTCHANGE_H
