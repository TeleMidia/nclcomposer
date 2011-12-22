#include "qnstgraphicsentity.h"

QnstGraphicsEntity::QnstGraphicsEntity(QnstGraphicsEntity* parent)
    : QncgGraphicsEntity(parent), QnstEntity(parent)
{
    setnstType(Qnst::NoType);

    setnstGraphicsParent(parent);

    connect(this, SIGNAL(entitySelected()), SLOT(requestEntitySelection()));

    menu = NULL;
}


QnstGraphicsEntity::~QnstGraphicsEntity()
{

}

QnstGraphicsEntity* QnstGraphicsEntity::getnstGraphicsParent() const
{
    return parent;
}

void QnstGraphicsEntity::setnstGraphicsParent(QnstGraphicsEntity* parent)
{
    this->parent = parent;

    setnstParent(parent);
    setncgGraphicsParent(parent);
}

QVector<QnstGraphicsEntity*> QnstGraphicsEntity::getnstGraphicsEntities()
{
    return entities;
}

void QnstGraphicsEntity::addnstGraphicsEntity(QnstGraphicsEntity* entity)
{
    if (entity != NULL){
        entities.append(entity);

        connect(entity, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
        connect(entity, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

        connect(entity, SIGNAL(cutRequested()), SIGNAL(cutRequested()));
        connect(entity, SIGNAL(copyRequested()), SIGNAL(copyRequested()));
        connect(entity, SIGNAL(pasteRequested()), SIGNAL(pasteRequested()));

        connect(entity, SIGNAL(deleteRequested()), SIGNAL(deleteRequested()));

        connect(entity, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

        connect(entity, SIGNAL(zoominRequested()), SIGNAL(zoominRequested()));
        connect(entity, SIGNAL(zoomoutRequested()), SIGNAL(zoomoutRequested()));
        connect(entity, SIGNAL(zoomresetRequested()), SIGNAL(zoomresetRequested()));
        connect(entity, SIGNAL(fullscreenRequested()), SIGNAL(fullscreenRequested()));

        connect(entity, SIGNAL(entityAdded(QnstGraphicsEntity*)), SIGNAL(entityAdded(QnstGraphicsEntity*)));
        connect(entity, SIGNAL(entityChanged(QnstGraphicsEntity*)), SIGNAL(entityChanged(QnstGraphicsEntity*)));
        connect(entity, SIGNAL(entityRemoved(QnstGraphicsEntity*)), SIGNAL(entityRemoved(QnstGraphicsEntity*)));
        connect(entity, SIGNAL(entitySelected(QnstGraphicsEntity*)), SIGNAL(entitySelected(QnstGraphicsEntity*)));

        addnstEntity(entity);
        addncgGraphicsEntity(entity);
    }
}

void QnstGraphicsEntity::removenstGraphicsEntity(QnstGraphicsEntity* entity)
{
    if (entity != NULL){
        int index = entities.indexOf(entity);

        if (index >= 0){
            entities.remove(index);

            removenstEntity(entity);
            removencgGraphicsEntity(entity);
        }
    }
}

void QnstGraphicsEntity::requestEntityChange()
{
    emit entityChanged(this);
}

void QnstGraphicsEntity::requestEntitySelection()
{
    emit entitySelected(this);
}

void QnstGraphicsEntity::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QncgGraphicsEntity::contextMenuEvent(event);

    if (!event->isAccepted()){

        if (!isSelected()){
            setSelected(true); emit entitySelected(this);
        }

        if (menu != NULL){
            menu->exec(event->screenPos());
        }

        event->accept();
    }
}


