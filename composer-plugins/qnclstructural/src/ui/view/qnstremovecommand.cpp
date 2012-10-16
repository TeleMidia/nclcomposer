#include "qnstremovecommand.h"

QnstRemoveCommand::QnstRemoveCommand(QnstView* view, QnstGraphicsEntity* entity)
{
    this->view = view; parent = "";

    this->entity = NULL;

    copy(entity); ignore = true;
}

QnstRemoveCommand::~QnstRemoveCommand()
{
    delete entity;
}

void QnstRemoveCommand::copy(QnstGraphicsEntity* entity)
{
    switch(entity->getnstType())
    {

    // if the entity type is BODY
    case Qnst::Body:
    // or if the entity type is IMAGE
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
    // if the entity type is NCL
    case Qnst::NCL:
    // if the entity type is MEDIA
    case Qnst::Media:
        this->entity = new QnstGraphicsMedia();
        this->entity->setnstType(entity->getnstType());
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

        qDebug() << "COPY:" << "AGGREGATOR";

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

        this->entity->setnstGraphicsParent(NULL);

        foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
            copy(e, this->entity);
        }

        if (entity->getnstGraphicsParent() != NULL){
            parent = QString(entity->getnstGraphicsParent()->getnstUid());
        }
    }
}

void QnstRemoveCommand::copy(QnstGraphicsEntity* entity, QnstGraphicsEntity* parent)
{
    QnstGraphicsEntity* c = NULL;

    switch(entity->getnstType()){

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
      // or if the entity type is MEDIA
      case Qnst::Media:
          c = new QnstGraphicsMedia();
          c->setnstType(entity->getnstType());
          break;

    // if the entity type is BODY
    case Qnst::Body:
        c = new QnstGraphicsBody();

        break;    

    // if the entity type is CONTEXT
    case Qnst::Context:
        c = new QnstGraphicsContext();

        break;

    // if the entity type is SWITCH
    case Qnst::Switch:
        c = new QnstGraphicsSwitch();

        break;

    // if the entity type is PORT
    case Qnst::Port:
        c = new QnstGraphicsPort();

        break;

    // if the entity type is SWPORT
    case Qnst::SwitchPort:
        c = new QnstGraphicsSwitchPort();

        break;

    // if the entity type is AREA
    case Qnst::Area:
        c = new QnstGraphicsArea();

        break;

    // if the entity type is PROPERTY
    case Qnst::Property:
        c = new QnstGraphicsProperty();

        break;

    // if the entity type is AGGREGATOR
    case Qnst::Aggregator:
        c = new QnstGraphicsAggregator();

        qDebug() << "COPY:" << "AGGREGATOR";

        break;

    default:
      // do nothing
      break;
    }

    if (c != NULL){
        c->setnstUid(entity->getnstUid());

        c->setnstGraphicsParent(parent);

        this->entity->setUsrData(entity->getUsrData());

        c->setnstId(entity->getnstId());

        c->setTop(entity->getTop());
        c->setLeft(entity->getLeft());
        c->setWidth(entity->getWidth());
        c->setHeight(entity->getHeight());

        if (c->getnstType() == Qnst::Aggregator){
            qDebug() << entity->getnstId() << entity->getWidth() << "x" << entity->getHeight();
        }

        parent->addnstGraphicsEntity(c);

        foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
            copy(e, c);
        }
    }
}

