#ifndef QNSTGRAPHICSSWITCHPORT_H
#define QNSTGRAPHICSSWITCHPORT_H

#include "StructuralGraphicsPort.h"

class StructuralGraphicsSwitchPort : public StructuralGraphicsPort
{
public:
  StructuralGraphicsSwitchPort(StructuralGraphicsEntity* parent = 0);

  ~StructuralGraphicsSwitchPort();
};

#endif // QNSTGRAPHICSSWITCHPORT_H
