#include "Insert.h"

Insert::Insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings)
{
  this->uid = uid;
  this->parent = parent;
  this->properties = properties;

  this->settings = settings;
}

Insert::~Insert()
{

}

void Insert::undo()
{
  settings["UNDO"] = "0";

  emit remove(uid, settings);
}

void Insert::redo()
{
  settings["UNDO"] = "0";

  emit insert(uid, parent, properties, settings);
}
