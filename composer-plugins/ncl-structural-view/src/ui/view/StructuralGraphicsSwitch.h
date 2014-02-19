#ifndef QNSTGRAPHICSSWITCH_H
#define QNSTGRAPHICSSWITCH_H

#include "StructuralGraphicsComposition.h"

#include "StructuralGraphicsContext.h"
#include "StructuralGraphicsMedia.h"
#include "StructuralGraphicsPort.h"
#include "StructuralGraphicsSwitchPort.h"

class StructuralGraphicsSwitch : public StructuralGraphicsComposition
{
  Q_OBJECT

public:
  StructuralGraphicsSwitch(StructuralGraphicsEntity* parent = 0);

  ~StructuralGraphicsSwitch();

private:
  void createObjects();

  void createConnections();
};


#endif // QNSTGRAPHICSSWITCH_H
