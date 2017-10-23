#include "Commands.h"

Command::Command (Command *parent) : QObject (parent), QUndoCommand (parent)
{
  // Nothing todo.
}

Insert::Insert (const QString &uid, const QString &parent,
                const QMap<QString, QString> &properties,
                const QMap<QString, QString> &settings)
{
  _uid = uid;
  _parent = parent;
  _properties = properties;
  _settings = settings;
}

void
Insert::undo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _settings[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  emit remove (_uid, _settings);
}

void
Insert::redo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;

  emit insert (_uid, _parent, _properties, _settings);
}

Remove::Remove (const QString &uid, const QString &parent,
                const QMap<QString, QString> &properties,
                const QMap<QString, QString> &settings)
{
  _uid = uid;
  _parent = parent;
  _properties = properties;
  _settings = settings;
}

void
Remove::undo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _settings[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  emit insert (_uid, _parent, _properties, _settings);
}

void
Remove::redo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _settings[ST_SETTINGS_UNDO_TRACE] = ST_VALUE_TRUE;

  emit remove (_uid, _settings);
}

Change::Change (const QString &uid, const QMap<QString, QString> &properties,
                const QMap<QString, QString> &previous,
                const QMap<QString, QString> &settings)
{
  _uid = uid;
  _properties = properties;
  _previous = previous;
  _settings = settings;
}

void
Change::undo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _settings[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  emit change (_uid, _previous, _properties, _settings);
}

void
Change::redo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;

  emit change (_uid, _properties, _previous, _settings);
}
