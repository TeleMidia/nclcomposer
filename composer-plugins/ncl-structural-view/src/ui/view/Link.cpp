#include "Link.h"

QnstLink::QnstLink(QnstEntity* parent)
  : QnstNode(parent)
{
  setnstType(Qnst::Node);
  setnstSubtype(Qnst::Link);

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

QnstLink::~QnstLink()
{

}

void QnstLink::createObjects()
{
  menu = new QnstMenu();
  menu->actionCut->setEnabled(true);
  menu->actionCopy->setEnabled(true);

  menu->actionDelete->setEnabled(true);

  menu->actionExport->setEnabled(true);
}

void QnstLink::createConnections()
{
  // connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  // connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

}

void QnstLink::draw(QPainter* painter)
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

void QnstLink::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}

QString QnstLink::getxConnector() const
{
  return xconnector;
}

void QnstLink::setxConnector(QString xconnector)
{
  this->xconnector = xconnector;
}

QString QnstLink::getxConnectorUID() const
{
  return xconnectorUID;
}

void QnstLink::setxConnectorUID(QString xconnectorUID)
{
  this->xconnectorUID = xconnectorUID;
}

QMap<QString, QnstBind*> QnstLink::getActions() const
{
  return actions;
}

void QnstLink::addAction(QnstBind* action)
{
  if (action != NULL)
  {
    actions.insert(action->getnstUid(), action);
    action->setLink(this);
  }
}

void QnstLink::removeAction(QnstBind* action)
{
  if (action != NULL)
  {
    actions.remove(action->getnstUid());
    action->setLink(NULL);
  }
}

QMap<QString, QnstBind*> QnstLink::getConditions() const
{
  return conditions;
}

void QnstLink::addCondition(QnstBind* condition)
{
  if (condition != NULL)
  {
    conditions.insert(condition->getnstUid(), condition);
    condition->setLink(this);
  }
}

void QnstLink::removeCondition(QnstBind* condition)
{
  if (condition != NULL)
  {
    conditions.remove(condition->getnstUid());
    condition->setLink(NULL);
  }
}
