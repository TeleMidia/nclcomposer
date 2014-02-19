#include "Change.h"

Change::Change(StructuralView* view, QString uid,
                                     const QMap<QString,QString> &properties)
{
  this->uid = uid;
  this->view = view;

  if (view->entities.contains(uid))
  {
    StructuralGraphicsEntity* entity = view->entities[uid];

    previous["top"] = QString::number(entity->getTop());
    previous["left"] = QString::number(entity->getLeft());
    previous["width"] = QString::number(entity->getWidth());
    previous["height"] = QString::number(entity->getHeight());
  }

  next = properties;

  ignore = true;
}

Change::~Change()
{

}

void Change::undo()
{
  if (view->entities.contains(uid))
  {
    StructuralGraphicsEntity* entity = view->entities[uid];

    entity->setProperties(previous);

    entity->adjust();
  }
}

void Change::redo()
{
  if (!ignore)
  {
    if (view->entities.contains(uid))
    {
      StructuralGraphicsEntity* entity = view->entities[uid];

      entity->setProperties(next);

      entity->adjust();
    }
  }
  else
  {
    ignore = false;
  }
}
