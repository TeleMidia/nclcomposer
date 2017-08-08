#ifndef INSERT_H
#define INSERT_H

#include "Command.h"

class Insert : public Command
{
public:
  Insert (const QString &uid, const QString &parent,
          const QMap<QString, QString> &properties,
          const QMap<QString, QString> &settings);
  virtual ~Insert ();

  virtual void undo ();
  virtual void redo ();

private:
  QString _uid;
  QString _parent;

  QMap<QString, QString> _properties;
  QMap<QString, QString> _settings;
};

#endif // INSERT_H
