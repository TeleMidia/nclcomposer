#include "RemoveCommand.h"

QnstRemove::QnstRemove(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings, QnstCommand* command)
  : QnstCommand(command)
{
  this->uid = uid;
  this->parent = parent;
  this->properties = properties;

  this->settings = settings;
}

QnstRemove::~QnstRemove()
{

}

void QnstRemove::undo()
{
  settings["UNDO"] = "0";

  emit insert(uid, parent, properties, settings);
}

void QnstRemove::redo()
{
  settings["UNDO"] = "0";

  emit remove(uid, settings);

}

