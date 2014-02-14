#include "LayoutGrid.h"

LayoutGrid::LayoutGrid(QGraphicsItem* parent, QGraphicsScene* scene)
  : QGraphicsRectItem(parent)
{

}

LayoutGrid::~LayoutGrid()
{

}

void LayoutGrid::setStep(qreal step)
{
  this->step = step;
}

qreal LayoutGrid::getStep()
{
  return step;
}

void LayoutGrid::setPen(QPen pen)
{
  this->pen = pen;
}

QPen LayoutGrid::getPen()
{
  return pen;
}

void LayoutGrid::paint( QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget)
{
  painter->setPen(pen);

  QRectF r = rect();
  int w = 0;

  while (w < r.width())
  {
    painter->drawLine(w,0,w,r.height());
    w += step;
  }

  int h = 0;

  while (h < r.height())
  {
    painter->drawLine(0,h,r.width(),h);
    h  += step;
  }
}
