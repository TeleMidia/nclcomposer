#ifndef COMMAND_H
#define COMMAND_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QUndoCommand>

#include "Structural.h"

class StructuralScene;

class Command : public QObject, public QUndoCommand
{
  Q_OBJECT

public:
  Command (StructuralScene *scn, Command *parent = 0);
  virtual ~Command () {}

  virtual void undo () = 0;
  virtual void redo () = 0;

protected:
  StructuralScene *_scn;
};

class Insert : public Command
{
public:
  Insert (StructuralScene *scn, const QString &uid, const QString &parent,
          const QMap<QString, QString> &props,
          const QMap<QString, QString> &settings);
  virtual ~Insert () {}

  virtual void undo ();
  virtual void redo ();

private:
  QString _uid, _parent;

  QStrMap _props, _stgs;
};

class Remove : public Command
{
public:
  Remove (StructuralScene *scn, const QString &uid, const QString &parent,
          const QMap<QString, QString> &props,
          const QMap<QString, QString> &settings);
  virtual ~Remove () {}

  virtual void undo ();
  virtual void redo ();

private:
  QString _uid, _parent;

  QStrMap _props, _stgs;
};

class Change : public Command
{
public:
  Change (StructuralScene *scn, const QString &uid,
          const QMap<QString, QString> &props,
          const QMap<QString, QString> &previous,
          const QMap<QString, QString> &settings);
  virtual ~Change () {}

  virtual void undo ();
  virtual void redo ();

private:
  QString _uid;
  QStrMap _props, _previous, _stgs;
};

#endif // COMMAND_H
