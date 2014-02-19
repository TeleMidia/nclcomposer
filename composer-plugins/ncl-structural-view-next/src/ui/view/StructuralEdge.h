#ifndef QNSTGRAPHICSEDGE_H
#define QNSTGRAPHICSEDGE_H

#define PI 3.14159265

#include <cmath>
#include <QLineF>

#include "StructuralEntity.h"

class StructuralEdge : public StructuralEntity
{
public:
  StructuralEdge(StructuralEntity* parent = 0);

  ~StructuralEdge();

  bool isInvalid();

  void setInvalid(bool invalid);

  StructuralEntity* getEntityA() const;

  void setEntityA(StructuralEntity* entity);

  StructuralEntity* getEntityB() const;

  void setEntityB(StructuralEntity* entity);

  bool isEntityAEnabled() const;

  void setEntityAEnabled(bool enable);

  bool isEntityBEnabled() const;

  void setEntityBEnabled(bool enable);

  virtual void adjust(bool avoidCollision = false);

  void aux_adjust(QPointF pointa, QPointF pointb);

  QPointF arcPointAt(QLineF line, qreal at, bool toend = true);

  qreal getAngle();

  void setAngle(qreal angle);

  qreal getAdjAngle();

  void setAdjAngle(qreal adjangle);

protected:
  virtual void draw(QPainter* painter) = 0;

  virtual void delineate(QPainterPath* painter) const = 0;

  virtual void move(QGraphicsSceneMouseEvent* event);

  virtual void resize(QGraphicsSceneMouseEvent* event);

private:
  bool invalid;

  qreal angle;

  qreal adjustedangle;

  bool entityaenabled;

  bool entitybenabled;

  StructuralEntity* entitya;

  StructuralEntity* entityb;
};

// \fixme This class is temporary and must be removed as soon as possible!
// The information of edges should be in the parent, and not inside the own
// entity
class QnstEntityWithEdges : public StructuralEntity
{
public:
  QnstEntityWithEdges(StructuralEntity *parent = 0);
  ~QnstEntityWithEdges();


  QVector<StructuralEdge*> getnstGraphicsEdges();

  void addnstGraphicsEdge(StructuralEdge* edge);

  void removenstGraphicsEdge(StructuralEdge* edge);

private:
    QVector<StructuralEdge*> edges;
};

#endif // QNSTGRAPHICSEDGE_H
