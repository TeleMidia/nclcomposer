#include "Change.h"

Change::Change(const QString &uid,
               const QMap<QString, QString> &properties,
               const QMap<QString, QString> &previous,
               const QMap<QString, QString> &settings)
{
  this->_uid = uid;
  this->_properties = properties;
  this->_previous = previous;

  this->_settings = settings;
}

Change::~Change()
{

}

void Change::undo()
{
  _settings.insert(PLG_SETTING_UNDO,"0");
  _settings.insert(PLG_SETTING_NOTIFY,"1");
  _settings.insert("LOCATION","UNDO COMMAND");

  emit change(_uid, _previous, _properties, _settings);
}

void Change::redo()
{
  _settings.insert(PLG_SETTING_UNDO,"0");



  emit change(_uid, _properties, _previous, _settings);

  if(_settings.value("LOCATION") != "REDO COMMAND")
    _settings.insert("LOCATION", "REDO COMMAND");
}
