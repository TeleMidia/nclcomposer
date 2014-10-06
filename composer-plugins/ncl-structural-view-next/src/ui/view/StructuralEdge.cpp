#include "StructuralEdge.h"

StructuralEdge::StructuralEdge(StructuralEntity* parent)
  : StructuralEntity(parent)
{
  setLocalType(Structural::Edge);
  setLocalName(Structural::NoName);

  setSelectable(true);
  setResizable(false);
  setMoveable(false);

  entitya = NULL;
  entityb = NULL;

  entityaenabled = true;
  entitybenabled = true;

  angle = 0;

  invalid = false;
}

StructuralEdge::~StructuralEdge()
{

}

bool StructuralEdge::isInvalid()
{
  return invalid;
}

void StructuralEdge::setInvalid(bool invalid)
{
  this->invalid = invalid;
}

StructuralEntity* StructuralEdge::getEntityA() const
{
  return entitya;
}

void StructuralEdge::setEntityA(StructuralEntity* entity)
{
  this->entitya = entity;
}

StructuralEntity* StructuralEdge::getEntityB() const
{
  return entityb;
}

void StructuralEdge::setEntityB(StructuralEntity* entity)
{
  this->entityb = entity;
}

bool StructuralEdge::isEntityAEnabled() const
{
  return entityaenabled;
}

void StructuralEdge::setEntityAEnabled(bool enable)
{
  this->entityaenabled = enable;
}

bool StructuralEdge::isEntityBEnabled() const
{
  return entitybenabled;
}

void StructuralEdge::setEntityBEnabled(bool enable)
{
  this->entitybenabled = enable;
}

qreal StructuralEdge::getAngle()
{
  return angle;
}

void StructuralEdge::setAngle(qreal angle)
{
  this->angle = angle;
}

qreal StructuralEdge::getAdjAngle()
{
  return adjustedangle;
}

void StructuralEdge::setAdjAngle(qreal adjangle)
{
  this->adjustedangle = adjangle;
}

void StructuralEdge::adjust(bool avoidCollision)
{


  if (entitya != NULL && entityb != NULL)
  {
    QLineF line = QLineF(QPointF(entitya->getLeft() + entitya->getWidth()/2,
                                 entitya->getTop() + entitya->getHeight()/2),
                         QPointF(entityb->getLeft() + entityb->getWidth()/2,
                                 entityb->getTop() + entityb->getHeight()/2));

    if (getEntityA()->getLocalType() == Structural::Interface)
    {
      if(getLocalParent())
        line.setP1(getLocalParent()->mapFromItem(getEntityA()->getLocalParent(), line.p1()));
    }

    if (getEntityB()->getLocalType() == Structural::Interface)
    {
      if(getLocalParent())
        line.setP2(getLocalParent()->mapFromItem(getEntityB()->getLocalParent(), line.p2()));
    }

    QPointF pointa = line.p1();
    QPointF pointb = line.p2();

    aux_adjust(pointa, pointb);

    entitya->setSelectable(false);
    entityb->setSelectable(false);

    qreal index;

    if (pointa != pointb){
      index = 1.0;

      int n = 0;

      while(entityb->collidesWithItem(this))
      {
        index -= 0.01;

        if (angle == 0)
          pointb = line.pointAt(index);
        else
          pointb = arcPointAt(line , index);

        aux_adjust(pointa, pointb);

        if (++n > 100) // avoiding infinity loop
        {
          break;
        }
      }

      index = 0;

      n = 0;

      while(entitya->collidesWithItem(this))
      {
        index += 0.01;

        if (angle == 0)
          pointa = line.pointAt(index);
        else
          pointa = arcPointAt(line , index);

        aux_adjust(pointa, pointb);

        if (++n > 100){ // avoiding infinity loop
          break;
        }
      }
    }

    entitya->setSelectable(true);
    entityb->setSelectable(true);

    if (scene() != NULL)
      scene()->update();
  }
}

QPointF StructuralEdge::arcPointAt(QLineF line, qreal at, bool toend)
{
  qreal alfa = getAngle();

  qreal beta = (180 - alfa)/2 + (360 - line.angle());

  qreal R = line.length()/(::sin(((alfa/2)*PI)/180)*2);

  QPointF center_p(line.p2().x() - ::cos((180-beta-alfa)*PI/180)*R,
                   line.p2().y() + ::sin((180-beta-alfa)*PI/180)*R);

  qreal arc_len = alfa*PI*R/180;

  qreal new_arc_len = arc_len*at;

  qreal new_alfa = (180*new_arc_len)/(PI*R);

  qreal gama = (180-beta-new_alfa);

  QPointF new_start_p(center_p.x() + ::cos((gama)*PI/180)*R,
                      center_p.y() - ::sin((gama)*PI/180)*R);

  if (toend)
    this->adjustedangle = new_alfa;
  else
    this->adjustedangle = (180*(arc_len-arc_len*at))/(PI*R);

  return new_start_p;
}

void StructuralEdge::aux_adjust(QPointF pointa, QPointF pointb)
{
  if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
  {
    setTop(pointa.y()-6);
    setLeft(pointa.x()-6);
    setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
    setHeight((pointb.y()-6)-(pointa.y()-6) + 12);
  }
  else if (pointa.x() > pointb.x() && pointa.y() < pointb.y())
  {
    setTop(pointa.y()-6);
    setLeft(pointb.x()-6);
    setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
    setHeight((pointb.y()-6)-(pointa.y()-6) + 12);
  }
  else if (pointa.x() < pointb.x() && pointa.y() > pointb.y())
  {
    setTop(pointb.y()-6);
    setLeft((pointa.x()-6));
    setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
    setHeight((pointa.y()-6)-(pointb.y()-6) + 12);
  }
  else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
  {
    setTop(pointb.y()-6);
    setLeft(pointb.x()-6);
    setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
    setHeight((pointa.y()-6)-(pointb.y()-6) + 12);
  }
}

void StructuralEdge::move(QGraphicsSceneMouseEvent* event)
{
  // nothing to do
}

void StructuralEdge::resize(QGraphicsSceneMouseEvent* event)
{
  // nothing to do
}

QnstEntityWithEdges::QnstEntityWithEdges(StructuralEntity *parent)
  : StructuralEntity(parent)
{

}

QnstEntityWithEdges::~QnstEntityWithEdges()
{

}

QVector<StructuralEdge*> QnstEntityWithEdges::getnstGraphicsEdges()
{
  return edges;
}

void QnstEntityWithEdges::addnstGraphicsEdge(StructuralEdge* edge)
{
  if (edge != NULL)
    edges.append(edge);
}

void QnstEntityWithEdges::removenstGraphicsEdge(StructuralEdge* edge)
{
  if (edge != NULL)
  {
    int index = edges.indexOf(edge);

    if (index >= 0)
      edges.remove(index);
  }
}
