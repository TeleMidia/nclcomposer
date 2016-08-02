#include "StructuralLink.h"

StructuralLink::StructuralLink(StructuralEntity* parent)
  : StructuralNode(parent)
{
  setStructuralCategory(Structural::Node);
  setStructuralType(Structural::Link);

  // setnstType(Qnst::Aggregator);

  setResizable(false);

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
