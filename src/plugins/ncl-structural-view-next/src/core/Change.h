#ifndef CHANGE_H
#define CHANGE_H

#include "Command.h"

class Change : public Command
{
public:
  Change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  virtual ~Change();

  virtual void undo();
  virtual void redo();

private:
  QString _uid;
  QString _parent;
  QMap<QString, QString> _properties; QMap<QString, QString> _previous; QMap<QString, QString> _settings;
};

#endif // CHANGE_H
