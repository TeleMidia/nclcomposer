#include "ChangeCommand.h"

QnstChange::QnstChange(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings)
{
  this->uid = uid;
  this->parent = parent;
  this->properties = properties;
  this->previous = previous;

  this->settings = settings;
}

QnstChange::~QnstChange()
{

}

void QnstChange::undo()
{
  settings["UNDO"] = "0";

  emit change(uid, previous, properties, settings);
}

void QnstChange::redo()
{
  settings["UNDO"] = "0";

  emit change(uid, properties, previous, settings);
}
