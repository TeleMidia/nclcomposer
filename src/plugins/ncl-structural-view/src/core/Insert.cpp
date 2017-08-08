#include "Insert.h"

Insert::Insert (const QString &uid, const QString &parent,
                const QMap<QString, QString> &properties,
                const QMap<QString, QString> &settings)
{
  _uid = uid;
  _parent = parent;
  _properties = properties;
  _settings = settings;
}

Insert::~Insert () {}

void
Insert::undo ()
{
  _settings[STR_SETTING_UNDO] = STR_VALUE_FALSE;
  _settings[STR_SETTING_NOTIFY] = STR_VALUE_TRUE;

  emit remove (_uid, _settings);
}

void
Insert::redo ()
{
  _settings[STR_SETTING_UNDO] = STR_VALUE_FALSE;

  emit insert (_uid, _parent, _properties, _settings);
}
