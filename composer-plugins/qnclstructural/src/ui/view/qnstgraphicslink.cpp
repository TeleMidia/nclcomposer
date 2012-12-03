#include "qnstgraphicslink.h"

QnstGraphicsLink::QnstGraphicsLink(QnstGraphicsEntity* parent)
  : QnstGraphicsNode(parent)
{
  setnstType(Qnst::Link);

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

QnstGraphicsLink::~QnstGraphicsLink()
{

}

void QnstGraphicsLink::createObjects()
{
  menu = new QnstMenu();
  menu->actionCut->setEnabled(true);
  menu->actionCopy->setEnabled(true);

  menu->actionDelete->setEnabled(true);

  menu->actionExport->setEnabled(true);
}

void QnstGraphicsLink::createConnections()
{
  // connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  // connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

  connect(menu, SIGNAL(cutRequested()), SIGNAL(cutRequested()));
  connect(menu, SIGNAL(copyRequested()), SIGNAL(copyRequested()));
  connect(menu, SIGNAL(pasteRequested()), SIGNAL(pasteRequested()));

  connect(menu, SIGNAL(deleteRequested()), SIGNAL(deleteRequested()));

  connect(menu, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

  connect(menu, SIGNAL(zoominRequested()), SIGNAL(zoominRequested()));
  connect(menu, SIGNAL(zoomoutRequested()), SIGNAL(zoomoutRequested()));
  connect(menu, SIGNAL(zoomresetRequested()), SIGNAL(zoomresetRequested()));
  connect(menu, SIGNAL(fullscreenRequested()), SIGNAL(fullscreenRequested()));
}

void QnstGraphicsLink::draw(QPainter* painter)
{
  painter->setRenderHint(QPainter::Antialiasing, true);

  painter->setBrush(Qt::black);

  painter->drawEllipse(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);

  // Draw MouseHover rectangle
  if (!isSelected() && hasMouseHover())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(QColor("#999999")), 0, Qt::DashLine)); // 0px = cosmetic border

    painter->drawRect(4, 4, getWidth(), getHeight());
  }

  if (isMoving())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

    painter->setRenderHint(QPainter::Antialiasing,false);
    painter->drawRect(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(),
                      getWidth()-1, getHeight()-1);
  }
}

void QnstGraphicsLink::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}

QString QnstGraphicsLink::getxConnector() const
{
  return xconnector;
}

void QnstGraphicsLink::setxConnector(QString xconnector)
{
  this->xconnector = xconnector;
}

QString QnstGraphicsLink::getxConnectorUID() const
{
  return xconnectorUID;
}

void QnstGraphicsLink::setxConnectorUID(QString xconnectorUID)
{
  this->xconnectorUID = xconnectorUID;
}

QMap<QString, QnstGraphicsBind*> QnstGraphicsLink::getActions() const
{
  return actions;
}

void QnstGraphicsLink::addAction(QnstGraphicsBind* action)
{
  if (action != NULL)
  {
    actions.insert(action->getnstUid(), action);
    action->setLink(this);
  }
}

void QnstGraphicsLink::removeAction(QnstGraphicsBind* action)
{
  if (action != NULL)
  {
    actions.remove(action->getnstUid());
    action->setLink(NULL);
  }
}

QMap<QString, QnstGraphicsBind*> QnstGraphicsLink::getConditions() const
{
  return conditions;
}

void QnstGraphicsLink::addCondition(QnstGraphicsBind* condition)
{
  if (condition != NULL)
  {
    conditions.insert(condition->getnstUid(), condition);
    condition->setLink(this);
  }
}

void QnstGraphicsLink::removeCondition(QnstGraphicsBind* condition)
{
  if (condition != NULL)
  {
    conditions.remove(condition->getnstUid());
    condition->setLink(NULL);
  }
}

void QnstGraphicsLink::setProperties(const QMap<QString, QString> &properties)
{
  QnstGraphicsNode::setProperties(properties);

  if (properties["xconnector"] != "")
    setxConnector(properties["xconnector"]);

  if (properties["xconnectorUID"] != "")
    setxConnectorUID(properties["xconnectorUID"]);
}

void QnstGraphicsLink::getProperties(QMap<QString, QString> &properties)
{
  QnstGraphicsNode::getProperties(properties);

  properties["xconnector"] = getxConnector();
  properties["xconnectorUID"] = getxConnectorUID();
}
