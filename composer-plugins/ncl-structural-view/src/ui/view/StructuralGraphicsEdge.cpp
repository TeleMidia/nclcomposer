#include "StructuralGraphicsEdge.h"

StructuralGraphicsEdge::StructuralGraphicsEdge(StructuralGraphicsEntity* parent)
  : StructuralGraphicsEntity(parent)
{
  setncgType(Qncg::Edge);
  setnstType(Structural::Edge);

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

StructuralGraphicsEdge::~StructuralGraphicsEdge()
{

}

bool StructuralGraphicsEdge::isInvalid()
{
  return invalid;
}

void StructuralGraphicsEdge::setInvalid(bool invalid)
{
  this->invalid = invalid;
}

StructuralGraphicsEntity* StructuralGraphicsEdge::getEntityA() const
{
  return entitya;
}

void StructuralGraphicsEdge::setEntityA(StructuralGraphicsEntity* entity)
{
  this->entitya = entity;
}

StructuralGraphicsEntity* StructuralGraphicsEdge::getEntityB() const
{
  return entityb;
}

void StructuralGraphicsEdge::setEntityB(StructuralGraphicsEntity* entity)
{
  this->entityb = entity;
}

bool StructuralGraphicsEdge::isEntityAEnabled() const
{
  return entityaenabled;
}

void StructuralGraphicsEdge::setEntityAEnabled(bool enable)
{
  this->entityaenabled = enable;
}

bool StructuralGraphicsEdge::isEntityBEnabled() const
{
  return entitybenabled;
}

void StructuralGraphicsEdge::setEntityBEnabled(bool enable)
{
  this->entitybenabled = enable;
}

qreal StructuralGraphicsEdge::getAngle()
{
  return angle;
}

void StructuralGraphicsEdge::setAngle(qreal angle)
{
  this->angle = angle;
}

qreal StructuralGraphicsEdge::getAdjAngle()
{
  return adjustedangle;
}

void StructuralGraphicsEdge::setAdjAngle(qreal adjangle)
{
  this->adjustedangle = adjangle;
}

void StructuralGraphicsEdge::adjust(bool avoidCollision)
{
  Q_UNUSED(avoidCollision)

  if (entitya != NULL && entityb != NULL)
  {
    QLineF line = QLineF(QPointF(entitya->getLeft() + entitya->getWidth()/2,
                                 entitya->getTop() + entitya->getHeight()/2),
                         QPointF(entityb->getLeft() + entityb->getWidth()/2,
                                 entityb->getTop() + entityb->getHeight()/2));

    if (getEntityA()->getncgType() == Qncg::Interface)
    {
      if(getnstGraphicsParent())
        line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
    }

    if (getEntityB()->getncgType() == Qncg::Interface)
    {
      if(getnstGraphicsParent())
        line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
    }

    QPointF pointa = line.p1();
    QPointF pointb = line.p2();

    aux_adjust(pointa, pointb);

    entitya->setSelectable(false);
    entityb->setSelectable(false);

    qreal index; int n;

    if (pointa != pointb)
    {
      index = 1.0; n = 0;

      if (LINK_WITH_PARENT && entityb == getnstGraphicsParent())
      {
        while(!entityb->collidesWithItem(this))
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

        index -= 0.01;

        if (angle == 0)
          pointb = line.pointAt(index);
        else
          pointb = arcPointAt(line , index);

        aux_adjust(pointa, pointb);

      }
      else
      {
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
      }

      index = 0; n = 0;

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

QPointF StructuralGraphicsEdge::arcPointAt(QLineF line, qreal at, bool toend)
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

void StructuralGraphicsEdge::aux_adjust(QPointF pointa, QPointF pointb)
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

void StructuralGraphicsEdge::move(QGraphicsSceneMouseEvent* event)
{
  Q_UNUSED(event)
  // nothing to do
}

void StructuralGraphicsEdge::resize(QGraphicsSceneMouseEvent* event)
{
  Q_UNUSED(event)
  // nothing to do
}

QnstGraphicsEntityWithEdges::QnstGraphicsEntityWithEdges(StructuralGraphicsEntity *parent)
  : StructuralGraphicsEntity(parent)
{

}

QnstGraphicsEntityWithEdges::~QnstGraphicsEntityWithEdges()
{

}

QVector<StructuralGraphicsEdge*> QnstGraphicsEntityWithEdges::getnstGraphicsEdges()
{
  return edges;
}

void QnstGraphicsEntityWithEdges::addnstGraphicsEdge(StructuralGraphicsEdge* edge)
{
  if (edge != NULL)
    edges.append(edge);
}

void QnstGraphicsEntityWithEdges::removenstGraphicsEdge(StructuralGraphicsEdge* edge)
{
  if (edge != NULL)
  {
    int index = edges.indexOf(edge);

    if (index >= 0)
      edges.remove(index);
  }
}
