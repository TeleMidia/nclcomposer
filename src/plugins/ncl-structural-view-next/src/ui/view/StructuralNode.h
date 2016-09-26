#ifndef QNSTGRAPHICSNODE_H
#define QNSTGRAPHICSNODE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>

#include "StructuralEntity.h"
//#include "StructuralView.h"

class StructuralView;

class StructuralNode : public StructuralEntity
{
public:
  StructuralNode(StructuralEntity* parent = 0);

  ~StructuralNode();

  virtual void fit(qreal padding);

  virtual void inside();

  virtual void adjust(bool collision = false,  bool recursion = true);

protected:

  virtual void move(QGraphicsSceneMouseEvent* event);
  virtual void resize(QGraphicsSceneMouseEvent* event);

};

#endif // QNSTGRAPHICSNODE_H
