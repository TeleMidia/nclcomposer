#include "StructuralBind.h"

#include <math.h>

StructuralBind::StructuralBind (StructuralEntity *parent)
    : StructuralEdge (parent)
{
  setCategory (Structural::Edge);
  setType (Structural::Bind);

  setRole (Structural::NoRole);
}

StructuralBind::~StructuralBind () {}

StructuralRole
StructuralBind::getRole () const
{
  return _role;
}

void
StructuralBind::setRole (StructuralRole role)
{
  _role = role;

  setProperty (STR_PROPERTY_BIND_ROLE,
                         StructuralUtil::translateRoleToString (role));
}

void
StructuralBind::adjust (bool collision, bool recursion)
{
  StructuralEdge::adjust (collision, recursion);

  // Adjusting properties...
  setRole (StructuralUtil::translateStringToRole (
      getProperty (STR_PROPERTY_BIND_ROLE)));
}

void
StructuralBind::draw (QPainter *painter)
{
  StructuralEntity *tail = getTail ();
  StructuralEntity *head = getHead ();

  if (tail != NULL && head != NULL)
  {
    painter->setRenderHint (QPainter::Antialiasing, true);
    painter->setRenderHint (QPainter::SmoothPixmapTransform, true);

    // Setting...
    QLineF drawLine = getDrawLine (STR_DEFAULT_BIND_PADDING);

    // Drawing line...
    painter->setPen (QPen (
        QBrush (QColor (StructuralUtil::getColor (getType ()))), 1));

    qreal x;
    qreal y;
    qreal z;
    qreal w;
    qreal a;
    qreal b;

    qreal r;

    qreal alfa;
    qreal beta;
    qreal delta;

    qreal angle;

    QPointF center;

    x = drawLine.p1 ().x ();
    y = drawLine.p1 ().y ();
    z = drawLine.p2 ().x ();
    w = drawLine.p2 ().y ();

    if (tail->getType () == Structural::Link)
    {
      a = drawLine.p2 ().x ();
      b = drawLine.p2 ().y ();
    }
    else
    {
      a = drawLine.p1 ().x ();
      b = drawLine.p1 ().y ();
    }

    alfa = getAngle ();

    angle = getAngle ();

    if (drawLine.p1 ().x () <= drawLine.p2 ().x ()
        && drawLine.p1 ().y () <= drawLine.p2 ().y ())
    {
      if (tail->getType () == Structural::Link)
      {
        z -= STR_DEFAULT_BIND_ROLE_W / 2;
        w -= STR_DEFAULT_BIND_ROLE_H / 2;

        a -= STR_DEFAULT_BIND_ROLE_W;
        b -= STR_DEFAULT_BIND_ROLE_H;
      }
      else
      {
        x += STR_DEFAULT_BIND_ROLE_W / 2;
        y += STR_DEFAULT_BIND_ROLE_H / 2;
      }
    }
    else if (drawLine.p1 ().x () > drawLine.p2 ().x ()
             && drawLine.p1 ().y () <= drawLine.p2 ().y ())
    {
      if (tail->getType () == Structural::Link)
      {
        z += STR_DEFAULT_BIND_ROLE_W / 2;
        w -= STR_DEFAULT_BIND_ROLE_H / 2;

        b -= STR_DEFAULT_BIND_ROLE_H;
      }
      else
      {
        x -= STR_DEFAULT_BIND_ROLE_W / 2;
        y += STR_DEFAULT_BIND_ROLE_H / 2;

        a -= STR_DEFAULT_BIND_ROLE_H;
      }
    }
    else if (drawLine.p1 ().x () <= drawLine.p2 ().x ()
             && drawLine.p1 ().y () > drawLine.p2 ().y ())
    {
      if (tail->getType () == Structural::Link)
      {
        z -= STR_DEFAULT_BIND_ROLE_W / 2;
        w += STR_DEFAULT_BIND_ROLE_H / 2;

        a -= STR_DEFAULT_BIND_ROLE_W;
      }
      else
      {
        x += STR_DEFAULT_BIND_ROLE_W / 2;
        y -= STR_DEFAULT_BIND_ROLE_H / 2;

        b -= STR_DEFAULT_BIND_ROLE_W;
      }
    }
    else if (drawLine.p1 ().x () > drawLine.p2 ().x ()
             && drawLine.p1 ().y () > drawLine.p2 ().y ())
    {
      if (tail->getType () == Structural::Link)
      {
        z += STR_DEFAULT_BIND_ROLE_W / 2;
        w += STR_DEFAULT_BIND_ROLE_H / 2;
      }
      else
      {
        x -= STR_DEFAULT_BIND_ROLE_W / 2;
        y -= STR_DEFAULT_BIND_ROLE_H / 2;

        a -= STR_DEFAULT_BIND_ROLE_W;
        b -= STR_DEFAULT_BIND_ROLE_H;
      }
    }

    QString icon = StructuralUtil::getIcon (getRole ());

    if (icon.isEmpty ())
    {
      if (tail->getType () == Structural::Link)
        icon = ":/icon/bind-unknow-action";
      else
        icon = ":/icon/bind-unknow-condition";
    }

    if (angle != 0)
    {
      QLineF line = QLineF (x, y, z, w);

      if (alfa < 0)
      {
        alfa = -alfa;

        r = line.length () / (::sin (((alfa / 2) * PI) / 180) * 2);
        beta = (180 - alfa) / 2 + (360 - line.angle ());
        delta = (180 - beta - alfa) + 180;

        center.setX (line.p1 ().x ()
                     + ::cos ((180 - beta - alfa) * PI / 180) * r);
        center.setY (line.p1 ().y ()
                     - ::sin ((180 - beta - alfa) * PI / 180) * r);
      }
      else
      {
        r = line.length () / (::sin (((alfa / 2) * PI) / 180) * 2);
        beta = (180 - alfa) / 2 + (360 - line.angle ());
        delta = (180 - beta - alfa);

        center.setX (line.p2 ().x ()
                     - ::cos ((180 - beta - alfa) * PI / 180) * r);
        center.setY (line.p2 ().y ()
                     + ::sin ((180 - beta - alfa) * PI / 180) * r);
      }

      painter->drawArc (center.x () - r, center.y () - r, 2 * r, 2 * r,
                        16 * delta, 16 * alfa);
    }
    else
    {
      painter->drawLine (x, y, z, w);
    }

    // Drawing tail...
    // nothing...

    // Drawing head...
    painter->drawPixmap (a, b, STR_DEFAULT_BIND_ROLE_W,
                         STR_DEFAULT_BIND_ROLE_H, QPixmap (icon));

    if (!getError ().isEmpty () || !getWarning ().isEmpty ())
    {

      QColor color;

      if (!getError ().isEmpty ())
      {
        color = QColor (QString (STR_DEFAULT_ALERT_ERROR_COLOR));
      }
      else
      {
        color = QColor (QString (STR_DEFAULT_ALERT_WARNING_COLOR));
      }

      color.setAlpha (0.75);

      painter->setBrush (color);
      painter->setPen (QPen (QBrush (color), 0));

      painter->drawEllipse (a, b, STR_DEFAULT_BIND_ROLE_W,
                            STR_DEFAULT_BIND_ROLE_H);
    }
  }
}

