#include "Remove.h"

Remove::Remove(const QString &uid,
               const QString &parent,
               const QMap<QString, QString> &properties,
               const QMap<QString, QString> &settings,
               Command* command)
  : Command(command)
{
  this->_uid = uid;
  this->_parent = parent;
  this->_properties = properties;

  this->_settings = settings;
}

Remove::~Remove()
{

}

void Remove::undo()
{
  _settings.insert(STR_SETTING_UNDO,"0");
  _settings.insert(STR_SETTING_NOTIFY,"1");

  emit insert(_uid, _parent, _properties, _settings);
}

void Remove::redo()
{
  _settings.insert(STR_SETTING_UNDO,"0");
  _settings.insert(STR_SETTING_UNDO_TRACE,"1");

  emit remove(_uid, _settings);

}

