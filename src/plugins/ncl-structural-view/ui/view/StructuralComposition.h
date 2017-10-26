#ifndef STRUCTURALCOMPOSITION_H
#define STRUCTURALCOMPOSITION_H

#include "StructuralNode.h"

class StructuralComposition : public StructuralNode
{
public:
  StructuralComposition (StructuralEntity *parent = 0);
  virtual ~StructuralComposition () {}

  void collapse (bool notify = false);

protected:
  virtual void draw (QPainter *painter);
  virtual void delineate (QPainterPath *painter) const;

  virtual void dragEnterEvent (QGraphicsSceneDragDropEvent *event);
  virtual void dragMoveEvent (QGraphicsSceneDragDropEvent *event);
  virtual void dropEvent (QGraphicsSceneDragDropEvent *event);

  virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event);
};

#endif // STRUCTURALCOMPOSITION_H
