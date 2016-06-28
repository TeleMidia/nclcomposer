#ifndef REMOVE_H
#define REMOVE_H

#include "Command.h"

class Remove : public Command
{
public:
  Remove(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings, Command* command = 0);
  virtual ~Remove();

  virtual void undo();
  virtual void redo();

private:
  QString _uid;
  QString _parent;
  QMap<QString, QString> _properties; QMap<QString, QString> _settings;
};

#endif // REMOVE_H
