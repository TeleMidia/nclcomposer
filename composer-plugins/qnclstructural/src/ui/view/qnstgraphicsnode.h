#ifndef QNSTGRAPHICSNODE_H
#define QNSTGRAPHICSNODE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>

#include "qnstgraphicsentity.h"
#include "qnstgraphicsedge.h"

class QnstGraphicsNode : public QnstGraphicsEntityWithEdges
{
public:
  QnstGraphicsNode(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsNode();

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
