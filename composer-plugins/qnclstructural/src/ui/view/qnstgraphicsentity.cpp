#include "qnstgraphicsentity.h"

//
// ATTENTION: This code needs a refactoring.
//

QnstGraphicsEntity::QnstGraphicsEntity(QnstGraphicsEntity* parent)
  : QncgGraphicsEntity(parent), QnstEntity(parent)
{
    setnstType(Qnst::NoType);

    setnstGraphicsParent(parent);

    connect(this, SIGNAL(entitySelected()), SLOT(requestEntitySelection()));
    connect(this, SIGNAL(entityAboutToChange(QMap<QString,QString>)), SLOT(requestEntityPreparation(QMap<QString,QString>)));

    menu = NULL;
    draggable = false;
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

QMap<QString, QSet<int> > QnstGraphicsEntity::getAngles()
{
    return angles;
}

void QnstGraphicsEntity::addAngle(QString uid, int angle)
{
    angles[uid].insert(angle);
}

void QnstGraphicsEntity::removeAngle(QString uid, int angle)
{
    angles[uid].remove(angle);

    if (angles[uid].isEmpty()){
        angles.remove(uid);
    }
}

QVector<QnstGraphicsEntity*> QnstGraphicsEntity::getnstGraphicsEntities()
{
    return entities;
}

bool QnstGraphicsEntity::isDraggable()
{
  return draggable;
}

void QnstGraphicsEntity::setDraggable(bool isDraggable)
{
  this->draggable = isDraggable;
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
        connect(entity, SIGNAL(entityAboutToChange(QnstGraphicsEntity*,QMap<QString,QString>)), SIGNAL(entityAboutToChange(QnstGraphicsEntity*,QMap<QString,QString>)));
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

void QnstGraphicsEntity::requestEntityPreparation(QMap<QString, QString> properties)
{
    emit entityAboutToChange(this, properties);
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


