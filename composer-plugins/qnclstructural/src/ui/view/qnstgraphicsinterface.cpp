#include "qnstgraphicsinterface.h"

QnstGraphicsInterface::QnstGraphicsInterface(QnstGraphicsEntity* parent)
  : QnstGraphicsEntityWithEdges(parent)
{
  setncgType(Qncg::Interface);
  setnstType(Qnst::Interface);

  setResizable(false);

  setnstId("");

  setRefer(false);

  /* Default size for interface */
  setTop(0);
  setLeft(0);
  setWidth(DEFAULT_INTERFACE_WIDTH);
  setHeight(DEFAULT_INTERFACE_HEIGHT);
}

QnstGraphicsInterface::~QnstGraphicsInterface()
{

}


bool QnstGraphicsInterface::isRefer()
{
  return _isRefer;
}

void QnstGraphicsInterface::setRefer(bool isRefer)
{
  _isRefer = isRefer;
}

void QnstGraphicsInterface::setnstId(QString id)
{
  QnstGraphicsEntity::setnstId(id);

  QString tip = "";
  QString name = (id != "" ? id : "?");

  if (getnstType() == Qnst::Port)
  {
    tip += "Port ("+name+")";
  }
  else if (getnstType() == Qnst::Area)
  {
    tip += "Area ("+name+")";
  }
  else if (getnstType() == Qnst::Property)
  {
    tip += "Property ("+name+")";
  }
  else
  {
    tip += "Interface ("+name+")";
  }

  setToolTip(tip);
}

void QnstGraphicsInterface::adjust(bool avoidCollision)
{
  QnstGraphicsEntity* parent = getnstGraphicsParent();

  if (parent != NULL)
  {
    adjustToBorder();

    if (avoidCollision)
    {
      int colliding;
      int maxInter = 10, inter = 0;
      do
      {
        if(inter > maxInter) break;
        inter++;

        colliding = false;
        foreach(QnstGraphicsEntity *entity,
                getnstGraphicsParent()->getnstGraphicsEntities())
        {
          if(this != entity && entity->getncgType() == Qncg::Interface)
          {
            qreal n = 0;
            qreal i = 0.0;

            entity->setSelectable(false); update();
            // check collision
            while(collidesWithItem(entity, Qt::IntersectsItemBoundingRect))
            {

              QPointF pa(getLeft()+getWidth()/2, getTop()+getHeight()/2);
              QPointF pb(entity->getWidth()/2, entity->getHeight()/2);

              QLineF line(pa, pb);

              line.setAngle(qrand()%360);

              i += (double)(qrand()%100)/10000.0;

              setTop(getTop()+line.pointAt(i/2).y()-pa.y());
              setLeft(getLeft()+line.pointAt(i/2).x()-pa.x());

              if (++n > 1000) // avoid infinity loop
              {
                n = -1;
                break;
              }
            }

            adjustToBorder();

            entity->setSelectable(true); update();
          }
        }

        foreach(QnstGraphicsEntity *entity,
                getnstGraphicsParent()->getnstGraphicsEntities())
        {
          if(collidesWithItem(entity, Qt::IntersectsItemBoundingRect))
          {
            colliding = true;
          }
        }
      }
      while(colliding);
    }
  }

  foreach(QnstGraphicsEntity* edges, getnstGraphicsEdges())
  {
    if (edges->getnstType() == Qnst::Reference ||
        edges->getnstType() == Qnst::Link ||
        edges->getnstType() == Qnst::Mapping ||
        edges->getnstType() == Qnst::Action ||
        edges->getnstType() == Qnst::Condition)
    {
      edges->adjust();
    }
  }

  // redrawing
  if (scene() != NULL)
    scene()->update();
}

