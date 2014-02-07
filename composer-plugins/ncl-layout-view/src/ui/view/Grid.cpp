#include "Grid.h"

Grid::Grid(QGraphicsItem* parent, QGraphicsScene* scene)
  : QGraphicsRectItem(parent)
{

}

Grid::~Grid()
{

}

void Grid::setStep(qreal step)
{
  this->step = step;
}

qreal Grid::getStep()
{
  return step;
}

void Grid::setPen(QPen pen)
{
  this->pen = pen;
}

QPen Grid::getPen()
{
  return pen;
}

void Grid::paint( QPainter *painter,
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
