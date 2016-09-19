#include "StructuralEdge.h"

StructuralEdge::StructuralEdge(StructuralEntity* parent)
  : StructuralEntity(parent)
{
  setStructuralCategory(Structural::Edge);
  setStructuralType(Structural::NoType);

  setResizable(false);
  setMoveable(false);

  setAlfa(0);
  setAngle(0);

  setTail(NULL);
  setHead(NULL);
}

StructuralEdge::~StructuralEdge()
{

}

qreal StructuralEdge::getAlfa() const
{
  return _alfa;
}

void StructuralEdge::setAlfa(qreal alfa)
{
  _alfa = alfa;
}

qreal StructuralEdge::getAngle() const
{
  return _angle;
}

void StructuralEdge::setAngle(qreal angle)
{
  this->_angle = angle;
}

qreal StructuralEdge::getTailTop() const
{
  return _tailTop;
}

void StructuralEdge::setTailTop(qreal tailTop)
{
  _tailTop = tailTop;
}

qreal StructuralEdge::getTailLeft() const
{
  return _tailLeft;
}

void StructuralEdge::setTailLeft(qreal tailLeft)
{
  _tailLeft = tailLeft;
}

qreal StructuralEdge::getHeadTop() const
{
  return _headTop;
}

void StructuralEdge::setHeadTop(qreal headTop)
{
  _headTop = headTop;
}

qreal StructuralEdge::getHeadLeft() const
{
  return _headLeft;
}

void StructuralEdge::setHeadLeft(qreal headLeft)
{
  _headLeft = headLeft;
}

StructuralEntity* StructuralEdge::getTail() const
{
  return _tail;
}

void StructuralEdge::setTail(StructuralEntity* entity)
{
  _tail = entity;
}

StructuralEntity* StructuralEdge::getHead() const
{
  return _head;
}

void StructuralEdge::setHead(StructuralEntity* entity)
{
  _head = entity;
}

void StructuralEdge::adjust(bool collision,  bool recursion)
{
  StructuralEntity::adjust(collision, recursion);

  // Adjusting position...
  StructuralEntity* parent = getStructuralParent();

  if (parent != NULL) {

    qreal angle = getAngle();

    StructuralEntity* tail = getTail();
    StructuralEntity* head = getHead();

    if (tail != NULL && head != NULL) {
      QLineF line = QLineF(QPointF(tail->getLeft() + tail->getWidth()/2,
                                   tail->getTop() + tail->getHeight()/2),
                           QPointF(head->getLeft() + head->getWidth()/2,
                                   head->getTop() + head->getHeight()/2));

      if (tail->getStructuralCategory() == Structural::Interface)
        if(parent != NULL)
          line.setP1(parent->mapFromItem(tail->getStructuralParent(), line.p1()));

      if (head->getStructuralCategory() == Structural::Interface)
        if(parent != NULL)
          line.setP2(parent->mapFromItem(head->getStructuralParent(), line.p2()));

      adjustBox(line);

      if (tail != head) {
        tail->setSelectable(false);
        head->setSelectable(false);

        adjustExtreme(tail, line, 0.0,  0.01, angle);
        adjustExtreme(head, line, 1.0, -0.01, angle);

        tail->setSelectable(true);
        head->setSelectable(true);
      }
    }
  }
}

void StructuralEdge::adjustBox(QLineF line)
{
  QPointF ptail = line.p1();
  QPointF phead = line.p2();

  int x;
  int y;
  int w;
  int h;

  if (ptail.x() <= phead.x() && ptail.y() <= phead.y())
  {
    x = ptail.x();
    y = ptail.y();
    w = phead.x() - ptail.x();
    h = phead.y() - ptail.y();
  }
  else if (ptail.x() > phead.x() && ptail.y() <= phead.y())
  {
    x = phead.x();
    y = ptail.y();
    w = ptail.x() - phead.x();
    h = phead.y() - ptail.y();
  }
  else if (ptail.x() <= phead.x() && ptail.y() > phead.y())
  {
    x = ptail.x();
    y = phead.y();
    w = phead.x() - ptail.x();
    h = ptail.y() - phead.y();
  }
  else if (ptail.x() > phead.x() && ptail.y() > phead.y())
  {
    x = phead.x();
    y = phead.y();
    w = ptail.x() - phead.x();
    h = ptail.y() - phead.y();
  }

  setTop(y - STR_DEFAULT_PADDING);
  setLeft(x - STR_DEFAULT_PADDING);
  setWidth(w + 2*STR_DEFAULT_PADDING);
  setHeight(h + 2*STR_DEFAULT_PADDING);

  setTailTop(ptail.y());
  setTailLeft(ptail.x());

  setHeadTop(phead.y());
  setHeadLeft(phead.x());
}

void StructuralEdge::adjustExtreme(StructuralEntity* extreme, QLineF line, qreal index, qreal step, qreal angle)
{
  int max = 100;
  int n = 0;

  while(extreme->collidesWithItem(this)) {
    index += step;

    QPointF p;

    if (angle != 0) {
      qreal r;
      qreal len;

      qreal alfa;
      qreal beta;
      qreal gama;

      QPointF center;

      r = line.length()/(::sin(((angle/2)*PI)/180.0)*2);
      len = (angle*PI*r/180.0)*index;

      alfa = (180.0*len)/(PI*r);
      beta = (180.0 - angle)/2 + (360.0 - line.angle());
      gama = (180.0 - beta - alfa);

      center.setX(line.p2().x() - ::cos((180.0-beta-angle)*PI/180.0)*r);
      center.setY(line.p2().y() + ::sin((180.0-beta-angle)*PI/180.0)*r);

      p.setX(center.x() + ::cos((gama)*PI/180.0)*r);
      p.setY(center.y() - ::sin((gama)*PI/180.0)*r);

      if (extreme == getTail())
        setAlfa(alfa*(1/index - 1));
      else
        setAlfa(alfa);

    } else {
      p = line.pointAt(index);
    }

    if (extreme == getTail())
      adjustBox(QLineF(p, line.p2()));
    else
      adjustBox(QLineF(line.p1(), p));

    if (++n > max)
      break;
  }
}
