#include "StructuralViewLink.h"

StructuralViewLink::StructuralViewLink(StructuralEntity* parent)
  : StructuralEntity(parent)
{
  setSelectable(false);
  setResizable(false);
  setMoveable(false);
}

StructuralViewLink::~StructuralViewLink()
{

}

QLineF StructuralViewLink::getLine() const
{
  return line;
}

void StructuralViewLink::setLine(QLineF line)
{
  this->line = line;

  adjust();
}

void StructuralViewLink::adjust(bool avoidCollision, bool rec)
{
  QPointF pointa = line.p1();
  QPointF pointb = line.p2();

  if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
  {
    setTop(pointa.y()-6);
    setLeft(pointa.x()-6);
    setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
    setHeight((pointb.y()-6)-(pointa.y()-6) + 12);
  }
  else if (pointa.x() > pointb.x() && pointa.y() < pointb.y())
  {
    setTop(pointa.y()-6);
    setLeft(pointb.x()-6);
    setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
    setHeight((pointb.y()-6)-(pointa.y()-6) + 12);
  }
  else if (pointa.x() < pointb.x() && pointa.y() > pointb.y())
  {
    setTop(pointb.y()-6);
    setLeft((pointa.x()-6));
    setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
    setHeight((pointa.y()-6)-(pointb.y()-6) + 12);
  }
  else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
  {
    setTop(pointb.y()-6);
    setLeft(pointb.x()-6);
    setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
    setHeight((pointa.y()-6)-(pointb.y()-6) + 12);
  }

  if (scene() != NULL)
    scene()->update();
}

void StructuralViewLink::draw(QPainter* painter)
{
  painter->setRenderHint(QPainter::Antialiasing, true);

  QPointF p1;

  QPointF pointa = line.p1();
  QPointF pointb = line.p2();

  if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
  {
    painter->setPen(QPen(QBrush(QColor("#000000")), 1, Qt::DashLine));

    painter->drawLine(4+6,4+6, 4+6+getWidth()-12-2, 4+6+getHeight()-12-2);

    painter->setBrush(QBrush(QColor("#000000")));
    painter->setPen(Qt::NoPen);

    painter->drawEllipse(4,4,12,12);

    p1 = QPointF(4+6+getWidth()-12, 4+6+getHeight()-12);

  }
  else if (pointa.x() > pointb.x() && pointa.y() < pointb.y())
  {
    painter->setPen(QPen(QBrush(QColor("#000000")), 1, Qt::DashLine));

    painter->drawLine(4+6+getWidth()-12,4+6, 4+6, 4+6+getHeight()-12);

    painter->setBrush(QBrush(QColor("#000000")));
    painter->setPen(Qt::NoPen);

    painter->drawEllipse(4+getWidth()-12,4,12,12);

    p1 = QPointF(4+6, 4+6+getHeight()-12);

  }
  else if (pointa.x() < pointb.x() && pointa.y() > pointb.y())
  {
    painter->setPen(QPen(QBrush(QColor("#000000")), 1, Qt::DashLine));

    painter->drawLine(4+6, 4+6+getHeight()-12, 4+6+getWidth()-12, 4+6);

    painter->setBrush(QBrush(QColor("#000000")));
    painter->setPen(Qt::NoPen);

    painter->drawEllipse(4, 4+getHeight()-12, 12, 12);

    p1 = QPointF(4+6+getWidth()-12, 4+6);

  }
  else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
  {
    painter->setPen(QPen(QBrush(QColor("#000000")), 1, Qt::DashLine));

    painter->drawLine(4+6+getWidth()-12, 4+6+getHeight()-12, 4+6, 4+6);

    painter->setBrush(QBrush(QColor("#000000")));
    painter->setPen(Qt::NoPen);

    painter->drawEllipse(4+getWidth()-12, 4+getHeight()-12, 12, 12);

    p1 = QPointF(4+6, 4+6);
  }

  double angle = ::acos(line.dx() / line.length());

  if (line.dy() >= 0)
  {
    angle = (PI * 2) - angle;
  }

  QPointF p2 = p1 - QPointF(sin(angle + PI / 3) * 12, cos(angle + PI / 3) * 12);
  QPointF p3 = p1 - QPointF(sin(angle + PI - PI / 3) * 12, cos(angle + PI - PI / 3) * 12);

  QVector<QPointF> polygon;

  polygon.append(p1);
  polygon.append(p2);
  polygon.append(p3);

  painter->drawPolygon(QPolygonF(polygon));
}

void StructuralViewLink::delineate(QPainterPath* painter) const
{
  // nothing to do
}

void StructuralViewLink::move(QGraphicsSceneMouseEvent* event)
{
  // nothing to do
}

void StructuralViewLink::resize(QGraphicsSceneMouseEvent* event)
{
  // nothing to do
}
