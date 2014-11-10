#include "Change.h"

Change::Change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings)
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
  _settings["UNDO"] = "0";
  _settings["NOTIFY"] = "1";

  emit change(_uid, _previous, _properties, _settings);
}

void Change::redo()
{
  _settings["UNDO"] = "0";

  emit change(_uid, _properties, _previous, _settings);
}
