#include "qnstgraphicsaggregator.h"

QnstGraphicsAggregator::QnstGraphicsAggregator(QnstGraphicsEntity* parent)
  : QnstGraphicsNode(parent)
{
  setnstType(Qnst::Aggregator);

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

QnstGraphicsAggregator::~QnstGraphicsAggregator()
{

}

void QnstGraphicsAggregator::createObjects()
{
  menu = new QnstMenu();
  menu->actionCut->setEnabled(true);
  menu->actionCopy->setEnabled(true);

  menu->actionDelete->setEnabled(true);

  menu->actionExport->setEnabled(true);
}

void QnstGraphicsAggregator::createConnections()
{
  connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

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

void QnstGraphicsAggregator::draw(QPainter* painter)
{
  painter->setRenderHint(QPainter::Antialiasing, true);

  painter->setBrush(Qt::black);

  painter->drawEllipse(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);

  if (isMoving())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

    painter->setRenderHint(QPainter::Antialiasing,false);
    painter->drawRect(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(),
                      getWidth()-1, getHeight()-1);
  }
}

void QnstGraphicsAggregator::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}
