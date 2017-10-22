#include "StructuralMenu.h"

StructuralMenu::StructuralMenu (QWidget *parent) :
  QMenu (parent)
{

}

void
StructuralMenu::exec (const QPoint &screenPos, const StructuralEntity *e)
{
  StructuralType t = Structural::NoType;
  if (e)
  {
    QMap <QString, QString> props = e->getProperties();
    t = e->getType();

    if (!STR_DEFAULT_WITH_INTERFACES)
    {
      setAutostartEnabled (true);

      if (props.value (STR_PROPERTY_ENTITY_AUTOSTART) == STR_VALUE_TRUE)
        setAutoStartChecked (true);
      else
        setAutoStartChecked (false);
    }
    else if (!STR_DEFAULT_WITH_BODY &&
             !STR_DEFAULT_WITH_FLOATING_INTERFACES)
    {
      if ((e->getParent() != nullptr
           && e->getParent()->getParent() == nullptr
           && e->getCategory() == Structural::Interface)
          || (e->getParent() == nullptr))
      {
        setAutostartEnabled (true);

        if (props.value (STR_PROPERTY_ENTITY_AUTOSTART) == STR_VALUE_TRUE)
          setAutoStartChecked (true);
        else
          setAutoStartChecked (false);
      }
    }
  }

  adjust (t);
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
  foreach (QAction *act, actions())
  {
    if (act->text() == "Set as starting node")
    {
      act->setEnabled (state);
    }
  }
}

void
StructuralMenu::setAutoStartChecked (bool state)
{
  foreach (QAction *act, actions())
  {
    if (act->text() == "Set as starting node")
    {
      act->setChecked (state);
    }
  }
}

