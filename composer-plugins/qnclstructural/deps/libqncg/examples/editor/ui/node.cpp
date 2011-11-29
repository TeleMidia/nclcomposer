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

    // interface menu
    interfaceAction = new QAction(this);
    interfaceAction->setText(tr("Interface"));

    interfaceAction->setEnabled(true);

    // compact action
    compactAction = new QAction(this);
    compactAction->setText(tr("Compact"));

    compactAction->setEnabled(true);

    // fit action
    fitAction = new QAction(this);
    fitAction->setText(tr("Fit"));

    fitAction->setEnabled(true);

    // spread action
    clockAction = new QAction(this);
    clockAction->setText(tr("Clock"));

    clockAction->setEnabled(true);

    // inside action
    insideAction = new QAction(this);
    insideAction->setText(tr("Inside"));

    insideAction->setEnabled(true);

    // outside action
    outsideAction = new QAction(this);
    outsideAction->setText(tr("Outside"));

    outsideAction->setEnabled(true);

    // attract action
    attractAction = new QAction(this);
    attractAction->setText(tr("Attract"));

    attractAction->setEnabled(true);

    // repel action
    repelAction = new QAction(this);
    repelAction->setText(tr("Repel"));

    repelAction->setEnabled(true);
}

void Node::createMenus()
{
    // insert menu
    insertMenu = new QMenu();
    insertMenu->setTitle(tr("Insert"));

    insertMenu->addAction(nodeAction);
    insertMenu->addAction(interfaceAction);

    insertMenu->setEnabled(true);

    // adjust menu
    adjustMenu = new QMenu();
    adjustMenu->setTitle(tr("Adjust"));

    adjustMenu->addAction(fitAction);
    adjustMenu->addAction(compactAction);
    adjustMenu->addSeparator();
    adjustMenu->addAction(insideAction);
    adjustMenu->addAction(outsideAction);

    adjustMenu->setEnabled(true);

    // spread menu
    organizeMenu = new QMenu();
    organizeMenu->setTitle(tr("Organize"));

    organizeMenu->addAction(clockAction);

    organizeMenu->setEnabled(true);

    // others menu
    othersMenu = new QMenu();
    othersMenu->setTitle(tr("Others"));

    othersMenu->setEnabled(true);

    othersMenu->addAction(repelAction);
    othersMenu->addAction(attractAction);

    // context menu
    contextMenu = new QMenu();
    contextMenu->addMenu(insertMenu);
    contextMenu->addMenu(adjustMenu);
    contextMenu->addMenu(organizeMenu);
    contextMenu->addSeparator();
    contextMenu->addMenu(othersMenu);
}

void Node::createConnections()
{
    connect(this, SIGNAL(entityChanged()), SLOT(requestEntityChange()));
    connect(this, SIGNAL(entitySelected()), SLOT(requestEntitySelection()));

    connect(nodeAction, SIGNAL(triggered()), SLOT(performNode()));
    connect(interfaceAction, SIGNAL(triggered()), SLOT(performInterface()));

    connect(fitAction, SIGNAL(triggered()), SLOT(performFit()));
    connect(compactAction, SIGNAL(triggered()), SLOT(performCompact()));

    connect(clockAction, SIGNAL(triggered()), SLOT(performClock()));

    connect(insideAction, SIGNAL(triggered()), SLOT(performInside()));
    connect(outsideAction, SIGNAL(triggered()), SLOT(performOutside()));
    connect(attractAction, SIGNAL(triggered()), SLOT(performAttract()));
    connect(repelAction, SIGNAL(triggered()), SLOT(performRepel()));
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

void Node::performInterface()
{
    Interface* entity = new Interface(this);
    entity->setTop(0);
    entity->setLeft(0);
    entity->setWidth(44);
    entity->setHeight(44);
    entity->adjust();

    connect(entity, SIGNAL(entityChanged(QncgGraphicsEntity*)), SIGNAL(entityChanged(QncgGraphicsEntity*)));
    connect(entity, SIGNAL(entitySelected(QncgGraphicsEntity*)), SIGNAL(entitySelected(QncgGraphicsEntity*)));

    addncgGraphicsEntity(entity);
}

void Node::performCompact()
{
    compact(50);

    adjust();
}

void Node::performFit()
{
    fit(50);

    adjust();
}

void Node::performClock()
{
    clock(50);

    adjust();
}

void Node::performInside()
{
    inside();

    adjust();
}

void Node::performOutside()
{
    outside();

    adjust();
}

void Node::performAttract()
{
    attract();

    adjust();
}

void Node::performRepel()
{
    repel();

    adjust();
}

void Node::draw(QPainter* painter)
{
//    painter->setRenderHint(QPainter::Antialiasing,true);

    painter->setBrush(QBrush(QColor("#C1FFC1")));
    painter->drawRect(4+20/2, 4+20/2, getWidth()-1-20, getHeight()-1-20);

//    painter->setRenderHint(QPainter::Antialiasing,false);

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
