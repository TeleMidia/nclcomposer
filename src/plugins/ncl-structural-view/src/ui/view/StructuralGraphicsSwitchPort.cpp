#include "StructuralGraphicsSwitchPort.h"

StructuralGraphicsSwitchPort::StructuralGraphicsSwitchPort(StructuralGraphicsEntity* parent)
    : StructuralGraphicsPort(parent)
{
    setnstType(Structural::SwitchPort);
}


StructuralGraphicsSwitchPort::~StructuralGraphicsSwitchPort()
{

}