void
StructuralBind::delineate (QPainterPath *painter) const
{
  StructuralEntity *tail = getTail ();
  StructuralEntity *head = getHead ();

  if (tail != NULL && head != NULL)
  {
    // Setting...
    QLineF drawLine = getDrawLine (STR_DEFAULT_BIND_PADDING);

    qreal x;
    qreal y;

    if (drawLine.p1 ().x () <= drawLine.p2 ().x ()
        && drawLine.p1 ().y () <= drawLine.p2 ().y ())
    {
      if (tail->getType () == Structural::Link)
      {
        x = drawLine.p2 ().x () - STR_DEFAULT_BIND_ROLE_W;
        y = drawLine.p2 ().y () - STR_DEFAULT_BIND_ROLE_H;
      }
      else
      {
        x = drawLine.p1 ().x ();
        y = drawLine.p1 ().y ();
      }
    }
    else if (drawLine.p1 ().x () > drawLine.p2 ().x ()
             && drawLine.p1 ().y () <= drawLine.p2 ().y ())
    {
      if (tail->getType () == Structural::Link)
      {
        x = drawLine.p2 ().x ();
        y = drawLine.p2 ().y () - STR_DEFAULT_BIND_ROLE_H;
      }
      else
      {
        x = drawLine.p1 ().x () - STR_DEFAULT_BIND_ROLE_W;
        y = drawLine.p1 ().y ();
      }
    }
    else if (drawLine.p1 ().x () <= drawLine.p2 ().x ()
             && drawLine.p1 ().y () > drawLine.p2 ().y ())
    {
      if (tail->getType () == Structural::Link)
      {
        x = drawLine.p2 ().x () - STR_DEFAULT_BIND_ROLE_W;
        y = drawLine.p2 ().y ();
      }
      else
      {
        x = drawLine.p1 ().x ();
        y = drawLine.p1 ().y () - STR_DEFAULT_BIND_ROLE_H;
      }
    }
    else if (drawLine.p1 ().x () > drawLine.p2 ().x ()
             && drawLine.p1 ().y () > drawLine.p2 ().y ())
    {
      if (tail->getType () == Structural::Link)
      {
        x = drawLine.p2 ().x ();
        y = drawLine.p2 ().y ();
      }
      else
      {
        x = drawLine.p1 ().x () - STR_DEFAULT_BIND_ROLE_W;
        y = drawLine.p1 ().y () - STR_DEFAULT_BIND_ROLE_H;
      }
    }

    painter->addEllipse (x, y, STR_DEFAULT_BIND_ROLE_W,
                         STR_DEFAULT_BIND_ROLE_H);
  }
}

void
StructuralBind::mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED (event);
  //  StructuralEdge::mouseDoubleClickEvent(event);

  emit performedEdit (this);
}
