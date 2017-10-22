#include "StructuralMenu.h"

StructuralMenu::StructuralMenu (QWidget *parent) : QMenu (parent)
{
  setInsertTop (0);
  setInsertLeft (0);

  adjust ();
}

StructuralMenu::~StructuralMenu () {}

qreal
StructuralMenu::getInsertTop () const
{
  return _insertTop;
}

void
StructuralMenu::setInsertTop (qreal insertTop)
{
  _insertTop = insertTop;
}

qreal
StructuralMenu::getInsertLeft () const
{
  return _insertLeft;
}

void
StructuralMenu::setInsertLeft (qreal insertLeft)
{
  _insertLeft = insertLeft;
}

void
StructuralMenu::adjust (StructuralType type)
{
  switch (type)
  {
    case Structural::Media:
    {
      switchAutostart (false);
      break;
    }

    case Structural::Body:
    case Structural::Context:
    {
      switchAutostart (false);
      break;
    }

    case Structural::Switch:
    {
      switchAutostart (false);
      break;
    }

    case Structural::Area:
    case Structural::Property:
    case Structural::Port:
    case Structural::SwitchPort:
    {
      switchAutostart (false);
      break;
    }

    default:
    {
      switchAutostart (false);
      break;
    }
  }
}

void
StructuralMenu::switchAutostart (bool state)
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
StructuralMenu::switchAutostartProperty (bool state)
{
  foreach (QAction *act, actions())
  {
    if (act->text() == "Set as starting node")
    {
      act->setChecked (state);
    }
  }
}

//void
//StructuralMenu::performMedia ()
//{
//  qreal top = _insertTop - STR_DEFAULT_CONTENT_H / 2;
//  qreal left = _insertLeft - STR_DEFAULT_CONTENT_W / 2;

//  emit performedInsert (Structural::Media,
//                        StructuralUtil::createProperties (top, left));
//}
