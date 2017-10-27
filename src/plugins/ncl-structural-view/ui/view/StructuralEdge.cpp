#include "StructuralEdge.h"

#include <math.h>

StructuralEdge::StructuralEdge (StructuralEntity *parent)
    : StructuralEntity (parent)
{
  setCategory (Structural::Edge);
  setType (Structural::NoType);

  setResizable (false);
  setMoveable (false);

  setAlfa (0);
  setAngle (0);

  setTail (NULL);
  setHead (NULL);
}

StructuralEdge::~StructuralEdge () {}

qreal
StructuralEdge::getAlfa () const
{
  return _alfa;
}

void
StructuralEdge::setAlfa (qreal alfa)
{
  _alfa = alfa;
}

qreal
StructuralEdge::getAngle () const
{
  return _angle;
}

void
StructuralEdge::setAngle (qreal angle)
{
  _angle = angle;

  setProperty (ST_ATTR_EDGE_ANGLE, QString::number (angle));
}

qreal
StructuralEdge::getTailTop () const
{
  return _tailTop;
}

void
StructuralEdge::setTailTop (qreal tailTop)
{
  _tailTop = tailTop;
}

qreal
StructuralEdge::getTailLeft () const
{
  return _tailLeft;
}

void
StructuralEdge::setTailLeft (qreal tailLeft)
{
  _tailLeft = tailLeft;
}

qreal
StructuralEdge::getHeadTop () const
{
  return _headTop;
}

void
StructuralEdge::setHeadTop (qreal headTop)
{
  _headTop = headTop;
}

qreal
StructuralEdge::getHeadLeft () const
{
  return _headLeft;
}

void
StructuralEdge::setHeadLeft (qreal headLeft)
{
  _headLeft = headLeft;
}

StructuralEntity *
StructuralEdge::getTail () const
{
  return _tail;
}

void
StructuralEdge::setTail (StructuralEntity *tail)
{
  _tail = tail;
}

StructuralEntity *
StructuralEdge::getHead () const
{
  return _head;
}

void
StructuralEdge::setHead (StructuralEntity *head)
{
  _head = head;
}

void
StructuralEdge::adjust (bool collision, bool recursion)
{
  StructuralEntity::adjust (collision, recursion);

  // Adjusting properties...
  setAngle (getProperty (ST_ATTR_EDGE_ANGLE).toDouble ());

  // Adjusting position...
  StructuralEntity *parent = getParent ();

  if (parent != NULL || !ST_OPT_WITH_BODY)
  {
    qreal angle = getAngle ();

    StructuralEntity *tail = getTail ();
    StructuralEntity *head = getHead ();

    if (tail != NULL && head != NULL)
    {
      QLineF line
          = QLineF (QPointF (tail->getLeft () + tail->getWidth () / 2,
                             tail->getTop () + tail->getHeight () / 2),
                    QPointF (head->getLeft () + head->getWidth () / 2,
                             head->getTop () + head->getHeight () / 2));

      if (tail->getCategory () == Structural::Interface)
      {
        if (parent != NULL)
          line.setP1 (parent->mapFromItem (tail->getParent (), line.p1 ()));
        else if (tail->getParent () != NULL)
          line.setP1 (tail->getParent ()->mapToScene (line.p1 ()));
      }

      if (head->getCategory () == Structural::Interface)
      {
        if (parent != NULL)
          line.setP2 (parent->mapFromItem (head->getParent (), line.p2 ()));
        else if (head->getParent () != NULL)
          line.setP2 (head->getParent ()->mapToScene (line.p2 ()));
      }

      adjustBox (line);

      if (tail != head)
      {
        tail->setSelectable (false);
        head->setSelectable (false);

        adjustExtreme (tail, line, 0.0, 0.01, angle);
        line.setP1 (QPointF (getTailLeft (), getTailTop ()));

        adjustExtreme (head, line, 1.0, -0.01, angle);
        line.setP2 (QPointF (getHeadLeft (), getHeadTop ()));

        tail->setSelectable (true);
        head->setSelectable (true);
      }
    }
  }
}

