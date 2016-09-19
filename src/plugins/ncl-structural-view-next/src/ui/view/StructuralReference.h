#ifndef STRUCTURALREFERENCE_H
#define STRUCTURALREFERENCE_H

#include "StructuralEdge.h"

class StructuralReference : public StructuralEdge
{
public:
  StructuralReference(StructuralEntity* parent = 0);
  virtual ~StructuralReference();

protected:
  virtual void draw(QPainter* painter);
  virtual void delineate(QPainterPath* painter) const;
};

#endif // STRUCTURALREFERENCE_H