void QnstGraphicsInterface::adjustToBorder()
{
  QnstGraphicsEntity* parent = getnstGraphicsParent();

  if (parent != NULL)
  {
    // setting
    QPointF pointa(parent->getWidth()/2, parent->getHeight()/2);
    QPointF pointb(getLeft() + getWidth()/2, getTop() + getHeight()/2);

    if (pointa == pointb)
    {
      pointb.setX(pointa.x());
      pointb.setY(pointa.y() - 10);
    }

    QLineF line(pointa,pointb);

    // adjusting
    if (parent->contains(pointb))
    {
      QPointF pointn = pointb;

      qreal index = 1.0;

      if (parent->contains(line.pointAt(index+0.01)))
      {
        while(parent->contains(pointn))
        {
          index += 0.01;

          pointn = line.pointAt(index);
        }
      }

      setTop(pointn.y() - getHeight()/2);
      setLeft(pointn.x() - getWidth()/2);

    }
    else
    {
      QPointF pointn = pointb;

      qreal index = 1.0;

      if (!parent->contains(line.pointAt(index-0.01)))
      {
        while(!parent->contains(pointn))
        {
          index -= 0.01;

          pointn = line.pointAt(index);
        }
      }

      setTop(pointn.y() - getHeight()/2);
      setLeft(pointn.x() - getWidth()/2);
    }
  }
}

void QnstGraphicsInterface::move(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();

  QnstGraphicsEntity* parent = getnstGraphicsParent();

  qreal minx;
  qreal miny;

  if (parent != NULL)
  {
    minx = 4;
    miny = 4;
  }
  else
  {
    minx = 0;
    miny = 0;
  }

  qreal maxx;
  qreal maxy;

  if (parent != NULL)
  {
    maxx = parent->getWidth() - getWidth() - 4;
    maxy = parent->getHeight() - getHeight() - 4;
  }
  else
  {
    maxx = scene()->width() - getWidth();
    maxy = scene()->height() - getHeight();
  }

  qreal dx = event->pos().x() - getPressLeft(); // (x1 - x0)
  qreal dy = event->pos().y() - getPressTop();  // (y1 - y0)

  qreal nextx = x + dx;
  qreal nexty = y + dy;

  // moving
  setMoveTop(nexty);
  setMoveLeft(nextx);

  // redrawing
  if (scene() != NULL)
    scene()->update();
}

void QnstGraphicsInterface::resize(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();
  qreal w = getWidth();
  qreal h = getHeight();

  QnstGraphicsEntity* parent = getnstGraphicsParent();

  qreal minx;
  qreal miny;
  qreal minw;
  qreal minh;

  if (parentItem() != NULL)
  {
    minx = 4;
    miny = 4;
    minw = -1; // not used
    minh = -1; // not used
  }
  else
  {
    minx = 0;
    miny = 0;
    minw = -1; // not used
    minh = -1; // not used
  }

  qreal maxx;
  qreal maxy;
  qreal maxw;
  qreal maxh;

  if (parentItem() != NULL)
  {
    maxx = parent->getWidth() - getWidth() - 4;
    maxy = parent->getHeight() - getHeight() - 4;
    maxw = parent->getWidth() - 4;
    maxh = parent->getHeight() - 4;
  }
  else
  {
    maxx = scene()->width() - getWidth();
    maxy = scene()->height() - getHeight();
    maxw = scene()->width();
    maxh = scene()->height();
  }

  qreal dx = event->pos().x() - getPressLeft();    // (x1 - x0)
  qreal dy = event->pos().y() - getPressTop();     // (y1 - y0)
  qreal dw = -dx;
  qreal dh = -dy;

  qreal nextx = x + dx;
  qreal nexty = y + dy;
  qreal nextw = w + dw;
  qreal nexth = h + dh;

  // adjusting
  switch(getncgResize())
  {
    case Qncg::TopLeft:
    {
      break;
    }

    case Qncg::Top:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width

      break;
    }

    case Qncg::TopRight:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      break;
    }

    case Qncg::Right:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      nexty = y; // fixed y
      nexth = h; // fixed height

      break;
    }

    case Qncg::BottomRight:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      nexty = y; // fixed y
      nexth = h - dh;

      break;
    }

    case Qncg::Bottom:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width

      nexty = y; // fixed y
      nexth = h - dh;

      break;
    }

    case Qncg::BottomLeft:
    {
      nexty = y; // fixed y
      nexth = h - dh;
      break;
    }

    case Qncg::Left:
    {
      nexty = y; // fixed y
      nexth = h; // fixed height

      break;
    }
  }

  // resizing
  setResizeTop(nexty);
  setResizeLeft(nextx);
  setResizeWidth(nextw);
  setResizeHeight(nexth);

  // redrawing
  if (scene() != NULL)
    scene()->update();
}
