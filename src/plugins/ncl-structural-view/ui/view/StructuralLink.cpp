#include "StructuralLink.h"

StructuralLink::StructuralLink (StructuralEntity *parent)
    : StructuralNode (parent)
{
  setCategory (Structural::Node);
  setStructuralType (Structural::Link);

  setWidth (ST_DEFAULT_LINK_W);
  setHeight (ST_DEFAULT_LINK_H);

  setResizable (false);
}

StructuralLink::~StructuralLink () {}

void
StructuralLink::draw (QPainter *painter)
{
  painter->setRenderHint (QPainter::Antialiasing, true);

  QColor drawColor = QColor (StructuralUtil::color (structuralType ()));

  if (!error ().isEmpty () || !warning ().isEmpty ())
  {
    if (!error ().isEmpty ())
    {
      drawColor = QString (ST_DEFAULT_ALERT_ERROR_COLOR);
    }
    else
    {
      drawColor = QString (ST_DEFAULT_ALERT_WARNING_COLOR);
    }

    drawColor = drawColor.light ();
  }

  painter->setBrush (drawColor);
  painter->setPen (QPen (drawColor.darker (), 0));

  painter->drawEllipse (ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_LINK_PADDING,
                        ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_LINK_PADDING,
                        width () - 2 * ST_DEFAULT_LINK_PADDING,
                        height () - 2 * ST_DEFAULT_LINK_PADDING);

  if (isMoving ())
  {
    painter->setBrush (Qt::NoBrush);
    painter->setPen (QPen (QBrush (Qt::black), 0));

    painter->setRenderHint (QPainter::Antialiasing, false);

    painter->drawRect (moveLeft () + ST_DEFAULT_ENTITY_PADDING - left (),
                       moveTop () + ST_DEFAULT_ENTITY_PADDING - top (),
                       width (), height ());
  }
}

void
StructuralLink::mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED (event);
  //  StructuralNode::mouseDoubleClickEvent(event);

  emit performedEdit (this);
}
