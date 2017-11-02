#include "LayoutGrid.h"

LayoutGrid::LayoutGrid (QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsRectItem (parent)
{
  Q_UNUSED (scene)
}

LayoutGrid::~LayoutGrid () {}

void
LayoutGrid::setStep (qreal step)
{
  this->_step = step;
}

qreal
LayoutGrid::getStep ()
{
  return _step;
}

void
LayoutGrid::setPen (QPen pen)
{
  this->_pen = pen;
}

QPen
LayoutGrid::getPen ()
{
  return _pen;
}

void
LayoutGrid::paint (QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
  Q_UNUSED (option)
  Q_UNUSED (widget)

  painter->setPen (_pen);

  QRectF r = rect ();
  int w = 0;

  while (w < r.width ())
  {
    painter->drawLine (w, 0, w, r.height ());
    w += _step;
  }

  int h = 0;

  while (h < r.height ())
  {
    painter->drawLine (0, h, r.width (), h);
    h += _step;
  }
}
