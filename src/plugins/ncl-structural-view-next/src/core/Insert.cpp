#include "Insert.h"

Insert::Insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings)
{
  this->_uid = uid;
  this->_parent = parent;
  this->_properties = properties;

  this->_settings = settings;
}

Insert::~Insert()
{

}

void Insert::undo()
{
  _settings.insert(PLG_SETTING_UNDO, "0");
  _settings.insert(PLG_SETTING_NOTIFY,"1");

  emit remove(_uid, _settings);
}

void Insert::redo()
{
  _settings.insert(PLG_SETTING_UNDO,"0");

  emit insert(_uid, _parent, _properties, _settings);
}
