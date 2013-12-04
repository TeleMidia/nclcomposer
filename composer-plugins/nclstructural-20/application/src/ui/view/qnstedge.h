#ifndef QNSTGRAPHICSEDGE_H
#define QNSTGRAPHICSEDGE_H

#define PI 3.14159265

#include <cmath>
#include <QLineF>

#include "qnstentity.h"

class QnstEdge : public QnstEntity
{
public:
  QnstEdge(QnstEntity* parent = 0);

  ~QnstEdge();

  bool isInvalid();

  void setInvalid(bool invalid);

  QnstEntity* getEntityA() const;

  void setEntityA(QnstEntity* entity);

  QnstEntity* getEntityB() const;

  void setEntityB(QnstEntity* entity);

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

  QnstEntity* entitya;

  QnstEntity* entityb;
};

// \fixme This class is temporary and must be removed as soon as possible!
// The information of edges should be in the parent, and not inside the own
// entity
class QnstEntityWithEdges : public QnstEntity
{
public:
  QnstEntityWithEdges(QnstEntity *parent = 0);
  ~QnstEntityWithEdges();


  QVector<QnstEdge*> getnstGraphicsEdges();

  void addnstGraphicsEdge(QnstEdge* edge);

  void removenstGraphicsEdge(QnstEdge* edge);

private:
    QVector<QnstEdge*> edges;
};

#endif // QNSTGRAPHICSEDGE_H
