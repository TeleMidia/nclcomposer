#ifndef QNSTREMOVE_H
#define QNSTREMOVE_H

#include "Command.h"

class Remove : public Command
{
public:
  Remove(QString _uid, QString _parent, QMap<QString, QString> _properties, QMap<QString, QString> _settings, Command* command = 0);
  virtual ~Remove();

  virtual void undo();
  virtual void redo();

private:
  QString _uid;
  QString _parent;
  QMap<QString, QString> _properties; QMap<QString, QString> _settings;
};

#endif // QNSTREMOVE_H
