#include "qnstaddcommand.h"

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
    switch(entity->getnstType())
    {
      // if the entity type is IMAGE
      case Qnst::Image:
      // or if the entity type is AUDIO
      case Qnst::Audio:
      // or if the entity type is VIDEO
      case Qnst::Video:
      // or if the entity type is TEXT
      case Qnst::Text:
      // or if the entity type is SCRIPT
      case Qnst::Script:
      // or if the entity type is SETTINGS
      case Qnst::Settings:
      // or if the entity type is HTML
      case Qnst::Html:
      // or if the entity type is NCL
      case Qnst::NCL:
      // if the entity type is MEDIA
      case Qnst::Media:
          this->entity = new QnstGraphicsMedia();
          this->entity->setnstType(entity->getnstType());
          break;

    // if the entity type is BODY
    case Qnst::Body:
        this->entity = new QnstGraphicsBody();

        break;

    // if the entity type is CONTEXT
    case Qnst::Context:
        this->entity = new QnstGraphicsContext();

        break;

    // if the entity type is SWITCH
    case Qnst::Switch:
        this->entity = new QnstGraphicsSwitch();

        break;

    // if the entity type is PORT
    case Qnst::Port:
        this->entity = new QnstGraphicsPort();

        break;

    // if the entity type is SWPORT
    case Qnst::SwitchPort:
        this->entity = new QnstGraphicsSwitchPort();

        break;

    // if the entity type is AREA
    case Qnst::Area:
        this->entity = new QnstGraphicsArea();

        break;

    // if the entity type is PROPERTY
    case Qnst::Property:
        this->entity = new QnstGraphicsProperty();

        break;

    // if the entity type is AGGREGATOR
    case Qnst::Aggregator:
        this->entity = new QnstGraphicsAggregator();

        break;

    default:
      // do nothing
      break;
    }

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
    if (view->entities.contains(entity->getnstUid())){
        view->requestEntityRemotion(view->entities[entity->getnstUid()], true);
    }
}

void QnstAddCommand::redo()
{
  if (!ignore)
  {
    QnstGraphicsEntity* e = NULL;

    if (view->entities.contains(parent) ||
        entity->getnstType() == Qnst::Body)
    {
      // instantiate the new entity
      switch(entity->getnstType())
      {
        case Qnst::Image:
        case Qnst::Audio:
        case Qnst::Video:
        case Qnst::Text:
        case Qnst::Script:
        case Qnst::Settings:
        case Qnst::Html:
        case Qnst::NCL:
        case Qnst::Media:
          e = new QnstGraphicsMedia();
          e->setnstType(entity->getnstType());
          break;

        case Qnst::Body: e = new QnstGraphicsBody(); break;

        case Qnst::Context: e = new QnstGraphicsContext(); break;
        case Qnst::Switch: e = new QnstGraphicsSwitch(); break;
        case Qnst::Port: e = new QnstGraphicsPort(); break;
        case Qnst::SwitchPort: e = new QnstGraphicsSwitchPort(); break;
        case Qnst::Area: e = new QnstGraphicsArea(); break;
        case Qnst::Property: e = new QnstGraphicsProperty(); break;
        case Qnst::Aggregator: e = new QnstGraphicsAggregator(); break;

      default:
        // do nothing
        break;
      }

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
