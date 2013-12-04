#include "qnstcommand.h"

QnstCommand::QnstCommand(QnstCommand* parent)
  : QObject(parent), QUndoCommand(parent)
{

}

QnstCommand::~QnstCommand()
{

}
