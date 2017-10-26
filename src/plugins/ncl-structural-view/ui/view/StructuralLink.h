#ifndef STRUCTURALLINK_H
#define STRUCTURALLINK_H

#include "StructuralNode.h"

class StructuralLink : public StructuralNode
{
  Q_OBJECT

public:
  StructuralLink (StructuralEntity *parent = 0);
  virtual ~StructuralLink ();

signals:
  void performedEdit (StructuralLink *entity);

protected:
  virtual void draw (QPainter *painter);

  virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event);
};

#endif // STRUCTURALLINK_H
