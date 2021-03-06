#include "StructuralEdge.h"

#include <math.h>

StructuralEdge::StructuralEdge (StructuralEntity *parent)
    : StructuralEntity (parent)
{
  setCategory (Structural::Edge);
  setStructuralType (Structural::NoType);

  setResizable (false);
  setMoveable (false);

  setAlfa (0);
  setAngle (0);

  setOrigin (nullptr);
  setDestination (nullptr);
}

qreal
StructuralEdge::alfa () const
{
  return _alfa;
}

void
StructuralEdge::setAlfa (qreal alfa)
{
  _alfa = alfa;
}

qreal
StructuralEdge::angle () const
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
StructuralEdge::origTop () const
{
  return _origTop;
}

void
StructuralEdge::setOrigTop (qreal origTop)
{
  _origTop = origTop;
}

qreal
StructuralEdge::origLeft () const
{
  return _origLeft;
}

void
StructuralEdge::setOrigLeft (qreal origLeft)
{
  _origLeft = origLeft;
}

qreal
StructuralEdge::destTop () const
{
  return _destTop;
}

void
StructuralEdge::setDestTop (qreal destTop)
{
  _destTop = destTop;
}

qreal
StructuralEdge::destLeft () const
{
  return _destLeft;
}

void
StructuralEdge::setDestLeft (qreal destLeft)
{
  _destLeft = destLeft;
}

StructuralEntity *
StructuralEdge::origin () const
{
  return _orig;
}

void
StructuralEdge::setOrigin (StructuralEntity *orig)
{
  _orig = orig;
}

bool
StructuralEdge::hasOrigin () const
{
  return _orig != nullptr;
}

StructuralEntity *
StructuralEdge::destination () const
{
  return _dest;
}

void
StructuralEdge::setDestination (StructuralEntity *head)
{
  _dest = head;
}

bool
StructuralEdge::hasDestination () const
{
  return _dest != nullptr;
}

void
StructuralEdge::adjust (bool collision, bool recursion)
{
  StructuralEntity::adjust (collision, recursion);

  setAngle (property (ST_ATTR_EDGE_ANGLE).toDouble ());

  StructuralEntity *parent = structuralParent ();
  if (parent || !ST_OPT_WITH_BODY)
  {
    qreal ang = angle ();

    StructuralEntity *orig = origin ();
    StructuralEntity *dest = destination ();

    if (orig && dest)
    {
      QLineF line = QLineF (QPointF (orig->left () + orig->width () / 2,
                                     orig->top () + orig->height () / 2),
                            QPointF (dest->left () + dest->width () / 2,
                                     dest->top () + dest->height () / 2));

      if (orig->category () == Structural::Interface)
      {
        if (parent)
          line.setP1 (
              parent->mapFromItem (orig->structuralParent (), line.p1 ()));
        else if (orig->structuralParent ())
          line.setP1 (orig->structuralParent ()->mapToScene (line.p1 ()));
      }

      if (dest->category () == Structural::Interface)
      {
        if (parent)
          line.setP2 (
              parent->mapFromItem (dest->structuralParent (), line.p2 ()));
        else if (dest->structuralParent ())
          line.setP2 (dest->structuralParent ()->mapToScene (line.p2 ()));
      }

      adjustBox (line);

      if (orig != dest)
      {
        orig->setSelectable (false);
        dest->setSelectable (false);

        adjustExtreme (orig, line, 0.0, 0.01, ang);
        line.setP1 (QPointF (origLeft (), origTop ()));

        adjustExtreme (dest, line, 1.0, -0.01, ang);
        line.setP2 (QPointF (destLeft (), destTop ()));

        orig->setSelectable (true);
        dest->setSelectable (true);
      }
    }
  }
}

void
StructuralEdge::adjustBox (QLineF line)
{
  QPointF pOrig = line.p1 ();
  QPointF pDest = line.p2 ();

  qreal x = 0.0, y = 0.0, w = 0.0, h = 0.0;

  if (pOrig.x () <= pDest.x () && pOrig.y () <= pDest.y ())
  {
    x = pOrig.x ();
    y = pOrig.y ();
    w = pDest.x () - pOrig.x ();
    h = pDest.y () - pOrig.y ();
  }
  else if (pOrig.x () > pDest.x () && pOrig.y () <= pDest.y ())
  {
    x = pDest.x ();
    y = pOrig.y ();
    w = pOrig.x () - pDest.x ();
    h = pDest.y () - pOrig.y ();
  }
  else if (pOrig.x () <= pDest.x () && pOrig.y () > pDest.y ())
  {
    x = pOrig.x ();
    y = pDest.y ();
    w = pDest.x () - pOrig.x ();
    h = pOrig.y () - pDest.y ();
  }
  else if (pOrig.x () > pDest.x () && pOrig.y () > pDest.y ())
  {
    x = pDest.x ();
    y = pDest.y ();
    w = pOrig.x () - pDest.x ();
    h = pOrig.y () - pDest.y ();
  }

  setOrigTop (pOrig.y ());
  setOrigLeft (pOrig.x ());
  setDestTop (pDest.y ());
  setDestLeft (pDest.x ());

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

      qreal alfa, beta, gama;

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

      if (extreme == origin ())
        setAlfa (alfa * (1 / index - 1));
      else
        setAlfa (alfa);
    }
    else
    {
      p = line.pointAt (index);
    }

    if (extreme == origin ())
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
  QPointF pOrig = QPointF (origLeft (), origTop ());
  QPointF pDest = QPointF (destLeft (), destTop ());

  QLineF drawLine;
  int x = 0, y = 0, z = 0, w = 0;

  if (pOrig.x () <= pDest.x () && pOrig.y () <= pDest.y ())
  {
    x = ST_DEFAULT_ENTITY_PADDING;
    y = ST_DEFAULT_ENTITY_PADDING;
    z = width () + ST_DEFAULT_ENTITY_PADDING;
    w = height () + ST_DEFAULT_ENTITY_PADDING;
  }
  else if (pOrig.x () > pDest.x () && pOrig.y () <= pDest.y ())
  {
    x = width () + ST_DEFAULT_ENTITY_PADDING;
    y = ST_DEFAULT_ENTITY_PADDING;
    z = ST_DEFAULT_ENTITY_PADDING;
    w = height () + ST_DEFAULT_ENTITY_PADDING;
  }
  else if (pOrig.x () <= pDest.x () && pOrig.y () > pDest.y ())
  {
    x = ST_DEFAULT_ENTITY_PADDING;
    y = height () + ST_DEFAULT_ENTITY_PADDING;
    z = width () + ST_DEFAULT_ENTITY_PADDING;
    w = ST_DEFAULT_ENTITY_PADDING;
  }
  else if (pOrig.x () > pDest.x () && pOrig.y () > pDest.y ())
  {
    x = width () + ST_DEFAULT_ENTITY_PADDING;
    y = height () + ST_DEFAULT_ENTITY_PADDING;
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
      QPen (QBrush (QColor (StructuralUtil::color (structuralType ()))), 1,
            Qt::DashLine));
  painter->drawLine (drawLine);

  // Drawing tail...
  // nothing...

  // Drawing head...
  painter->setBrush (
      QBrush (QColor (StructuralUtil::color (structuralType ()))));
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

  painter->addPolygon (QPolygonF ({ a, b, c }));
}
