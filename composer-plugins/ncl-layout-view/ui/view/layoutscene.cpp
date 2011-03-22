#include "layoutscene.h"

#include <QDebug>

#include "layoutitem.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

LayoutScene::LayoutScene(QObject* parent)
    : QGraphicsScene(parent)
{
    createActions();
    createMenus();
    createConnections();
}

LayoutScene::~LayoutScene()
{
}

void LayoutScene::createMenus()
{
    //TODO: LayoutDocument::createMenus()

    contextMenu = new QMenu();

    insertMenu = new QMenu("insert", contextMenu);
    insertMenu->addAction(regionAction);

    contextMenu->addMenu(insertMenu);
}

void LayoutScene::createActions()
{
    //TODO: LayoutDocument::createActions()
    regionAction = new QAction(this);
    regionAction->setText("Region");
}

void LayoutScene::createConnections()
{
    //TODO: LayoutDocument::createConnections()
    connect(regionAction, SIGNAL(triggered()),SLOT(createItem()));
}

void LayoutScene::createItem()
{
    //TODO: LayoutDocument::createItem()
    LayoutItem* item = new LayoutItem();
    addItem(item);

    item->setParent(this);
    item->setpWidth(0.25);
    item->setpHeight(0.25);
    item->setpLeft(insertPoint.x()/width());
    item->setpTop(insertPoint.y()/height());
    item->adjustItem();

    connect(item,SIGNAL(itemAdded(LayoutItem*)),this,SIGNAL(itemAdded(LayoutItem*)));
    connect(item,SIGNAL(itemRemoved(LayoutItem*)),this,SIGNAL(itemRemoved(LayoutItem*)));

    qDebug() << "emit itemAdded from LayoutScene";

    emit itemAdded(item);
}

// void LayoutScene::removeItem(LayoutItem* item)
// {

// }

//void LayoutScene::addItem(AbstractItem* item)
//{
    //TODO: LayoutDocument::addItem(...)

//    QGraphicsScene::addItem(item);
//}

//void LayoutScene::removeItem(AbstractItem* item)
//{
    //TODO: LayoutDocument::removeItem(...)
//}

void LayoutScene::contextMenuEvent(
            QGraphicsSceneContextMenuEvent* contextMenuEvent)
{
//    contextMenuEvent->setAccepted(false);
    //TODO: LayoutDocument::contextMenuEvent(...)
    QGraphicsScene::contextMenuEvent(contextMenuEvent);

    qDebug() << "scene";

    if (!contextMenuEvent->isAccepted()){
        insertPoint = contextMenuEvent->scenePos();
        contextMenu->exec(contextMenuEvent->screenPos());
    }
}

void LayoutScene::adjustItems()
{
//    qDebug() << "adjusting";

    for (int i=0;i<items().size();i++){
        LayoutItem* item = (LayoutItem*) items().at(i);

//        qDebug() << "adjusting" << item->getTile();
        item->adjustItem();
//        item->setPos(item->pos());
//        item->setSize(item->getSize());
    }

    qDebug() << "--------------------------";
}

}
}
}
}
}
