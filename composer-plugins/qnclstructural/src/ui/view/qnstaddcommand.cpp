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
    }

    if (this->entity != NULL){
        this->entity->setnstUid(entity->getnstUid());

        this->entity->setnstId(entity->getnstId());

        this->entity->setTop(entity->getTop());
        this->entity->setLeft(entity->getLeft());
        this->entity->setWidth(entity->getWidth());
        this->entity->setHeight(entity->getHeight());

        if (entity->getnstGraphicsParent() != NULL){
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
    if (!ignore){
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

                ((QnstGraphicsImage*) e)->setnstId(entity->getnstId());

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
            }
       }

    }else{
        ignore = false;
    }
}
