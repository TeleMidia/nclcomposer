#include "StructuralReference.h"

StructuralReference::StructuralReference(StructuralEntity* parent)
  : StructuralEdge(parent)
{
  setStructuralCategory(Structural::Edge);
  setStructuralType(Structural::Reference);
}

StructuralReference::~StructuralReference()
{

}

void StructuralReference::draw(QPainter* painter)
{
  if (getTail() != NULL && getHead() != NULL)
  {
    painter->setRenderHint(QPainter::Antialiasing, true);

    QLineF line = QLineF(QPointF(getTail()->getLeft() + getTail()->getWidth()/2,
                                 getTail()->getTop() + getTail()->getHeight()/2),
                         QPointF(getHead()->getLeft() + getHead()->getWidth()/2,
                                 getHead()->getTop() + getHead()->getHeight()/2));

    if (getTail()->getStructuralCategory() == Structural::Interface)
    {
      line.setP1(getStructuralParent()->mapFromItem(getTail()->getStructuralParent(), line.p1()));
    }

    if (getHead()->getStructuralCategory() == Structural::Interface)
    {
      line.setP2(getStructuralParent()->mapFromItem(getHead()->getStructuralParent(), line.p2()));
    }

    QPointF p1;

    QPointF pointa = line.p1();
    QPointF pointb = line.p2();

    QColor lineColor("#000000");

    if (getStructuralType() == Structural::Mapping)
      lineColor = QColor("#990099");

    if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
    {
      painter->setPen(QPen(QBrush(lineColor), 1, Qt::DashLine));

      painter->drawLine(4+6,4+6, 4+6+getWidth()-12, 4+6+getHeight()-12);

      painter->setBrush(QBrush(lineColor));
      painter->setPen(Qt::NoPen);

      p1 = QPointF(4+6+getWidth()-12, 4+6+getHeight()-12);

    }
    else if (pointa.x() > pointb.x() && pointa.y() < pointb.y())
    {
      painter->setPen(QPen(QBrush(lineColor), 1, Qt::DashLine));

      painter->drawLine(4+6+getWidth()-12,4+6, 4+6, 4+6+getHeight()-12);

      painter->setBrush(QBrush(lineColor));
      painter->setPen(Qt::NoPen);

      p1 = QPointF(4+6, 4+6+getHeight()-12);

    }
    else if (pointa.x() < pointb.x() && pointa.y() > pointb.y())
    {
      painter->setPen(QPen(QBrush(lineColor), 1, Qt::DashLine));

      painter->drawLine(4+6, 4+6+getHeight()-12, 4+6+getWidth()-12, 4+6);

      painter->setBrush(QBrush(lineColor));
      painter->setPen(Qt::NoPen);

      p1 = QPointF(4+6+getWidth()-12, 4+6);

    }
    else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
    {
      painter->setPen(QPen(QBrush(lineColor), 1, Qt::DashLine));

      painter->drawLine(4+6+getWidth()-12, 4+6+getHeight()-12, 4+6, 4+6);

      painter->setBrush(QBrush(lineColor));
      painter->setPen(Qt::NoPen);

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
}

void StructuralReference::delineate(QPainterPath* painter) const
{
  if (getTail() != NULL && getHead() != NULL)
  {
    QLineF line = QLineF(QPointF(getTail()->getLeft() + getTail()->getWidth()/2,
                                 getTail()->getTop() + getTail()->getHeight()/2),
                         QPointF(getHead()->getLeft() + getHead()->getWidth()/2,
                                 getHead()->getTop() + getHead()->getHeight()/2));

    if (getTail()->getStructuralCategory() == Structural::Interface)
    {
      if(getStructuralParent())
        line.setP1(getStructuralParent()->mapFromItem(getTail()->getStructuralParent(), line.p1()));
    }

    if (getHead()->getStructuralCategory() == Structural::Interface)
    {
      if(getStructuralParent())
        line.setP2(getStructuralParent()->mapFromItem(getHead()->getStructuralParent(), line.p2()));
    }

    QPointF p1;

    QPointF pointa = line.p1();
    QPointF pointb = line.p2();

    if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
    {
      painter->addEllipse(4,4,3,3);

      p1 = QPointF(4+6+getWidth()-12, 4+6+getHeight()-12);

    }
    else if (pointa.x() > pointb.x() && pointa.y() < pointb.y())
    {
      painter->addEllipse(4+getWidth()-3,4,3,3);

      p1 = QPointF(4+6, 4+6+getHeight()-12);

    }
    else if (pointa.x() < pointb.x() && pointa.y() > pointb.y())
    {
      painter->addEllipse(4, 4+getHeight()-3, 3, 3);

      p1 = QPointF(4+6+getWidth()-12, 4+6);

    }
    else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
    {
      painter->addEllipse(4+getWidth()-3, 4+getHeight()-3, 3, 3);

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

    painter->addPolygon(QPolygonF(polygon));
  }
}
