#include "scene.h"

Scene::Scene(QObject* parent)
    : QGraphicsScene(parent)
{
    createActions();
    createMenus();
    createConnections();
}

Scene::~Scene()
{

}

void Scene::createActions()
{
    // node action
    nodeAction = new QAction(this);
    nodeAction->setText(tr("Node"));

    nodeAction->setEnabled(true);
}

void Scene::createMenus()
{
    // insert menu
    insertMenu = new QMenu();
    insertMenu->setTitle(tr("Insert"));

    insertMenu->addAction(nodeAction);

    insertMenu->setEnabled(true);

    // context menu
    contextMenu = new QMenu();
    contextMenu->addMenu(insertMenu);
}

void Scene::createConnections()
{
    connect(nodeAction, SIGNAL(triggered()), SLOT(performNode()));
}

void Scene::performNode()
{
    Node* entity = new Node();
    entity->setTop(height()/2 - 300/2);
    entity->setLeft(width()/2 - 300/2);
    entity->setWidth(300);
    entity->setHeight(300);
    entity->adjust();

    connect(entity, SIGNAL(entityChanged(QncgGraphicsEntity*)), SIGNAL(entityChanged(QncgGraphicsEntity*)));
    connect(entity, SIGNAL(entitySelected(QncgGraphicsEntity*)), SIGNAL(entitySelected(QncgGraphicsEntity*)));

    addItem(entity);
}

void Scene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QGraphicsScene::contextMenuEvent(event);

    if (!event->isAccepted())
    {
        contextMenu->exec(event->screenPos());

        event->accept();
    }
}




