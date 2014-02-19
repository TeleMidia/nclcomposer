#include "node.h"

Node::Node(QncgGraphicsEntity* parent)
    : QncgGraphicsNode(parent)
{
    createActions();
    createMenus();
    createConnections();
}

Node::~Node()
{

}

void Node::createActions()
{
    // node menu
    nodeAction = new QAction(this);
    nodeAction->setText(tr("Node"));

    nodeAction->setEnabled(true);
}

void Node::createMenus()
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

void Node::createConnections()
{
    connect(this, SIGNAL(entityChanged()), SLOT(requestEntityChange()));
    connect(this, SIGNAL(entitySelected()), SLOT(requestEntitySelection()));

    connect(nodeAction, SIGNAL(triggered()), SLOT(performNode()));
}

void Node::requestEntityChange()
{
    emit entityChanged(this);
}

void Node::requestEntitySelection()
{
    emit entitySelected(this);
}

void Node::performNode()
{
    Node* entity = new Node(this);
    entity->setTop(getHeight()/2 - 100/2);
    entity->setLeft(getWidth()/2 - 100/2);
    entity->setWidth(100);
    entity->setHeight(100);
    entity->adjust();

    connect(entity, SIGNAL(entityChanged(QncgGraphicsEntity*)), SIGNAL(entityChanged(QncgGraphicsEntity*)));
    connect(entity, SIGNAL(entitySelected(QncgGraphicsEntity*)), SIGNAL(entitySelected(QncgGraphicsEntity*)));

    addncgGraphicsEntity(entity);
}

void Node::draw(QPainter* painter)
{
    painter->setBrush(QBrush(QColor("#C1FFC1")));
    painter->drawRect(4+20/2, 4+20/2, getWidth()-1-20, getHeight()-1-20);

    if (isMoving()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawRect(getMoveLeft()+4-getLeft(),getMoveTop()+4-getTop(),getWidth()-1,getHeight()-1);

    }else if (isResizing()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawRect(getResizeLeft()+4-getLeft(),getResizeTop()+4-getTop(),getResizeWidth()-1,getResizeHeight()-1);
    }
}

void Node::delineate(QPainterPath* painter) const
{
    painter->addRect(4+20/2, 4+20/2, getWidth()-20, getHeight()-20);
}

void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QncgGraphicsNode::contextMenuEvent(event);

    if (!event->isAccepted())
    {
        contextMenu->exec(event->screenPos());

        event->accept();
    }
}
