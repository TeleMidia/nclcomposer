#ifndef QNSTGRAPHICSNODE_H
#define QNSTGRAPHICSNODE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>

#include "StructuralGraphicsEntity.h"
#include "StructuralGraphicsEdge.h"

class StructuralGraphicsNode : public QnstGraphicsEntityWithEdges
{
public:
  StructuralGraphicsNode(StructuralGraphicsEntity* parent = 0);

  ~StructuralGraphicsNode();

  virtual void fit(qreal padding);

  virtual void inside();

  void adjust(bool avoidCollision = true);

protected:
  virtual void draw(QPainter* painter) = 0;

  virtual void delineate(QPainterPath* painter) const = 0;

  virtual void move(QGraphicsSceneMouseEvent* event);

  virtual void resize(QGraphicsSceneMouseEvent* event);

};

#endif // QNSTGRAPHICSNODE_H
