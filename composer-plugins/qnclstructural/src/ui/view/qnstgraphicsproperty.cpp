#include "qnstgraphicsproperty.h"

QnstGraphicsProperty::QnstGraphicsProperty(QnstGraphicsEntity* parent)
  : QnstGraphicsInterface(parent)
{
  setnstType(Qnst::Property);
}

QnstGraphicsProperty::~QnstGraphicsProperty()
{

}

void QnstGraphicsProperty::draw(QPainter* painter)
{
  QColor bg;
  QColor border;

  if (isRefer())
  {
    bg = QColor(153,153,153,75);
//    border = QColor(238,238,238);
    border = QColor(102,102,102,75);
  }
  else
  {
    bg = QColor(153,153,153);
    border = QColor(102,102,102);
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

void QnstGraphicsProperty::delineate(QPainterPath* painter) const
{
  painter->addRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);
}
