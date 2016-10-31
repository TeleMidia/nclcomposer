#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>
#include <QObject>
#include <QString>
#include <QMap>

#include "Structural.h"

class Command : public QObject, public QUndoCommand
{
  Q_OBJECT

public:
  Command(Command* parent = 0);
  virtual ~Command();

  virtual void undo() = 0;
  virtual void redo() = 0;

signals:
  void insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void remove(QString uid, QMap<QString, QString> settings);
  void change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void select(QString uid, QMap<QString, QString> settings);
};

#endif // COMMAND_H
