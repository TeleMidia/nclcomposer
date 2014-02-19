#include "Insert.h"

#include "StructuralUtil.h"

Insert::Insert(StructuralView* view, StructuralGraphicsEntity* entity)
{
  this->view = view; parent = "";

  this->entity = NULL;

  copy(entity);

  ignore = true;
}

Insert::~Insert()
{
  delete entity;
}

void Insert::copy(StructuralGraphicsEntity* entity)
{
  this->entity = StructuralUtil::makeGraphicsEntity(entity->getnstType());

  if (this->entity != NULL)
  {
    this->entity->setnstUid(entity->getnstUid());
    this->entity->setnstId(entity->getnstId());

    this->entity->setUsrData(entity->getUsrData());

    this->entity->setTop(entity->getTop());
    this->entity->setLeft(entity->getLeft());
    this->entity->setWidth(entity->getWidth());
    this->entity->setHeight(entity->getHeight());

    if (entity->getnstGraphicsParent() != NULL)
    {
      parent = entity->getnstGraphicsParent()->getnstUid();
    }

    this->entity->setnstGraphicsParent(NULL);
  }
}

void Insert::undo()
{
  if ( !ignore && this->entity != NULL &&
       view->entities.contains(entity->getnstUid()) )
  {
     view->requestEntityRemotion(view->entities[entity->getnstUid()], true);
  }
}

void Insert::redo()
{
  if (!ignore)
  {
    StructuralGraphicsEntity* e = NULL;

    if (view->entities.contains(parent) ||
        entity->getnstType() == Structural::Body)
    {
      // instantiate the new entity
      e = StructuralUtil::makeGraphicsEntity(entity->getnstType());

      if (e != NULL)
      {
        e->setnstUid(entity->getnstUid());
        e->setUsrData(entity->getUsrData());

        if (parent != "")
        {
          StructuralGraphicsEntity* gparent = view->entities[parent];
          gparent->addnstGraphicsEntity(e);

          e->setnstGraphicsParent(gparent);
        }
        else
        {
          e->setnstGraphicsParent(NULL);
        }

        // \todo separate this in a function
        e->setTop(entity->getTop());
        e->setLeft(entity->getLeft());
        e->setWidth(entity->getWidth());
        e->setHeight(entity->getHeight());
        e->adjust();

        view->requestEntityAddition(e, true);
      }
    }
  }
  else
  {
    ignore = false;
  }
}
