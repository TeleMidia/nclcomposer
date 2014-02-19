#include "StructuralGraphicsArea.h"

StructuralGraphicsArea::StructuralGraphicsArea(StructuralGraphicsEntity* parent)
  : StructuralGraphicsInterface(parent)
{
  setnstType(Structural::Area);

  setnstId("");
}

StructuralGraphicsArea::~StructuralGraphicsArea()
{

}

void StructuralGraphicsArea::draw(QPainter* painter)
{
  QColor bg;
  QColor border;

  if (isRefer())
  {
    bg = QColor(244,164,96,75);
//    border = QColor(238,238,238);
    border = QColor(153,153,153, 75);
  }
  else
  {
    bg = QColor(244,164,96);
    border = QColor(153,153,153);
  }
  painter->setBrush(QBrush(bg));
  painter->setPen(QPen(QBrush(border), 0, Qt::SolidLine));

  painter->drawRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);

  drawMouseHoverHighlight(painter); // This should not be HERE!!

  if (isMoving())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

    painter->drawRect(getMoveLeft()+4-getLeft(),
                      getMoveTop()+4-getTop(),
                      getWidth()-1,
                      getHeight()-1);
  }
}

void StructuralGraphicsArea::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}
