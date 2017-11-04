#ifndef STRUCTURALNODE_H
#define STRUCTURALNODE_H

#include "StructuralEntity.h"

class StructuralNode : public StructuralEntity
{
public:
  StructuralNode (StructuralEntity *structuralParent = 0);
  virtual ~StructuralNode ();

  virtual void adjust (bool collision = false, bool recursion = true);

protected:
  virtual void inside ();
};

#endif // STRUCTURALNODE_H
