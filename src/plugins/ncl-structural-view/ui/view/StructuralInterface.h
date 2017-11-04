#ifndef STRUCTURALINTERFACE_H
#define STRUCTURALINTERFACE_H

#include "StructuralEdge.h"
#include "StructuralEntity.h"

class StructuralInterface : public StructuralEntity
{
public:
  StructuralInterface (StructuralEntity *structuralParent = 0);
  virtual ~StructuralInterface () {}

  virtual void adjust (bool collision = false, bool recursion = true);

protected:
  virtual void draw (QPainter *painter);

private:
  void constrain ();
};

#endif // STRUCTURALINTERFACE_H
