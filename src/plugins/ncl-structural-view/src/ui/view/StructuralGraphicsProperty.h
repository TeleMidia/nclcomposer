#ifndef QNSTGRAPHICSPROPERTY_H
#define QNSTGRAPHICSPROPERTY_H

#include "StructuralGraphicsInterface.h"

class StructuralGraphicsProperty : public StructuralGraphicsInterface
{
public:
  StructuralGraphicsProperty(StructuralGraphicsEntity* parent = 0);

  ~StructuralGraphicsProperty();

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTGRAPHICSPROPERTY_H
