#include "Remove.h"

Remove::Remove(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings, Command* command)
  : Command(command)
{
  this->uid = uid;
  this->parent = parent;
  this->properties = properties;

  this->settings = settings;
}

Remove::~Remove()
{

}

void Remove::undo()
{
  settings["UNDO"] = "0";

  emit insert(uid, parent, properties, settings);
}

void Remove::redo()
{
  settings["UNDO"] = "0";

  emit remove(uid, settings);

}

