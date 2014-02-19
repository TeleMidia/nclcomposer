#ifndef QNSTCHANGE_H
#define QNSTCHANGE_H

#include "Command.h"

class Change : public Command
{
public:
  Change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  virtual ~Change();

  virtual void undo();
  virtual void redo();

private:
  QString uid;
  QString parent;
  QMap<QString, QString> properties; QMap<QString, QString> previous; QMap<QString, QString> settings;
};

#endif // QNSTCHANGE_H
