#include "StructuralGraphicsLink.h"

StructuralGraphicsLink::StructuralGraphicsLink(StructuralGraphicsEntity* parent)
  : StructuralGraphicsNode(parent)
{
  setnstType(Structural::Link);

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

StructuralGraphicsLink::~StructuralGraphicsLink()
{

}

void StructuralGraphicsLink::createObjects()
{
  menu = new StructuralMenu();
  menu->actionCut->setEnabled(true);
  menu->actionCopy->setEnabled(true);

  menu->actionDelete->setEnabled(true);

  menu->actionExport->setEnabled(true);
}

void StructuralGraphicsLink::createConnections()
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

void StructuralGraphicsLink::draw(QPainter* painter)
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

void StructuralGraphicsLink::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}

QString StructuralGraphicsLink::getxConnector() const
{
  return xconnector;
}

void StructuralGraphicsLink::setxConnector(QString xconnector)
{
  this->xconnector = xconnector;
}

QString StructuralGraphicsLink::getxConnectorUID() const
{
  return xconnectorUID;
}

void StructuralGraphicsLink::setxConnectorUID(QString xconnectorUID)
{
  this->xconnectorUID = xconnectorUID;
}

QMap<QString, StructuralGraphicsBind*> StructuralGraphicsLink::getActions() const
{
  return actions;
}

void StructuralGraphicsLink::addAction(StructuralGraphicsBind* action)
{
  if (action != NULL)
  {
    actions.insert(action->getnstUid(), action);
    action->setLink(this);
  }
}

void StructuralGraphicsLink::removeAction(StructuralGraphicsBind* action)
{
  if (action != NULL)
  {
    actions.remove ( action->getnstUid() );
    action->setLink(NULL);
  }
}

QMap<QString, StructuralGraphicsBind*> StructuralGraphicsLink::getConditions() const
{
  return conditions;
}

void StructuralGraphicsLink::addCondition(StructuralGraphicsBind* condition)
{
  if (condition != NULL)
  {
    conditions.insert(condition->getnstUid(), condition);
    condition->setLink(this);
  }
}

void StructuralGraphicsLink::removeCondition(StructuralGraphicsBind* condition)
{
  if (condition != NULL)
  {
    conditions.remove(condition->getnstUid());
    condition->setLink(NULL);
  }
}

void StructuralGraphicsLink::setProperties(const QMap<QString, QString> &properties)
{
  StructuralGraphicsNode::setProperties(properties);

  if (properties["xconnector"] != "")
    setxConnector(properties["xconnector"]);

  if (properties["xconnectorUID"] != "")
    setxConnectorUID(properties["xconnectorUID"]);
}

void StructuralGraphicsLink::getProperties(QMap<QString, QString> &properties)
{
  StructuralGraphicsNode::getProperties(properties);

  properties["xconnector"] = getxConnector();
  properties["xconnectorUID"] = getxConnectorUID();
}