void
StructuralEdge::adjustBox (QLineF line)
{
  QPointF ptail = line.p1 ();
  QPointF phead = line.p2 ();

  qreal x = 0.0, y = 0.0, w = 0.0, h = 0.0;

  if (ptail.x () <= phead.x () && ptail.y () <= phead.y ())
  {
    x = ptail.x ();
    y = ptail.y ();
    w = phead.x () - ptail.x ();
    h = phead.y () - ptail.y ();
  }
  else if (ptail.x () > phead.x () && ptail.y () <= phead.y ())
  {
    x = phead.x ();
    y = ptail.y ();
    w = ptail.x () - phead.x ();
    h = phead.y () - ptail.y ();
  }
  else if (ptail.x () <= phead.x () && ptail.y () > phead.y ())
  {
    x = ptail.x ();
    y = phead.y ();
    w = phead.x () - ptail.x ();
    h = ptail.y () - phead.y ();
  }
  else if (ptail.x () > phead.x () && ptail.y () > phead.y ())
  {
    x = phead.x ();
    y = phead.y ();
    w = ptail.x () - phead.x ();
    h = ptail.y () - phead.y ();
  }

  setTailTop (ptail.y ());
  setTailLeft (ptail.x ());
  setHeadTop (phead.y ());
  setHeadLeft (phead.x ());

  setTop (y);
  setLeft (x);
  setWidth (w);
  setHeight (h);
}

void
StructuralEdge::adjustExtreme (StructuralEntity *extreme, QLineF line,
                               qreal index, qreal step, qreal angle)
{
  int max = 100;
  int n = 0;

  while (extreme->collidesWithItem (this))
  {
    index += step;

    QPointF p;

    if (angle != 0)
    {
      qreal r;
      qreal len;

      qreal alfa;
      qreal beta;
      qreal gama;

      QPointF center;

      r = line.length () / (::sin (((angle / 2) * PI) / 180.0) * 2);
      len = (angle * PI * r / 180.0) * index;

      alfa = (180.0 * len) / (PI * r);
      beta = (180.0 - angle) / 2 + (360.0 - line.angle ());
      gama = (180.0 - beta - alfa);

      center.setX (line.p2 ().x ()
                   - ::cos ((180.0 - beta - angle) * PI / 180.0) * r);
      center.setY (line.p2 ().y ()
                   + ::sin ((180.0 - beta - angle) * PI / 180.0) * r);

      p.setX (center.x () + ::cos ((gama)*PI / 180.0) * r);
      p.setY (center.y () - ::sin ((gama)*PI / 180.0) * r);

      if (extreme == getTail ())
        setAlfa (alfa * (1 / index - 1));
      else
        setAlfa (alfa);
    }
    else
    {
      p = line.pointAt (index);
    }

    if (extreme == getTail ())
      adjustBox (QLineF (p, line.p2 ()));
    else
      adjustBox (QLineF (line.p1 (), p));

    if (++n > max)
      break;
  }
}

