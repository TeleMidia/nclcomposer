#include "StructuralMenu.h"

#include <QDebug>

StructuralMenu::StructuralMenu (QWidget *parent) : QMenu (parent) {}

void
StructuralMenu::exec (const QPoint &screenPos, const StructuralEntity *e)
{
  StructuralType t = Structural::NoType;
  if (e)
    t = e->structuralType ();
  adjust (t);

  if (e)
  {
    QMap<QString, QString> props = e->properties ();
    if (!ST_OPT_SHOW_INTERFACES)
    {
      setAutostartEnabled (true);

      if (props.value (ST_ATTR_ENT_AUTOSTART) == ST_VALUE_TRUE)
        setAutoStartChecked (true);
      else
        setAutoStartChecked (false);
    }
    else if (!ST_OPT_WITH_BODY && !ST_OPT_USE_FLOATING_INTERFACES)
    {
      if ((e->structuralParent () != nullptr
           && e->structuralParent ()->structuralParent () == nullptr
           && e->category () == Structural::Interface)
          || (e->structuralParent () == nullptr))
      {
        setAutostartEnabled (true);

        if (props.value (ST_ATTR_ENT_AUTOSTART) == ST_VALUE_TRUE)
          setAutoStartChecked (true);
        else
          setAutoStartChecked (false);
      }
    }
  }

  QMenu::exec (screenPos);
}

void
StructuralMenu::adjust (StructuralType type)
{
  switch (type)
  {
    case Structural::Media:
    {
      setAutostartEnabled (false);
      break;
    }

    case Structural::Body:
    case Structural::Context:
    {
      setAutostartEnabled (false);
      break;
    }

    case Structural::Switch:
    {
      setAutostartEnabled (false);
      break;
    }

    case Structural::Area:
    case Structural::Property:
    case Structural::Port:
    case Structural::SwitchPort:
    {
      setAutostartEnabled (false);
      break;
    }

    default:
    {
      setAutostartEnabled (false);
      break;
    }
  }
}

void
StructuralMenu::setAutostartEnabled (bool state)
{
  foreach (QAction *act, actions ())
  {
    if (act->text () == "Set as starting node")
    {
      act->setEnabled (state);
    }
  }
}

void
StructuralMenu::setAutoStartChecked (bool state)
{
  foreach (QAction *act, actions ())
  {
    if (act->text () == "Set as starting node")
    {
      act->setChecked (state);
    }
  }
}
