#include "Command.h"

Command::Command(Command* parent)
  : QObject(parent), QUndoCommand(parent)
{

}

Command::~Command()
{

}
