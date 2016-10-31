#include "Command.h"

Command::Command(Command* parent)
  : QObject(parent), QUndoCommand(parent)
{
  // Nothing todo.
}

Command::~Command()
{
  // Nothing todo.
}
