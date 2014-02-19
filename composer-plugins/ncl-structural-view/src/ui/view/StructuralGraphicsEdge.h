#ifndef QNSTGRAPHICSEDGE_H
#define QNSTGRAPHICSEDGE_H

#define PI 3.14159265

#include <cmath>
#include <QLineF>

#include "StructuralGraphicsEntity.h"

class StructuralGraphicsEdge : public StructuralGraphicsEntity
{
public:
  explicit StructuralGraphicsEdge(StructuralGraphicsEntity* parent = 0);

  virtual ~StructuralGraphicsEdge();

  bool isInvalid();
  void setInvalid(bool invalid);

  StructuralGraphicsEntity* getEntityA() const;
  void setEntityA(StructuralGraphicsEntity* entity);
  bool isEntityAEnabled() const;
  void setEntityAEnabled(bool enable);

  StructuralGraphicsEntity* getEntityB() const;
  void setEntityB(StructuralGraphicsEntity* entity);
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

  StructuralGraphicsEntity* entitya;
  StructuralGraphicsEntity* entityb;
};

// \fixme This class is temporary and must be removed as soon as possible!
// The information of edges should be in the parent, and not inside the own
// entity
class QnstGraphicsEntityWithEdges : public StructuralGraphicsEntity
{
public:
  QnstGraphicsEntityWithEdges(StructuralGraphicsEntity *parent = 0);
  ~QnstGraphicsEntityWithEdges();


  QVector<StructuralGraphicsEdge*> getnstGraphicsEdges();

  void addnstGraphicsEdge(StructuralGraphicsEdge* edge);

  void removenstGraphicsEdge(StructuralGraphicsEdge* edge);

private:
    QVector<StructuralGraphicsEdge*> edges;
};

#endif // QNSTGRAPHICSEDGE_H
