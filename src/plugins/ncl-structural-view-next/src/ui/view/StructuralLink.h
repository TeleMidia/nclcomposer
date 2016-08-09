#ifndef QNSTGRAPHICSLINK_H
#define QNSTGRAPHICSLINK_H

#include "StructuralNode.h"
#include "StructuralBind.h"

class StructuralBind;

// \todo The qnstgraphicsaggregator and qnstlink must be part of this class.
class StructuralLink : public StructuralNode
{
public:
  StructuralLink(StructuralEntity* parent = 0);

  ~StructuralLink();

  virtual void refresh();

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

};

#endif // QNSTGRAPHICSLINK_H
