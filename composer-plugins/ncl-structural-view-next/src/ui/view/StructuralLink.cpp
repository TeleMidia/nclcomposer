#include "StructuralLink.h"

StructuralLink::StructuralLink(StructuralEntity* parent)
  : StructuralNode(parent)
{
  setnstType(Structural::Node);
  setnstSubtype(Structural::Link);

  // setnstType(Qnst::Aggregator);

  setResizable(false);

  createObjects();
  createConnections();

  /* Default position for aggregator */
  if(parent)
  {
    setTop(parent->getHeight()/2 - DEFAULT_AGGREGATOR_HEIGHT/2);
    setLeft(parent->getWidth()/2 - DEFAULT_AGGREGATOR_WIDTH/2);
  }
  else
  {
    setTop(0);
    setLeft(0);
  }

  setWidth(DEFAULT_AGGREGATOR_WIDTH);
  setHeight(DEFAULT_AGGREGATOR_HEIGHT);
}

StructuralLink::~StructuralLink()
{

}

void StructuralLink::createObjects()
{
  menu = new StructuralMenu();
//  menu->_cutAction->setEnabled(true);
//  menu->_copyAction->setEnabled(true);

//  menu->_deleteAction->setEnabled(true);

//  menu->actionExport->setEnabled(true);
}

void StructuralLink::createConnections()
{
  // connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  // connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

}

void StructuralLink::draw(QPainter* painter)
{
  painter->setRenderHint(QPainter::Antialiasing, true);

  painter->setBrush(Qt::black);

  painter->drawEllipse(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);

  drawMouseHoverHighlight(painter); // This should not be HERE!!

  if (isMoving())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

    painter->setRenderHint(QPainter::Antialiasing,false);
    painter->drawRect(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(),
                      getWidth()-1, getHeight()-1);
  }
}

void StructuralLink::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}

QString StructuralLink::getxConnector() const
{
  return xconnector;
}

void StructuralLink::setxConnector(QString xconnector)
{
  this->xconnector = xconnector;
}

QString StructuralLink::getxConnectorUID() const
{
  return xconnectorUID;
}

void StructuralLink::setxConnectorUID(QString xconnectorUID)
{
  this->xconnectorUID = xconnectorUID;
}

QMap<QString, StructuralBind*> StructuralLink::getActions() const
{
  return actions;
}

void StructuralLink::addAction(StructuralBind* action)
{
  if (action != NULL)
  {
    actions.insert(action->getnstUid(), action);
    action->setLink(this);
  }
}

void StructuralLink::removeAction(StructuralBind* action)
{
  if (action != NULL)
  {
    actions.remove(action->getnstUid());
    action->setLink(NULL);
  }
}

QMap<QString, StructuralBind*> StructuralLink::getConditions() const
{
  return conditions;
}

void StructuralLink::addCondition(StructuralBind* condition)
{
  if (condition != NULL)
  {
    conditions.insert(condition->getnstUid(), condition);
    condition->setLink(this);
  }
}

void StructuralLink::removeCondition(StructuralBind* condition)
{
  if (condition != NULL)
  {
    conditions.remove(condition->getnstUid());
    condition->setLink(NULL);
  }
}
