#include "StructuralInterface.h"

StructuralInterface::StructuralInterface (StructuralEntity *parent)
    : StructuralEntity (parent)
{
  setCategory (Structural::Interface);
  setStructuralType (Structural::NoType);

  setResizable (false);

  setTop (0);
  setLeft (0);
  setWidth (ST_DEFAULT_INTERFACE_W);
  setHeight (ST_DEFAULT_INTERFACE_H);

  if (!ST_OPT_SHOW_INTERFACES)
    setHidden (true);
}

void
StructuralInterface::adjust (bool collision, bool recursion)
{
  StructuralEntity::adjust (collision, recursion);

  // Adjusting position...
  StructuralEntity *parent = structuralParent ();

  if (parent || !ST_OPT_WITH_BODY)
  {
    if (!collision)
    {
      // Tries (10x) to find a position where there is no collision with others
      // relatives
      for (int i = 0; i < 10; i++)
      {
        bool colliding = false;
        for (StructuralEntity *ent : StructuralUtil::neighbors (this))
        {
          if (ent != this)
          {
            int n = 0, max = 1000;
            qreal current = 0.0;

            ent->setSelectable (false);

            while (collidesWithItem (ent, Qt::IntersectsItemBoundingRect))
            {
              QLineF line
                  = QLineF (left () + width () / 2, top () + height () / 2,
                            ent->width () / 2, ent->height () / 2);

              line.setAngle (qrand () % 360);

              current += (double)(qrand () % 100) / 1000.0;

              setTop (top () + line.pointAt (current / 2).y ()
                      - line.p1 ().y ());
              setLeft (left () + line.pointAt (current / 2).x ()
                       - line.p1 ().x ());

              if (++n > max)
                break;
            }

            constrain ();

            ent->setSelectable (true);
          }
        }

        for (StructuralEntity *ent : StructuralUtil::neighbors (this))
          if (collidesWithItem (ent, Qt::IntersectsItemBoundingRect))
            colliding = true;

        if (!colliding)
          break;
      }
    }

    constrain ();

    StructuralUtil::adjustEdges (this);
  }
}

void
StructuralInterface::constrain ()
{
  StructuralEntity *parent = structuralParent ();

  if (parent != nullptr)
  {
    QPointF tail (parent->width () / 2, parent->height () / 2);
    QPointF head (left () + width () / 2, top () + height () / 2);

    if (tail == head)
    {
      head.setX (tail.x ());
      head.setY (tail.y () - 10);
    }

    QPointF p = head;
    QLineF line (tail, head);
    bool status = true;

    qreal current = 1.0;
    qreal step = 0.01;

    if (!parent->contains (p))
    {
      step = -0.01;
      status = false;
    }

    do
    {
      current += step;
      p = line.pointAt (current);
    } while (parent->contains (p) == status);

    if (QLineF (p, head).length () > 7)
    {
      setTop (p.y () - height () / 2);
      setLeft (p.x () - width () / 2);
    }
  }
}

void
StructuralInterface::draw (QPainter *painter)
{
  int x = ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_INTERFACE_PADDING;
  int y = ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_INTERFACE_PADDING;
  int w = width () - 2 * ST_DEFAULT_INTERFACE_PADDING;
  int h = height () - 2 * ST_DEFAULT_INTERFACE_PADDING;

  painter->drawPixmap (x, y, w, h,
                       QPixmap (StructuralUtil::icon (structuralType ())));

  if (!ST_OPT_WITH_BODY && !ST_OPT_USE_FLOATING_INTERFACES)
  {
    if (property (ST_ATTR_ENT_AUTOSTART) == ST_VALUE_TRUE)
    {
      painter->setPen (QPen (QBrush (QColor (76, 76, 76)), 2));
      painter->drawRect (x, y, w, h);
    }
  }

  if (!error ().isEmpty () || !warning ().isEmpty ())
  {

    QString icon;

    if (!error ().isEmpty ())
      icon = QString (ST_DEFAULT_ALERT_ERROR_ICON);
    else
      icon = QString (ST_DEFAULT_ALERT_WARNING_ICON);

    painter->drawPixmap (x + w / 2 - (ST_DEFAULT_ALERT_ICON_W - 3) / 2,
                         y + h / 2 - (ST_DEFAULT_ALERT_ICON_H - 3) / 2,
                         ST_DEFAULT_ALERT_ICON_W - 3,
                         ST_DEFAULT_ALERT_ICON_H - 3, QPixmap (icon));
  }

  if (isMoving ())
  {
    painter->setBrush (QBrush (Qt::NoBrush));
    painter->setPen (QPen (QBrush (Qt::black), 0));

    int moveX = x + moveLeft () - left ();
    int moveY = y + moveTop () - top ();
    int moveW = w;
    int moveH = h;

    painter->drawRect (moveX, moveY, moveW, moveH);
  }
}
