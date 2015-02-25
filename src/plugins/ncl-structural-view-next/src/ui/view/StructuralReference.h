#ifndef QNSTGRAPHICSREFERENCE_H
#define QNSTGRAPHICSREFERENCE_H

#include "StructuralEdge.h"

class StructuralReference : public StructuralEdge
{
public:
  StructuralReference(StructuralEntity* parent = 0);

  ~StructuralReference();

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTGRAPHICSREFERENCE_H
