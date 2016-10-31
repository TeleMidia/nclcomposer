#ifndef STRUCTURALINTERFACE_H
#define STRUCTURALINTERFACE_H

#include "StructuralEntity.h"
#include "StructuralEdge.h"

class StructuralInterface : public StructuralEntity
{
public:
  StructuralInterface(StructuralEntity* parent = 0);
  virtual ~StructuralInterface();

  virtual void adjust(bool collision = false,  bool recursion = true);

protected:
  virtual void draw(QPainter* painter);

private:
  void constrain();
};

#endif // STRUCTURALINTERFACE_H
