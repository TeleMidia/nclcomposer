#include "qnstscene.h"

QnstScene::QnstScene(QObject* parent)
    : QGraphicsScene(parent)
{
    createObjects();
    createConnections();
}

QnstScene::~QnstScene()
{

}

QVector<QnstGraphicsEntity*> QnstScene::getRoots() const
{
    return roots;
}

void QnstScene::addRoot(QnstGraphicsEntity* entity)
{
    if (entity != NULL){
        roots.append(entity);

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

        addItem(entity);

        menu->menuInsert->setEnabled(false);
        menu->actionBody->setEnabled(false);
    }
}

void QnstScene::removeRoot(QnstGraphicsEntity* entity)
{
    if (entity != NULL){
        int index = roots.indexOf(entity);

        if (index >= 0){
            roots.remove(index);

            removeItem(entity);

            menu->menuInsert->setEnabled(true);
            menu->actionBody->setEnabled(true);
        }
    }
}

void QnstScene::createObjects()
{
    menu = new QnstMenu();
    menu->actionExport->setEnabled(true);

    menu->menuInsert->setEnabled(true);
    menu->actionBody->setEnabled(true);
}

void QnstScene::createConnections()
{
    connect(menu, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

    connect(menu, SIGNAL(bodyRequested()), SLOT(performBody()));
}

void QnstScene::performBody()
{
    QnstGraphicsBody* entity = new QnstGraphicsBody();
    entity->setTop(height()/2 - 550/2);
    entity->setLeft(width()/2 - 750/2);
    entity->setWidth(750);
    entity->setHeight(550);

    addRoot(entity);

    emit entityAdded(entity);
}

void QnstScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QGraphicsScene::contextMenuEvent(event);

    if (!event->isAccepted())
    {
        menu->exec(event->screenPos());

        event->accept();
    }
}
