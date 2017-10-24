#ifndef STRUCTURALBIND_H
#define STRUCTURALBIND_H

#include "StructuralEdge.h"

class StructuralBind : public StructuralEdge
{
  Q_OBJECT

public:
  StructuralBind (StructuralEntity *parent = 0);
  virtual ~StructuralBind () {}

  StructuralRole getRole () const;
  void setRole (StructuralRole role);

  virtual void adjust (bool avoidCollision = false, bool rec = true);

signals:
  void performedEdit (StructuralBind *entity);

protected:
  virtual void draw (QPainter *painter);
  virtual void delineate (QPainterPath *painter) const;

  virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event);

private:
  StructuralRole _role;
};

#endif // STRUCTURALBIND_H
