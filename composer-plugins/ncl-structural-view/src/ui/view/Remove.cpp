#include "Remove.h"
#include "StructuralUtil.h"

Remove::Remove(StructuralView* view, StructuralGraphicsEntity* entity)
{
  this->view = view; parent = "";

  this->entity = NULL;

  copy(entity); ignore = true;
}

Remove::~Remove()
{
  delete entity;
}

void Remove::copy(StructuralGraphicsEntity* entity)
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

    this->entity->setnstGraphicsParent(NULL);

    foreach(StructuralGraphicsEntity* e, entity->getnstGraphicsEntities())
    {
      copy(e, this->entity);
    }

    if (entity->getnstGraphicsParent() != NULL)
    {
      parent = QString(entity->getnstGraphicsParent()->getnstUid());
    }
  }
}

void Remove::copy(StructuralGraphicsEntity* entity,
                             StructuralGraphicsEntity* parent)
{
  StructuralGraphicsEntity* c = StructuralUtil::makeGraphicsEntity(entity->getnstType(),
                                                       parent);

  if (c != NULL)
  {
    c->setnstUid(entity->getnstUid());

    c->setnstGraphicsParent(parent);

    this->entity->setUsrData(entity->getUsrData());

    c->setnstId(entity->getnstId());

    c->setTop(entity->getTop());
    c->setLeft(entity->getLeft());
    c->setWidth(entity->getWidth());
    c->setHeight(entity->getHeight());

    if (c->getnstType() == Structural::Aggregator)
    {
      qDebug() << entity->getnstId() << entity->getWidth() << "x" << entity->getHeight();
    }

    parent->addnstGraphicsEntity(c);

    foreach(StructuralGraphicsEntity* e, entity->getnstGraphicsEntities())
    {
      copy(e, c);
    }
  }
}

void Remove::undo()
{
  if (view->entities.contains(parent) || entity->getnstType() == Structural::Body)
  {
    StructuralGraphicsEntity* e = StructuralUtil::makeGraphicsEntity(entity->getnstType());
    if(e != NULL)
    {
      e->setnstId(entity->getnstId());
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

      e->setTop(entity->getTop());
      e->setLeft(entity->getLeft());
      e->setWidth(entity->getWidth());
      e->setHeight(entity->getHeight());
      e->adjust();

      view->requestEntityAddition(e, true);

      foreach(StructuralGraphicsEntity* c, entity->getnstGraphicsEntities())
      {
        paste(c, e);
      }
    }
  }
}

void Remove::paste(StructuralGraphicsEntity* entity,
                              StructuralGraphicsEntity* parent)
{
  StructuralGraphicsEntity* e = StructuralUtil::makeGraphicsEntity(entity->getnstType(),
                                                       parent);

  if (e != NULL)
  {
    e->setnstId(entity->getnstId());
    e->setnstUid(entity->getnstUid());
    e->setnstGraphicsParent(parent);

    e->setUsrData(entity->getUsrData());

    parent->addnstGraphicsEntity(e);

    e->setTop(entity->getTop());
    e->setLeft(entity->getLeft());
    e->setWidth(entity->getWidth());
    e->setHeight(entity->getHeight());
    e->adjust();

    if (e->getnstType() == Structural::Aggregator)
    {
      qDebug() << entity->getWidth() << "x" << entity->getHeight();
    }

    view->requestEntityAddition(e, true);

    foreach(StructuralGraphicsEntity* c, entity->getnstGraphicsEntities()){
      paste(c, e);
    }
  }
}

void Remove::redo()
{
  if (!ignore)
  {
    if (view->entities.contains(entity->getnstUid()))
    {
      view->requestEntityRemotion(view->entities[entity->getnstUid()], true);
    }
  }
  else
  {
    ignore = false;
  }
}
