#include "Insert.h"

Insert::Insert(const QString &uid,
               const QString &parent,
               const QMap<QString, QString> &properties,
               const QMap<QString, QString> &settings)
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
