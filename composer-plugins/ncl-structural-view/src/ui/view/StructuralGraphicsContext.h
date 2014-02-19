#ifndef QNSTGRAPHICSCONTEXT_H
#define QNSTGRAPHICSCONTEXT_H

#include "StructuralGraphicsComposition.h"

#include "StructuralGraphicsSwitch.h"
#include "StructuralGraphicsMedia.h"
#include "StructuralGraphicsPort.h"

class StructuralGraphicsContext : public StructuralGraphicsComposition
{
  Q_OBJECT

public:
  StructuralGraphicsContext(StructuralGraphicsEntity* parent = 0);

  ~StructuralGraphicsContext();

private:
  void createObjects();

  void createConnections();
};

#endif // QNSTGRAPHICSCONTEXT_H
