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

void StructuralInterface::adjust(bool collision,  bool recursion)
{
  StructuralEntity::adjust(collision, recursion);

  // Adjusting position...
  StructuralEntity* parent = getStructuralParent();

  if (parent != NULL) {

    if (!collision) {
      bool colliding = true;

      int max = 10;
      int current = 0;

      do {
        if(current > max)
          break;

        current++; colliding = false;

        foreach(StructuralEntity *e, parent->getStructuralEntities()) {
          if(this != e && e->getStructuralCategory() == Structural::Interface) {
            qreal n = 0;
            qreal index = 0.0;

            e->setSelectable(false);
            e->update();

            while(collidesWithItem(e, Qt::IntersectsItemBoundingRect)) {
              QPointF tail(getLeft()+getWidth()/2, getTop()+getHeight()/2);
              QPointF head(e->getWidth()/2, e->getHeight()/2);

              QLineF line(tail, head);

              line.setAngle(qrand()%360);

              index += (double)(qrand()%100)/10000.0;

              setTop(getTop()+line.pointAt(index/2).y()-tail.y());
              setLeft(getLeft()+line.pointAt(index/2).x()-tail.x());

              if (++n > 1000)
                break;
            }

            constrain();

            e->setSelectable(true);
            e->update();
          }
        }

        foreach(StructuralEntity *e, parent->getStructuralEntities())
          if(collidesWithItem(e, Qt::IntersectsItemBoundingRect))
            colliding = true;

      }while(colliding);
    }

    constrain(); StructuralUtil::adjustEdges(this);
  }
}

void StructuralInterface::constrain()
{
  StructuralEntity* parent = getStructuralParent();

  if (parent != NULL)
  {
    QPointF tail(parent->getWidth()/2, parent->getHeight()/2);
    QPointF head(getLeft() + getWidth()/2, getTop() + getHeight()/2);

    if (tail == head) {
      head.setX(tail.x());
      head.setY(tail.y() - 10);
    }

    QPointF p = head;  QLineF line(tail,head); bool status = true;

    qreal current = 1.0;
    qreal step = 0.01;

    if (!parent->contains(head)) {
      step = -0.01;
      status = false;
    }

    if (parent->contains(line.pointAt(current+step)) == status) {
      while(parent->contains(p) == status) {
        current += step;
        p = line.pointAt(current);
      }
    }

    setTop(p.y() - getHeight()/2);
    setLeft(p.x() - getWidth()/2);
  }
}

void StructuralInterface::draw(QPainter* painter)
{
  int x = STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_INTERFACE_PADDING;
  int y = STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_INTERFACE_PADDING;
  int w = getWidth() - 2*STR_DEFAULT_INTERFACE_PADDING;
  int h = getHeight() - 2*STR_DEFAULT_INTERFACE_PADDING;

  painter->drawPixmap(x, y, w, h, QPixmap(StructuralUtil::getIcon(getStructuralType())));

  if (isMoving()) {
    painter->setPen(QPen(QBrush(Qt::black), 0));
    painter->setBrush(QBrush(Qt::NoBrush));

    int moveX = x + getMoveLeft() - getLeft();
    int moveY = y + getMoveTop() - getTop();
    int moveW = w;
    int moveH = h;

    painter->drawRect(moveX, moveY, moveW, moveH);
  }
}
