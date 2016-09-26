#ifndef STRUCTURALNODE_H
#define STRUCTURALNODE_H

#include "StructuralEntity.h"

class StructuralNode : public StructuralEntity
{
public:
  StructuralNode(StructuralEntity* parent = 0);
  virtual ~StructuralNode();

  virtual void inside();

  virtual void adjust(bool collision = false,  bool recursion = true);

protected:
  virtual void move(QGraphicsSceneMouseEvent* event);
  virtual void resize(QGraphicsSceneMouseEvent* event);
};

#endif // STRUCTURALNODE_H
