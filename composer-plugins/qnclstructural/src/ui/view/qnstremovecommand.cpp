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
    switch(entity->getnstType()){

    // if the entity type is BODY
    case Qnst::Body:
        this->entity = new QnstGraphicsBody();

        break;

    // if the entity type is IMAGE
    case Qnst::Image:
        this->entity = new QnstGraphicsImage();

        break;

    // if the entity type is AUDIO
    case Qnst::Audio:
        this->entity = new QnstGraphicsAudio();

        break;

    // if the entity type is VIDEO
    case Qnst::Video:
        this->entity = new QnstGraphicsVideo();

        break;

    // if the entity type is TEXT
    case Qnst::Text:
        this->entity = new QnstGraphicsText();

        break;

    // if the entity type is SCRIPT
    case Qnst::Script:
        this->entity = new QnstGraphicsScript();

        break;

    // if the entity type is SETTINGS
    case Qnst::Settings:
        this->entity = new QnstGraphicsSettings();

        break;

    // if the entity type is HTML
    case Qnst::Html:
        this->entity = new QnstGraphicsHTML();

        break;

    // if the entity type is NCL
    case Qnst::NCL:
        this->entity = new QnstGraphicsNCL();

        break;

    // if the entity type is MEDIA
    case Qnst::Media:
        this->entity = new QnstGraphicsMedia();
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

    if (this->entity != NULL){
        this->entity->setnstUid(entity->getnstUid());

        this->entity->setnstId(entity->getnstId());

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

    // if the entity type is BODY
    case Qnst::Body:
        c = new QnstGraphicsBody();

        break;

    // if the entity type is IMAGE
    case Qnst::Image:
        c = new QnstGraphicsImage();

        break;

    // if the entity type is AUDIO
    case Qnst::Audio:
        c = new QnstGraphicsAudio();

        break;

    // if the entity type is VIDEO
    case Qnst::Video:
        c = new QnstGraphicsVideo();

        break;

    // if the entity type is TEXT
    case Qnst::Text:
        c = new QnstGraphicsText();

        break;

    // if the entity type is SCRIPT
    case Qnst::Script:
        c = new QnstGraphicsScript();

        break;

    // if the entity type is SETTINGS
    case Qnst::Settings:
        c = new QnstGraphicsSettings();

        break;

    // if the entity type is HTML
    case Qnst::Html:
        c = new QnstGraphicsHTML();

        break;

    // if the entity type is NCL
    case Qnst::NCL:
        c = new QnstGraphicsNCL();

        break;

    // if the entity type is MEDIA
    case Qnst::Media:
        c = new QnstGraphicsMedia();
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
            e = new QnstGraphicsBody();

            e->setnstId(entity->getnstId());

            break;

        // if the entity type is IMAGE
        case Qnst::Image:
            e = new QnstGraphicsImage();

            ((QnstGraphicsImage*) e)->setnstId(entity->getnstId());

            break;

        // if the entity type is AUDIO
        case Qnst::Audio:
            e = new QnstGraphicsAudio();

            ((QnstGraphicsAudio*) e)->setnstId(entity->getnstId());

            break;

        // if the entity type is VIDEO
        case Qnst::Video:
            e = new QnstGraphicsVideo();

            ((QnstGraphicsVideo*) e)->setnstId(entity->getnstId());

            break;

        // if the entity type is TEXT
        case Qnst::Text:
            e = new QnstGraphicsText();

            ((QnstGraphicsText*) e)->setnstId(entity->getnstId());

            break;

        // if the entity type is SCRIPT
        case Qnst::Script:
            e = new QnstGraphicsScript();

            ((QnstGraphicsScript*) e)->setnstId(entity->getnstId());

            break;

        // if the entity type is SETTINGS
        case Qnst::Settings:
            e = new QnstGraphicsSettings();

            ((QnstGraphicsSettings*) e)->setnstId(entity->getnstId());

            break;

        // if the entity type is HTML
        case Qnst::Html:
            e = new QnstGraphicsHTML();

            ((QnstGraphicsHTML*) e)->setnstId(entity->getnstId());

            break;

        // if the entity type is NCL
        case Qnst::NCL:
            e = new QnstGraphicsNCL();

            ((QnstGraphicsNCL*) e)->setnstId(entity->getnstId());

            break;

        // if the entity type is MEDIA
        case Qnst::Media:
            e = new QnstGraphicsMedia();

            ((QnstGraphicsImage*) e)->setnstId(entity->getnstId());

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

    // if the entity type is BODY
    case Qnst::Body:
        e = new QnstGraphicsBody();

        e->setnstId(entity->getnstId());

        break;

    // if the entity type is IMAGE
    case Qnst::Image:
        e = new QnstGraphicsImage();

        ((QnstGraphicsImage*) e)->setnstId(entity->getnstId());

        break;

    // if the entity type is AUDIO
    case Qnst::Audio:
        e = new QnstGraphicsAudio();

        ((QnstGraphicsAudio*) e)->setnstId(entity->getnstId());

        break;

    // if the entity type is VIDEO
    case Qnst::Video:
        e = new QnstGraphicsVideo();

        ((QnstGraphicsVideo*) e)->setnstId(entity->getnstId());

        break;

    // if the entity type is TEXT
    case Qnst::Text:
        e = new QnstGraphicsText();

        ((QnstGraphicsText*) e)->setnstId(entity->getnstId());

        break;

    // if the entity type is SCRIPT
    case Qnst::Script:
        e = new QnstGraphicsScript();

        ((QnstGraphicsScript*) e)->setnstId(entity->getnstId());

        break;

    // if the entity type is SETTINGS
    case Qnst::Settings:
        e = new QnstGraphicsSettings();

        ((QnstGraphicsSettings*) e)->setnstId(entity->getnstId());

        break;

    // if the entity type is HTML
    case Qnst::Html:
        e = new QnstGraphicsHTML();

        ((QnstGraphicsHTML*) e)->setnstId(entity->getnstId());

        break;

    // if the entity type is NCL
    case Qnst::NCL:
        e = new QnstGraphicsNCL();

        ((QnstGraphicsNCL*) e)->setnstId(entity->getnstId());

        break;

    // if the entity type is MEDIA
    case Qnst::Media:
        e = new QnstGraphicsMedia();

        ((QnstGraphicsImage*) e)->setnstId(entity->getnstId());

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
