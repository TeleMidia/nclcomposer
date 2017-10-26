#ifndef COMMAND_H
#define COMMAND_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QUndoCommand>

#include "Structural.h"

class StructuralView;

class Command : public QObject, public QUndoCommand
{
  Q_OBJECT

public:
  Command (StructuralView *view, Command *parent = 0);
  virtual ~Command () {}

  virtual void undo () = 0;
  virtual void redo () = 0;

protected:
  StructuralView *_view;

};

class Insert : public Command
{
public:
  Insert (StructuralView *view, const QString &uid, const QString &parent,
          const QMap<QString, QString> &props,
          const QMap<QString, QString> &settings);
  virtual ~Insert () {}

  virtual void undo ();
  virtual void redo ();

private:
  QString _uid;
  QString _parent;

  QMap<QString, QString> _properties;
  QMap<QString, QString> _settings;
};

class Remove : public Command
{
public:
  Remove (StructuralView *view, const QString &uid, const QString &parent,
          const QMap<QString, QString> &props,
          const QMap<QString, QString> &settings);
  virtual ~Remove () {}

  virtual void undo ();
  virtual void redo ();

private:
  QString _uid;
  QString _parent;

  QMap<QString, QString> _properties;
  QMap<QString, QString> _settings;
};

class Change : public Command
{
public:
  Change (StructuralView *view, const QString &uid,
          const QMap<QString, QString> &props,
          const QMap<QString, QString> &previous,
          const QMap<QString, QString> &settings);
  virtual ~Change () {}

  virtual void undo ();
  virtual void redo ();

private:
  QString _uid;

  QMap<QString, QString> _properties;
  QMap<QString, QString> _previous;
  QMap<QString, QString> _settings;
};

#endif // COMMAND_H
