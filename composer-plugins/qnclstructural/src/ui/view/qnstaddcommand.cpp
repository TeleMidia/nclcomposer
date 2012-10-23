#include "qnstaddcommand.h"

#include "qnstutil.h"

QnstAddCommand::QnstAddCommand(QnstView* view, QnstGraphicsEntity* entity)
{
  this->view = view; parent = "";

  this->entity = NULL;

  copy(entity); ignore = true;
}

QnstAddCommand::~QnstAddCommand()
{
  delete entity;
}

void QnstAddCommand::copy(QnstGraphicsEntity* entity)
{
  this->entity = QnstUtil::makeGraphicsEntity(entity->getnstType());

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
      parent = QString(entity->getnstGraphicsParent()->getnstUid());
    }

    this->entity->setnstGraphicsParent(NULL);
  }
}

void QnstAddCommand::undo()
{
  if (view->entities.contains(entity->getnstUid()))
  {
     view->requestEntityRemotion(view->entities[entity->getnstUid()], true);
  }
}

void QnstAddCommand::redo()
{
  if (!ignore)
  {
    QnstGraphicsEntity* e = NULL;

    if (view->entities.contains(parent) || entity->getnstType() == Qnst::Body)
    {
      // instantiate the new entity
      e = QnstUtil::makeGraphicsEntity(entity->getnstType());

      if (e != NULL)
      {
        e->setnstUid(entity->getnstUid());
        e->setUsrData(entity->getUsrData());

        if (parent != "")
        {
          QnstGraphicsEntity* gparent = view->entities[parent];
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
