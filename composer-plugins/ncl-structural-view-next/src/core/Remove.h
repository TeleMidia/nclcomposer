#ifndef QNSTREMOVE_H
#define QNSTREMOVE_H

#include "Command.h"

class Remove : public Command
{
public:
  Remove(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings, Command* command = 0);
  virtual ~Remove();

  virtual void undo();
  virtual void redo();

private:
  QString uid;
  QString parent;
  QMap<QString, QString> properties; QMap<QString, QString> settings;
};

#endif // QNSTREMOVE_H
