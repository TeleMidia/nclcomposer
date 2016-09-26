#include "StructuralNode.h"

#include "StructuralEdge.h"

StructuralNode::StructuralNode(StructuralEntity* parent)
  : StructuralEntity(parent)
{
  setStructuralCategory(Structural::Node);
  setStructuralType(Structural::NoType);
}

StructuralNode::~StructuralNode()
{

}

void StructuralNode::inside()
{
  StructuralEntity* parent = getStructuralParent();

  if (parent != NULL) {
    QLineF line = QLineF(getLeft()+getWidth()/2, getTop()+getHeight()/2,
                         parent->getWidth()/2, parent->getHeight()/2);

    int max = 1000;
    int n = 0;

    qreal current = 0.0;

    setSelectable(false);

    while(!collidesWithItem(parent,Qt::ContainsItemShape)) {
      current += 0.01;

      setTop(getTop()+line.pointAt(current).y()-line.p1().y());
      setLeft(getLeft()+line.pointAt(current).x()-line.p1().x());

      if (++n > max) {
        n = -1;
        break;
      }
    }

    if (n < 0) {
      setTop(line.p2().y()-getHeight()/2);
      setLeft(line.p2().x()-getWidth()/2);
    }

    setSelectable(true);
  }
}


void StructuralNode::adjust(bool collision, bool recursion)
{
  StructuralEntity::adjust(collision, recursion);

  if (recursion)
    foreach(StructuralEntity* entity, getStructuralEntities())
      if (entity->getStructuralCategory() != Structural::Edge)
        entity->adjust(true, false);

  StructuralEntity* parent = getStructuralParent();

  if (parent != NULL) {

    if (!collision) {
      // Tries (10x) to find a position where there is no collision
      // with others relatives
      for (int i = 0; i < 10; i++) {
        bool colliding = false;

        foreach(StructuralEntity *entity, parent->getStructuralEntities()) {
          if(this != entity &&
             entity->getStructuralType() >= Structural::Media &&
             entity->getStructuralType() <= Structural::Switch) {

            int max = 1000;
            int n = 0;

            qreal current = 0.0;

            entity->setSelectable(false);

            while(collidesWithItem(entity, Qt::IntersectsItemBoundingRect)) {
              QLineF line = QLineF(getLeft()+getWidth()/2, getTop()+getHeight()/2,
                                   parent->getWidth()/2, parent->getHeight()/2);

              line.setAngle(qrand()%360);

              current += (double)(qrand()%100)/10000.0;

              setTop(getTop()+line.pointAt(current/2).y()-line.p1().y());
              setLeft(getLeft()+line.pointAt(current/2).x()-line.p1().x());

              if (++n > max)
                break;
            }

            inside();

            entity->setSelectable(true);
          }
        }

        foreach(StructuralEntity *entity, parent->getStructuralEntities())
          if(collidesWithItem(entity, Qt::IntersectsItemBoundingRect))
            colliding = true;

        if (!colliding)
          break;
      }
    }

    inside();

    foreach (StructuralEntity* entity, parent->getStructuralEntities()) {
      if (entity->getStructuralCategory() == Structural::Edge) {
        StructuralEdge* edge = (StructuralEdge*) entity;

        if (edge->getTail() == this ||
            edge->getHead() == this) {
          edge->adjust(true);
        }
      }
    }
  }

  if (scene() != NULL)
    scene()->update();
}

void StructuralNode::move(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();

  StructuralEntity* parent = getStructuralParent();

  // setting minimal position
  qreal minx;
  qreal miny;

  if (parent != NULL) {
    minx = STR_DEFAULT_ENTITY_PADDING;
    miny = STR_DEFAULT_ENTITY_PADDING;

  } else {
    minx = 0;
    miny = 0;
  }

  // setting maximal position
  qreal maxx;
  qreal maxy;

  if (parent != NULL) {
    maxx = parent->getWidth() - getWidth() - STR_DEFAULT_ENTITY_PADDING;
    maxy = parent->getHeight() - getHeight() - STR_DEFAULT_ENTITY_PADDING;

  } else {
    maxx = scene()->width() - getWidth();
    maxy = scene()->height() - getHeight();
  }

  // setting delta
  qreal dx = event->pos().x() - getPressLeft(); // (x1 - x0)
  qreal dy = event->pos().y() - getPressTop();  // (y1 - y0)

  // setting next position
  qreal nextx = x + dx;
  qreal nexty = y + dy;

  // moving
  setMoveTop(nexty);
  setMoveLeft(nextx);

  if (scene() != NULL)
    scene()->update();
}

void StructuralNode::resize(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();
  qreal w = getWidth();
  qreal h = getHeight();

  StructuralEntity* parent = getStructuralParent();

  // setting minimal bounds
  qreal minx;
  qreal miny;
  qreal minw;
  qreal minh;

  if (parentItem() != NULL) {
    minx = STR_DEFAULT_ENTITY_PADDING;
    miny = STR_DEFAULT_ENTITY_PADDING;
    minw = -1; // not used
    minh = -1; // not used

  } else {
    minx = 0;
    miny = 0;
    minw = -1; // not used
    minh = -1; // not used
  }

  // setting maximal bounds
  qreal maxx;
  qreal maxy;
  qreal maxw;
  qreal maxh;

  if (parentItem() != NULL) {
    maxx = parent->getWidth() - getWidth() - STR_DEFAULT_ENTITY_PADDING;
    maxy = parent->getHeight() - getHeight() - STR_DEFAULT_ENTITY_PADDING;
    maxw = parent->getWidth() - STR_DEFAULT_ENTITY_PADDING;
    maxh = parent->getHeight() - STR_DEFAULT_ENTITY_PADDING;

  } else {
    maxx = scene()->width() - getWidth();
    maxy = scene()->height() - getHeight();
    maxw = scene()->width();
    maxh = scene()->height();
  }

  // setting delta
  qreal dx = event->pos().x() - getPressLeft();    // (x1 - x0)
  qreal dy = event->pos().y() - getPressTop();     // (y1 - y0)
  qreal dw = -dx;
  qreal dh = -dy;

  // setting next bounds
  qreal nextx = x + dx;
  qreal nexty = y + dy;
  qreal nextw = w + dw;
  qreal nexth = h + dh;

  // adjusting
  switch(getStructuralResize()) {
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

  if (scene() != NULL)
    scene()->update();
}
