#include "Remove.h"

Remove::Remove(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings, Command* command)
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
  _settings.insert(PLG_SETTING_UNDO,"0");
  _settings.insert(PLG_SETTING_NOTIFY,"1");

  emit insert(_uid, _parent, _properties, _settings);
}

void Remove::redo()
{
  _settings.insert(PLG_SETTING_UNDO,"0");
  _settings.insert(PLG_SETTING_UNDO_CHILDREN,"1");

  emit remove(_uid, _settings);

}

