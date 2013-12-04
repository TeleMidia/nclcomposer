#include "qnstinterface.h"

QnstInterface::QnstInterface(QnstEntity* parent)
  : QnstEntityWithEdges(parent)
{
  setnstType(Qnst::Interface);
  setnstSubtype(Qnst::NoSubtype);

  setResizable(false);

  setnstId("");

  setRefer(false);

  setHexColor("#000000");
  setHexBorderColor("#000000");

  /* Default size for interface */
  setTop(0);
  setLeft(0);
  setWidth(DEFAULT_INTERFACE_WIDTH);
  setHeight(DEFAULT_INTERFACE_HEIGHT);
}

QnstInterface::~QnstInterface()
{

}

bool QnstInterface::isRefer()
{
  return _isRefer;
}

void QnstInterface::setRefer(bool isRefer)
{
  _isRefer = isRefer;
}

void QnstInterface::setnstSubtype(const QnstSubtype subtype)
{
  if (subtype == Qnst::Port){
    setHexColor("#000000");
    setHexBorderColor("#000000");
  }else if (subtype == Qnst::Area){
    setHexColor("#F4A460");
    setHexBorderColor("#999999");
  }else if (subtype == Qnst::Property){
    setHexColor("#999999");
    setHexBorderColor("#666666");
  }else if (subtype == Qnst::SwitchPort){
    setHexColor("#000000");
    setHexBorderColor("#000000");
  }

  QnstEntityWithEdges::setnstSubtype(subtype);

  updateToolTip();
}

void QnstInterface::setHexColor(QString hexColor)
{
  this->hexColor = hexColor;
}

void QnstInterface::setHexBorderColor(QString hexBorderColor)
{
  this->hexBorderColor = hexBorderColor;
}

void QnstInterface::updateToolTip()
{
  QString tip = "";
  QString name = (getnstProperty(":nst:id") != "" ? getnstProperty(":nst:id") : "?");

  if (getnstSubtype() == Qnst::Port)
  {
    tip += "Port ("+name+")";
  }
  else if (getnstSubtype() == Qnst::Area)
  {
    tip += "Area ("+name+")";
  }
  else if (getnstSubtype() == Qnst::Property)
  {
    tip += "Property ("+name+")";
  }
  else if (getnstSubtype() == Qnst::SwitchPort)
  {
    tip += "SwitchPort ("+name+")";
  }
  else
  {
    tip += "Interface ("+name+")";
  }

  setToolTip(tip);
}


void QnstInterface::adjust(bool avoidCollision)
{
  QnstEntity* parent = getnstParent();

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
        foreach(QnstEntity *entity,
                getnstParent()->getnstChildren())
        {
          if(this != entity && entity->getnstType() == Qnst::Interface)
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

        foreach(QnstEntity *entity,
                getnstParent()->getnstChildren())
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

  /*
  foreach(QnstEntity* edges, getnstGraphicsEdges())
  {
    if (edges->getnstSubtype() == Qnst::Reference ||
        edges->getnstSubtype() == Qnst::Link ||
        edges->getnstSubtype() == Qnst::Mapping ||
        edges->getnstSubtype() == Qnst::Action ||
        edges->getnstSubtype() == Qnst::Condition)
    {
      edges->adjust();
    }
  }
*/
  // redrawing
  if (scene() != NULL)
    scene()->update();
}

void QnstInterface::adjustToBorder()
{
  QnstEntity* parent = getnstParent();

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

void QnstInterface::move(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();

  QnstEntity* parent = getnstParent();

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

void QnstInterface::resize(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();
  qreal w = getWidth();
  qreal h = getHeight();

  QnstEntity* parent = getnstParent();

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
  switch(getResizeType())
  {
    case Qnst::TopLeft:
    {
      break;
    }

    case Qnst::Top:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width

      break;
    }

    case Qnst::TopRight:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      break;
    }

    case Qnst::Right:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      nexty = y; // fixed y
      nexth = h; // fixed height

      break;
    }

    case Qnst::BottomRight:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      nexty = y; // fixed y
      nexth = h - dh;

      break;
    }

    case Qnst::Bottom:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width

      nexty = y; // fixed y
      nexth = h - dh;

      break;
    }

    case Qnst::BottomLeft:
    {
      nexty = y; // fixed y
      nexth = h - dh;
      break;
    }

    case Qnst::Left:
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

void QnstInterface::draw(QPainter* painter)
{
  QColor bg(hexColor);
  QColor border(hexBorderColor);

  if (isRefer())
  {
    bg.setAlpha(75);
    border.setAlpha(75);
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

void QnstInterface::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}
