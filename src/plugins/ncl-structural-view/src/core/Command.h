#ifndef COMMAND_H
#define COMMAND_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QUndoCommand>

#include "Structural.h"

class Command : public QObject, public QUndoCommand
{
  Q_OBJECT

public:
  Command (Command *parent = 0);
  virtual ~Command ();

  virtual void undo () = 0;
  virtual void redo () = 0;

signals:
  void insert (QString uid, QString parent, QMap<QString, QString> properties,
               QMap<QString, QString> settings);
  void remove (QString uid, QMap<QString, QString> settings);
  void change (QString uid, QMap<QString, QString> properties,
               QMap<QString, QString> previous,
               QMap<QString, QString> settings);
  void select (QString uid, QMap<QString, QString> settings);
};

class Insert : public Command
{
public:
  Insert (const QString &uid, const QString &parent,
          const QMap<QString, QString> &properties,
          const QMap<QString, QString> &settings);
  virtual ~Insert ();

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
  Remove (const QString &uid, const QString &parent,
          const QMap<QString, QString> &properties,
          const QMap<QString, QString> &settings);
  virtual ~Remove ();

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
  Change (const QString &uid, const QMap<QString, QString> &properties,
          const QMap<QString, QString> &previous,
          const QMap<QString, QString> &settings);
  virtual ~Change ();

  virtual void undo ();
  virtual void redo ();

private:
  QString _uid;

  QMap<QString, QString> _properties;
  QMap<QString, QString> _previous;
  QMap<QString, QString> _settings;
};

#endif // COMMAND_H
