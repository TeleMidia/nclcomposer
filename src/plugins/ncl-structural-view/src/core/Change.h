#ifndef CHANGE_H
#define CHANGE_H

#include "Command.h"

class Change : public Command
{
public:
  Change (const QString &uid, const QMap<QString, QString> &properties,
          const QMap<QString, QString> &previous,
          const QMap<QString, QString> &settings);
  virtual ~Change ();

  virtual void undo ();
  virtual void redo ();

private:
  QString _uid;

  QMap<QString, QString> _properties;
  QMap<QString, QString> _previous;
  QMap<QString, QString> _settings;
};

#endif // CHANGE_H
