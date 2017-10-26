#include "Commands.h"
#include "StructuralView.h"

Command::Command (StructuralView *view, Command *parent)
  : QObject (parent), QUndoCommand (parent)
{
  _view = view;
}

Insert::Insert (StructuralView *view,
                const QString &uid, const QString &parent,
                const QMap<QString, QString> &props,
                const QMap<QString, QString> &settings)
  : Command (view)
{
  _uid = uid;
  _parent = parent;
  _properties = props;
  _settings = settings;
}

void
Insert::undo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _settings[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  _view->remove (_uid, _settings);
}

void
Insert::redo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;

  _view->insert (_uid, _parent, _properties, _settings);
}

Remove::Remove (StructuralView *view,
                const QString &uid, const QString &parent,
                const QMap<QString, QString> &props,
                const QMap<QString, QString> &settings)
  : Command (view)
{
  _uid = uid;
  _parent = parent;
  _properties = props;
  _settings = settings;
}

void
Remove::undo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _settings[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  _view->insert (_uid, _parent, _properties, _settings);
}

void
Remove::redo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _settings[ST_SETTINGS_UNDO_TRACE] = ST_VALUE_TRUE;

  _view->remove (_uid, _settings);
}

Change::Change (StructuralView *view,
                const QString &uid, const QMap<QString, QString> &props,
                const QMap<QString, QString> &previous,
                const QMap<QString, QString> &settings)
  : Command (view)
{
  _uid = uid;
  _properties = props;
  _previous = previous;
  _settings = settings;
}

void
Change::undo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _settings[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  _view->change (_uid, _previous, _properties, _settings);
}

void
Change::redo ()
{
  _settings[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;

  _view->change (_uid, _properties, _previous, _settings);
}
