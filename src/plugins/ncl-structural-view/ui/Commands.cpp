#include "Commands.h"
#include "StructuralView.h"

Command::Command (StructuralView *view, Command *parent)
    : QObject (parent), QUndoCommand (parent)
{
  _view = view;
}

Insert::Insert (StructuralView *view, const QString &uid,
                const QString &parent, const QMap<QString, QString> &props,
                const QMap<QString, QString> &settings)
    : Command (view)
{
  _uid = uid;
  _parent = parent;
  _props = props;
  _stgs = settings;
}

void
Insert::undo ()
{
  _stgs[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _stgs[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  _view->remove (_uid, _stgs);
}

void
Insert::redo ()
{
  _stgs[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;

  _view->insert (_uid, _parent, _props, _stgs);
}

Remove::Remove (StructuralView *view, const QString &uid,
                const QString &parent, const QMap<QString, QString> &props,
                const QMap<QString, QString> &settings)
    : Command (view)
{
  _uid = uid;
  _parent = parent;
  _props = props;
  _stgs = settings;
}

void
Remove::undo ()
{
  _stgs[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _stgs[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  _view->insert (_uid, _parent, _props, _stgs);
}

void
Remove::redo ()
{
  _stgs[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _stgs[ST_SETTINGS_UNDO_TRACE] = ST_VALUE_TRUE;

  _view->remove (_uid, _stgs);
}

Change::Change (StructuralView *view, const QString &uid,
                const QMap<QString, QString> &props,
                const QMap<QString, QString> &previous,
                const QMap<QString, QString> &settings)
    : Command (view)
{
  _uid = uid;
  _props = props;
  _previous = previous;
  _stgs = settings;
}

void
Change::undo ()
{
  _stgs[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;
  _stgs[ST_SETTINGS_NOTIFY] = ST_VALUE_TRUE;

  _view->change (_uid, _previous, _stgs);
}

void
Change::redo ()
{
  _stgs[ST_SETTINGS_UNDO] = ST_VALUE_FALSE;

  _view->change (_uid, _props, _stgs);
}
