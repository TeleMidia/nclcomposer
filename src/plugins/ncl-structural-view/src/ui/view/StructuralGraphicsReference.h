#ifndef QNSTGRAPHICSREFERENCE_H
#define QNSTGRAPHICSREFERENCE_H

#include "StructuralGraphicsEdge.h"

class StructuralGraphicsReference : public StructuralGraphicsEdge
{
public:
  StructuralGraphicsReference(StructuralGraphicsEntity* parent = 0);

  virtual ~StructuralGraphicsReference();

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTGRAPHICSREFERENCE_H
