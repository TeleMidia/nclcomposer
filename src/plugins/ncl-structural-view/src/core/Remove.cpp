#include "Remove.h"

Remove::Remove (const QString &uid, const QString &parent,
                const QMap<QString, QString> &properties,
                const QMap<QString, QString> &settings)
{
  _uid = uid;
  _parent = parent;
  _properties = properties;
  _settings = settings;
}

Remove::~Remove () {}

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
