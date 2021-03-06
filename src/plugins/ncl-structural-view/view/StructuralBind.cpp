#include "StructuralBind.h"

#include <math.h>

StructuralBind::StructuralBind (StructuralEntity *parent)
    : StructuralEdge (parent)
{
  setCategory (Structural::Edge);
  setStructuralType (Structural::Bind);

  setRole (Structural::NoRole);
}

StructuralRole
StructuralBind::getRole () const
{
  return _role;
}

void
StructuralBind::setRole (StructuralRole role)
{
  _role = role;

  setProperty (ST_ATTR_BIND_ROLE, StructuralUtil::roletostr (role));
}

void
StructuralBind::adjust (bool collision, bool recursion)
{
  StructuralEdge::adjust (collision, recursion);

  setRole (StructuralUtil::strtorole (property (ST_ATTR_BIND_ROLE)));
}

void
StructuralBind::draw (QPainter *painter)
{
  StructuralEntity *orig = origin ();
  StructuralEntity *dest = destination ();

  if (orig && dest)
  {
    painter->setRenderHint (QPainter::Antialiasing, true);
    painter->setRenderHint (QPainter::SmoothPixmapTransform, true);

    QLineF drawLine = getDrawLine (ST_DEFAULT_BIND_PADDING);

    painter->setPen (
        QPen (QBrush (QColor (StructuralUtil::color (structuralType ()))), 1));

    qreal x, y, z, w;
    qreal a, b;

    qreal r;

    qreal alfa, beta, delta;
    qreal ang;

    QPointF center;

    x = drawLine.p1 ().x ();
    y = drawLine.p1 ().y ();
    z = drawLine.p2 ().x ();
    w = drawLine.p2 ().y ();

    if (orig->structuralType () == Structural::Link)
    {
      a = drawLine.p2 ().x ();
      b = drawLine.p2 ().y ();
    }
    else
    {
      a = drawLine.p1 ().x ();
      b = drawLine.p1 ().y ();
    }

    alfa = angle ();
    ang = angle ();

    if (drawLine.p1 ().x () <= drawLine.p2 ().x ()
        && drawLine.p1 ().y () <= drawLine.p2 ().y ())
    {
      if (orig->structuralType () == Structural::Link)
      {
        z -= ST_DEFAULT_BIND_ROLE_W / 2;
        w -= ST_DEFAULT_BIND_ROLE_H / 2;

        a -= ST_DEFAULT_BIND_ROLE_W;
        b -= ST_DEFAULT_BIND_ROLE_H;
      }
      else
      {
        x += ST_DEFAULT_BIND_ROLE_W / 2;
        y += ST_DEFAULT_BIND_ROLE_H / 2;
      }
    }
    else if (drawLine.p1 ().x () > drawLine.p2 ().x ()
             && drawLine.p1 ().y () <= drawLine.p2 ().y ())
    {
      if (orig->structuralType () == Structural::Link)
      {
        z += ST_DEFAULT_BIND_ROLE_W / 2;
        w -= ST_DEFAULT_BIND_ROLE_H / 2;

        b -= ST_DEFAULT_BIND_ROLE_H;
      }
      else
      {
        x -= ST_DEFAULT_BIND_ROLE_W / 2;
        y += ST_DEFAULT_BIND_ROLE_H / 2;

        a -= ST_DEFAULT_BIND_ROLE_H;
      }
    }
    else if (drawLine.p1 ().x () <= drawLine.p2 ().x ()
             && drawLine.p1 ().y () > drawLine.p2 ().y ())
    {
      if (orig->structuralType () == Structural::Link)
      {
        z -= ST_DEFAULT_BIND_ROLE_W / 2;
        w += ST_DEFAULT_BIND_ROLE_H / 2;

        a -= ST_DEFAULT_BIND_ROLE_W;
      }
      else
      {
        x += ST_DEFAULT_BIND_ROLE_W / 2;
        y -= ST_DEFAULT_BIND_ROLE_H / 2;

        b -= ST_DEFAULT_BIND_ROLE_W;
      }
    }
    else if (drawLine.p1 ().x () > drawLine.p2 ().x ()
             && drawLine.p1 ().y () > drawLine.p2 ().y ())
    {
      if (orig->structuralType () == Structural::Link)
      {
        z += ST_DEFAULT_BIND_ROLE_W / 2;
        w += ST_DEFAULT_BIND_ROLE_H / 2;
      }
      else
      {
        x -= ST_DEFAULT_BIND_ROLE_W / 2;
        y -= ST_DEFAULT_BIND_ROLE_H / 2;

        a -= ST_DEFAULT_BIND_ROLE_W;
        b -= ST_DEFAULT_BIND_ROLE_H;
      }
    }

    QString icon = StructuralUtil::icon (getRole ());

    if (icon.isEmpty ())
    {
      if (orig->structuralType () == Structural::Link)
        icon = ":/icon/bind-unknow-action";
      else
        icon = ":/icon/bind-unknow-condition";
    }

    if (ang != 0)
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
    painter->drawPixmap (a, b, ST_DEFAULT_BIND_ROLE_W, ST_DEFAULT_BIND_ROLE_H,
                         QPixmap (icon));

    if (!error ().isEmpty () || !warning ().isEmpty ())
    {

      QColor color;

      if (!error ().isEmpty ())
      {
        color = QColor (QString (ST_DEFAULT_ALERT_ERROR_COLOR));
      }
      else
      {
        color = QColor (QString (ST_DEFAULT_ALERT_WARNING_COLOR));
      }

      color.setAlphaF (0.75);

      painter->setBrush (color);
      painter->setPen (QPen (QBrush (color), 0));

      painter->drawEllipse (a, b, ST_DEFAULT_BIND_ROLE_W,
                            ST_DEFAULT_BIND_ROLE_H);
    }
  }
}

