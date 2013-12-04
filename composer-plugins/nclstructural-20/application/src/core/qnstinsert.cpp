#include "qnstinsert.h"

QnstInsert::QnstInsert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings)
{
  this->uid = uid;
  this->parent = parent;
  this->properties = properties;

  this->settings = settings;
}

QnstInsert::~QnstInsert()
{

}

void QnstInsert::undo()
{
  settings["UNDO"] = "0";

  emit remove(uid, settings);
}

void QnstInsert::redo()
{
  settings["UNDO"] = "0";

  emit insert(uid, parent, properties, settings);
}