void QnstRemoveCommand::undo()
{
    QnstGraphicsEntity* e = NULL;

    if (view->entities.contains(parent) || entity->getnstType() == Qnst::Body){

        switch(entity->getnstType()){

        // if the entity type is BODY
        case Qnst::Body:
        // if the entity type is IMAGE
        case Qnst::Image:
        // if the entity type is AUDIO
        case Qnst::Audio:
        // if the entity type is VIDEO
        case Qnst::Video:
        // if the entity type is TEXT
        case Qnst::Text:
        // if the entity type is SCRIPT
        case Qnst::Script:
        // if the entity type is SETTINGS
        case Qnst::Settings:
        // if the entity type is HTML
        case Qnst::Html:
        // if the entity type is NCL
        case Qnst::NCL:
        // if the entity type is MEDIA
        case Qnst::Media:
            e = new QnstGraphicsMedia();
            e->setnstType(entity->getnstType());

            ((QnstGraphicsMedia*) e)->setnstId(entity->getnstId());

            break;

        // if the entity type is CONTEXT
        case Qnst::Context:
            e = new QnstGraphicsContext();

            e->setnstId(entity->getnstId());

            break;

        // if the entity type is SWITCH
        case Qnst::Switch:
            e = new QnstGraphicsSwitch();

            e->setnstId(entity->getnstId());

            break;

        // if the entity type is PORT
        case Qnst::Port:
            e = new QnstGraphicsPort();

            e->setnstId(entity->getnstId());

            break;

        // if the entity type is SWPORT
        case Qnst::SwitchPort:
            e = new QnstGraphicsSwitchPort();

            e->setnstId(entity->getnstId());

            break;

        // if the entity type is AREA
        case Qnst::Area:
            e = new QnstGraphicsArea();

            e->setnstId(entity->getnstId());

            break;

        // if the entity type is PROPERTY
        case Qnst::Property:
            e = new QnstGraphicsProperty();

            e->setnstId(entity->getnstId());

            break;

        // if the entity type is AGGREGATOR
        case Qnst::Aggregator:
            e = new QnstGraphicsAggregator();

            e->setnstId(entity->getnstId());

            qDebug() << "PASTE:" << "AGGREGATOR";

            break;

        default:
          // do nothing
          break;
        }

        if (e != NULL){
            e->setnstUid(entity->getnstUid());

            e->setUsrData(entity->getUsrData());

            if (parent != ""){
                QnstGraphicsEntity* gparent = view->entities[parent];

                gparent->addnstGraphicsEntity(e);

                e->setnstGraphicsParent(gparent);
            }else{

                e->setnstGraphicsParent(NULL);
            }

            e->setTop(entity->getTop());
            e->setLeft(entity->getLeft());
            e->setWidth(entity->getWidth());
            e->setHeight(entity->getHeight());
            e->adjust();

            view->requestEntityAddition(e, true);

            foreach(QnstGraphicsEntity* c, entity->getnstGraphicsEntities()){
                paste(c, e);
            }
        }
   }
}

void QnstRemoveCommand::paste(QnstGraphicsEntity* entity, QnstGraphicsEntity* parent)
{
    QnstGraphicsEntity* e = NULL;

    switch(entity->getnstType()){

    // if the entity type is IMAGE
    case Qnst::Image:
    // if the entity type is AUDIO
    case Qnst::Audio:
    // if the entity type is VIDEO
    case Qnst::Video:
    // if the entity type is TEXT
    case Qnst::Text:
    // if the entity type is SCRIPT
    case Qnst::Script:
    // if the entity type is SETTINGS
    case Qnst::Settings:
    // if the entity type is HTML
    case Qnst::Html:
    // if the entity type is NCL
    case Qnst::NCL:
    // if the entity type is MEDIA
    case Qnst::Media:
        e = new QnstGraphicsMedia();
        e->setnstType(entity->getnstType());

        ((QnstGraphicsMedia*) e)->setnstId(entity->getnstId());

        break;

    // if the entity type is BODY
    case Qnst::Body:
        e = new QnstGraphicsBody();

        e->setnstId(entity->getnstId());

        break;

    // if the entity type is CONTEXT
    case Qnst::Context:
        e = new QnstGraphicsContext();

        e->setnstId(entity->getnstId());

        break;

    // if the entity type is SWITCH
    case Qnst::Switch:
        e = new QnstGraphicsSwitch();

        e->setnstId(entity->getnstId());

        break;

    // if the entity type is PORT
    case Qnst::Port:
        e = new QnstGraphicsPort();

        e->setnstId(entity->getnstId());

        break;

    // if the entity type is SWPORT
    case Qnst::SwitchPort:
        e = new QnstGraphicsSwitchPort();

        e->setnstId(entity->getnstId());

        break;

    // if the entity type is AREA
    case Qnst::Area:
        e = new QnstGraphicsArea();

        e->setnstId(entity->getnstId());

        break;

    // if the entity type is PROPERTY
    case Qnst::Property:
        e = new QnstGraphicsProperty();

        e->setnstId(entity->getnstId());

        break;

    // if the entity type is AGGREGATOR
    case Qnst::Aggregator:
        e = new QnstGraphicsAggregator();

        e->setnstId(entity->getnstId());

        qDebug() << "PASTE:" << "AGGREGATOR";

        break;

    default:
      // do nothing
      break;
    }

    if (e != NULL){
        e->setnstUid(entity->getnstUid());
        e->setnstGraphicsParent(parent);

        e->setUsrData(entity->getUsrData());

        parent->addnstGraphicsEntity(e);

        e->setTop(entity->getTop());
        e->setLeft(entity->getLeft());
        e->setWidth(entity->getWidth());
        e->setHeight(entity->getHeight());
        e->adjust();

        if (e->getnstType() == Qnst::Aggregator){
            qDebug() << entity->getWidth() << "x" << entity->getHeight();
        }

        view->requestEntityAddition(e, true);

        foreach(QnstGraphicsEntity* c, entity->getnstGraphicsEntities()){
            paste(c, e);
        }
    }
}

void QnstRemoveCommand::redo()
{
    if (!ignore){
        if (view->entities.contains(entity->getnstUid())){
            view->requestEntityRemotion(view->entities[entity->getnstUid()], true);
        }
    }else{
        ignore = false;
    }
}