void
StructuralBind::delineate (QPainterPath *painter) const
{
  StructuralEntity *orig = origin ();
  StructuralEntity *dest = destination ();

  if (orig && dest)
  {
    // Setting...
    QLineF drawLine = getDrawLine (ST_DEFAULT_BIND_PADDING);

    qreal x = 0.0, y = 0.0;

    if (drawLine.p1 ().x () <= drawLine.p2 ().x ()
        && drawLine.p1 ().y () <= drawLine.p2 ().y ())
    {
      if (orig->structuralType () == Structural::Link)
      {
        x = drawLine.p2 ().x () - ST_DEFAULT_BIND_ROLE_W;
        y = drawLine.p2 ().y () - ST_DEFAULT_BIND_ROLE_H;
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
      if (orig->structuralType () == Structural::Link)
      {
        x = drawLine.p2 ().x ();
        y = drawLine.p2 ().y () - ST_DEFAULT_BIND_ROLE_H;
      }
      else
      {
        x = drawLine.p1 ().x () - ST_DEFAULT_BIND_ROLE_W;
        y = drawLine.p1 ().y ();
      }
    }
    else if (drawLine.p1 ().x () <= drawLine.p2 ().x ()
             && drawLine.p1 ().y () > drawLine.p2 ().y ())
    {
      if (orig->structuralType () == Structural::Link)
      {
        x = drawLine.p2 ().x () - ST_DEFAULT_BIND_ROLE_W;
        y = drawLine.p2 ().y ();
      }
      else
      {
        x = drawLine.p1 ().x ();
        y = drawLine.p1 ().y () - ST_DEFAULT_BIND_ROLE_H;
      }
    }
    else if (drawLine.p1 ().x () > drawLine.p2 ().x ()
             && drawLine.p1 ().y () > drawLine.p2 ().y ())
    {
      if (orig->structuralType () == Structural::Link)
      {
        x = drawLine.p2 ().x ();
        y = drawLine.p2 ().y ();
      }
      else
      {
        x = drawLine.p1 ().x () - ST_DEFAULT_BIND_ROLE_W;
        y = drawLine.p1 ().y () - ST_DEFAULT_BIND_ROLE_H;
      }
    }

    painter->addEllipse (x, y, ST_DEFAULT_BIND_ROLE_W, ST_DEFAULT_BIND_ROLE_H);
  }
}

void
StructuralBind::mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED (event);
  //  StructuralEdge::mouseDoubleClickEvent(event);

  emit performedEdit (this);
}
