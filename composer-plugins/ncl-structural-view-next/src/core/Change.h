#ifndef QNSTCHANGE_H
#define QNSTCHANGE_H

#include "Command.h"

class Change : public Command
{
public:
  Change(QString _uid, QMap<QString, QString> _properties, QMap<QString, QString> _previous, QMap<QString, QString> _settings);
  virtual ~Change();

  virtual void undo();
  virtual void redo();

private:
  QString _uid;
  QString _parent;
  QMap<QString, QString> _properties; QMap<QString, QString> _previous; QMap<QString, QString> _settings;
};

#endif // QNSTCHANGE_H
