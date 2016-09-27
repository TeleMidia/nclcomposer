#include "Remove.h"

Remove::Remove(const QString &uid, const QString &parent, const QMap<QString, QString> &properties, const QMap<QString, QString> &settings)
{
  _uid = uid;
  _parent = parent;
  _properties = properties;
  _settings = settings;
}

Remove::~Remove()
{

}

void Remove::undo()
{
  _settings[STR_SETTING_UNDO] = STR_VALUE_FALSE;
  _settings[STR_SETTING_NOTIFY] = STR_VALUE_TRUE;

  emit insert(_uid, _parent, _properties, _settings);
}

void Remove::redo()
{
  _settings[STR_SETTING_UNDO] = STR_VALUE_FALSE;
  _settings[STR_SETTING_UNDO_TRACE] = STR_VALUE_TRUE;

  emit remove(_uid, _settings);

}
