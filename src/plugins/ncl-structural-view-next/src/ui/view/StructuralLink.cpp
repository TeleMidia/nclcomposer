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
    setTop(parent->getHeight()/2 - STR_DEFAULT_LINK_H/2);
    setLeft(parent->getWidth()/2 - STR_DEFAULT_LINK_W/2);
  }
  else
  {
    setTop(0);
    setLeft(0);
  }

  setWidth(STR_DEFAULT_LINK_W);
  setHeight(STR_DEFAULT_LINK_H);
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

void StructuralLink::adjust(bool avoidCollision,  bool rec)
{
  StructuralNode::adjust(avoidCollision, rec);

  QString tip = "";
  QString name = (getStructuralId() != "" ? getStructuralId() : "?");

  tip += "Link ("+name+") : "+getStructuralProperty(STR_PROPERTY_REFERENCE_XCONNECTOR_ID);

  setToolTip(tip);
}

void StructuralLink::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  StructuralNode::mouseDoubleClickEvent(event);

  setMoving(false);
  update();

  emit showLinkEditDialog(this);
}