QLineF
StructuralEdge::getDrawLine (qreal padding) const
{
  QPointF ptail = QPointF (getTailLeft (), getTailTop ());
  QPointF phead = QPointF (getHeadLeft (), getHeadTop ());

  QLineF drawLine;

  int x = 0, y = 0, z = 0, w = 0;

  if (ptail.x () <= phead.x () && ptail.y () <= phead.y ())
  {
    x = ST_DEFAULT_ENTITY_PADDING;
    y = ST_DEFAULT_ENTITY_PADDING;
    z = getWidth () + ST_DEFAULT_ENTITY_PADDING;
    w = getHeight () + ST_DEFAULT_ENTITY_PADDING;
  }
  else if (ptail.x () > phead.x () && ptail.y () <= phead.y ())
  {
    x = getWidth () + ST_DEFAULT_ENTITY_PADDING;
    y = ST_DEFAULT_ENTITY_PADDING;
    z = ST_DEFAULT_ENTITY_PADDING;
    w = getHeight () + ST_DEFAULT_ENTITY_PADDING;
  }
  else if (ptail.x () <= phead.x () && ptail.y () > phead.y ())
  {
    x = ST_DEFAULT_ENTITY_PADDING;
    y = getHeight () + ST_DEFAULT_ENTITY_PADDING;
    z = getWidth () + ST_DEFAULT_ENTITY_PADDING;
    w = ST_DEFAULT_ENTITY_PADDING;
  }
  else if (ptail.x () > phead.x () && ptail.y () > phead.y ())
  {
    x = getWidth () + ST_DEFAULT_ENTITY_PADDING;
    y = getHeight () + ST_DEFAULT_ENTITY_PADDING;
    z = ST_DEFAULT_ENTITY_PADDING;
    w = ST_DEFAULT_ENTITY_PADDING;
  }

  drawLine.setPoints (QPointF (x, y), QPointF (z, w));
  drawLine.setPoints (drawLine.pointAt (padding / drawLine.length ()),
                      drawLine.pointAt (1 - padding / drawLine.length ()));

  return drawLine;
}

void
StructuralEdge::draw (QPainter *painter)
{
  painter->setRenderHint (QPainter::Antialiasing, true);

  // Setting...
  QLineF drawLine = getDrawLine (ST_DEFAULT_EDGE_PADDING);

  // Drawing line...
  painter->setPen (
      QPen (QBrush (QColor (StructuralUtil::getColor (getType ()))), 1,
            Qt::DashLine));
  painter->drawLine (drawLine);

  // Drawing tail...
  // nothing...

  // Drawing head...
  painter->setBrush (QBrush (QColor (StructuralUtil::getColor (getType ()))));
  painter->setPen (Qt::NoPen);

  qreal angle;

  if (drawLine.dy () < 0)
    angle = ::acos (drawLine.dx () / drawLine.length ());
  else
    angle = (PI * 2) - ::acos (drawLine.dx () / drawLine.length ());

  QPointF a = drawLine.p2 ();

  QPointF b = a - QPointF (sin (angle + PI / 3) * ST_DEFAULT_EDGE_HEAD_W,
                           cos (angle + PI / 3) * ST_DEFAULT_EDGE_HEAD_H);
  QPointF c = a - QPointF (sin (angle + PI - PI / 3) * ST_DEFAULT_EDGE_HEAD_W,
                           cos (angle + PI - PI / 3) * ST_DEFAULT_EDGE_HEAD_H);

  QVector<QPointF> polygon;
  polygon.append (a);
  polygon.append (b);
  polygon.append (c);

  painter->drawPolygon (QPolygonF (polygon));
}

void
StructuralEdge::delineate (QPainterPath *painter) const
{
  // Setting...
  QLineF boxLine = getDrawLine (0);
  QLineF drawLine = getDrawLine (ST_DEFAULT_EDGE_PADDING);

  // Delineating line
  // nothing

  // Delineating tail...
  painter->addEllipse (boxLine.p1 ().x (), boxLine.p1 ().y (), 1, 1);

  // Delineating head...
  painter->addEllipse (boxLine.p2 ().x (), boxLine.p2 ().y (), 1, 1);

  qreal angle;

  if (drawLine.dy () < 0)
    angle = ::acos (drawLine.dx () / drawLine.length ());
  else
    angle = (PI * 2) - ::acos (drawLine.dx () / drawLine.length ());

  QPointF a = drawLine.p2 ();

  QPointF b = a - QPointF (sin (angle + PI / 3) * ST_DEFAULT_EDGE_HEAD_W,
                           cos (angle + PI / 3) * ST_DEFAULT_EDGE_HEAD_H);
  QPointF c = a - QPointF (sin (angle + PI - PI / 3) * ST_DEFAULT_EDGE_HEAD_W,
                           cos (angle + PI - PI / 3) * ST_DEFAULT_EDGE_HEAD_H);

  QVector<QPointF> polygon;
  polygon.append (a);
  polygon.append (b);
  polygon.append (c);

  painter->addPolygon (QPolygonF (polygon));
}
