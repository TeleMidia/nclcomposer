#include "StructuralLink.h"

StructuralLink::StructuralLink(StructuralEntity* parent)
  : StructuralNode(parent)
{
  setStructuralCategory(Structural::Node);
  setStructuralType(Structural::Link);

  setWidth(STR_DEFAULT_LINK_W);
  setHeight(STR_DEFAULT_LINK_H);

  setResizable(false);
}

StructuralLink::~StructuralLink()
{

}

void StructuralLink::draw(QPainter* painter)
{
  painter->setRenderHint(QPainter::Antialiasing, true);

  QColor drawColor = QColor(StructuralUtil::getColor(getStructuralType()));

  if (!getError().isEmpty() ||
      !getWarning().isEmpty()) {

    if (!getError().isEmpty()) {
      drawColor = QString(STR_DEFAULT_ALERT_ERROR_COLOR);
    } else {

      drawColor = QString(STR_DEFAULT_ALERT_WARNING_COLOR);
    }

    drawColor = drawColor.light();
  }

  painter->setBrush(drawColor);
  painter->setPen(QPen(drawColor.darker(), 0));

  painter->drawEllipse(STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_LINK_PADDING,
                       STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_LINK_PADDING,
                       getWidth() - 2*STR_DEFAULT_LINK_PADDING,
                       getHeight() - 2*STR_DEFAULT_LINK_PADDING);

  if (isMoving())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0));

    painter->setRenderHint(QPainter::Antialiasing,false);

    painter->drawRect(getMoveLeft() + STR_DEFAULT_ENTITY_PADDING - getLeft(),
                      getMoveTop() + STR_DEFAULT_ENTITY_PADDING - getTop(),
                      getWidth(),
                      getHeight());
  }
}

void StructuralLink::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
//  StructuralNode::mouseDoubleClickEvent(event);

  emit performedEdit(this);
}
