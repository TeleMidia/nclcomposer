#include "StructuralInterface.h"

StructuralInterface::StructuralInterface(StructuralEntity* parent)
  : StructuralEntity(parent)
{
  setStructuralCategory(Structural::Interface);
  setStructuralType(Structural::NoType);

  setResizable(false);

  setTop(0);
  setLeft(0);
  setWidth(STR_DEFAULT_INTERFACE_W);
  setHeight(STR_DEFAULT_INTERFACE_H);
}

StructuralInterface::~StructuralInterface()
{

}

void StructuralInterface::adjust(bool avoidCollision,  bool rec)
{
  StructuralEntity::adjust(avoidCollision, rec);

  // Adjusting properties...
  StructuralType type = getStructuralType();

  if (type == Structural::Port){
    icon = QPixmap(":/images/icon/port");
  }else if (type == Structural::Area){
    icon = QPixmap(":/images/icon/area");
  }else if (type == Structural::Property){
    icon = QPixmap(":/images/icon/property");
  }else if (type == Structural::SwitchPort){
    icon = QPixmap(":/images/icon/switchport");
  }

  QString tip = "";
  QString name = (getStructuralProperty(STR_PROPERTY_ENTITY_ID) != "" ? getStructuralProperty(STR_PROPERTY_ENTITY_ID) : "?");

  if (type == Structural::Port)
  {
    tip += "Port ("+name+")";
  }
  else if (type == Structural::Area)
  {
    tip += "Area ("+name+")";
  }
  else if (type == Structural::Property)
  {
    tip += "Property ("+name+")";
  }
  else if (type == Structural::SwitchPort)
  {
    tip += "SwitchPort ("+name+")";
  }
  else
  {
    tip += "Interface ("+name+")";
  }

  setToolTip(tip);


  // Adjusting position...

  StructuralEntity* parent = getStructuralParent();

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
        foreach(StructuralEntity *entity,
                getStructuralParent()->getStructuralEntities())
        {
          if(this != entity && entity->getStructuralCategory() == Structural::Interface)
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

        foreach(StructuralEntity *entity,
                getStructuralParent()->getStructuralEntities())
        {
          if(collidesWithItem(entity, Qt::IntersectsItemBoundingRect))
          {
            colliding = true;
          }
        }
      }
      while(colliding);
    }

    QVector<StructuralEntity*> roots;
    roots = getStructuralParent()->getStructuralEntities();

    if (getStructuralParent()->getStructuralParent() != NULL){
      roots += getStructuralParent()->getStructuralParent()->getStructuralEntities();
    }

    foreach (StructuralEntity* c, roots) {
      if (c->getStructuralCategory() == Structural::Edge){
        StructuralEdge *e = (StructuralEdge*) c;

        if (e->getEntityA() == this || e->getEntityB() == this){
          e->adjust();
        }
      }
    }
  }

  // redrawing
  if (scene() != NULL)
    scene()->update();
}

void StructuralInterface::adjustToBorder()
{
  StructuralEntity* parent = getStructuralParent();

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

void StructuralInterface::move(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();

  StructuralEntity* parent = getStructuralParent();

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

void StructuralInterface::resize(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();
  qreal w = getWidth();
  qreal h = getHeight();

  StructuralEntity* parent = getStructuralParent();

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
  switch(getStructuralResize())
  {
    case Structural::TopLeft:
    {
      break;
    }

    case Structural::Top:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width

      break;
    }

    case Structural::TopRight:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      break;
    }

    case Structural::Right:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      nexty = y; // fixed y
      nexth = h; // fixed height

      break;
    }

    case Structural::BottomRight:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      nexty = y; // fixed y
      nexth = h - dh;

      break;
    }

    case Structural::Bottom:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width

      nexty = y; // fixed y
      nexth = h - dh;

      break;
    }

    case Structural::BottomLeft:
    {
      nexty = y; // fixed y
      nexth = h - dh;
      break;
    }

    case Structural::Left:
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

void StructuralInterface::draw(QPainter* painter)
{
//  painter->drawRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);

  painter->drawPixmap(4 + 4/2, 4 + 4/2, getWidth()-4, getHeight()-4,icon);

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

void StructuralInterface::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}
