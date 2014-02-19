#ifndef QNSTGRAPHICSAREA_H
#define QNSTGRAPHICSAREA_H

#include "StructuralGraphicsInterface.h"

class StructuralGraphicsArea : public StructuralGraphicsInterface
{
public:
  StructuralGraphicsArea(StructuralGraphicsEntity* parent = 0);

  ~StructuralGraphicsArea();

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

};

#endif // QNSTGRAPHICSAREA_H
