#include "Change.h"

Change::Change (const QString &uid, const QMap<QString, QString> &properties,
                const QMap<QString, QString> &previous,
                const QMap<QString, QString> &settings)
{
  _uid = uid;
  _properties = properties;
  _previous = previous;
  _settings = settings;
}

Change::~Change () {}

void
Change::undo ()
{
  _settings[STR_SETTINGS_UNDO] = STR_VALUE_FALSE;
  _settings[STR_SETTINGS_NOTIFY] = STR_VALUE_TRUE;

  emit change (_uid, _previous, _properties, _settings);
}

void
Change::redo ()
{
  _settings[STR_SETTINGS_UNDO] = STR_VALUE_FALSE;

  emit change (_uid, _properties, _previous, _settings);
}
