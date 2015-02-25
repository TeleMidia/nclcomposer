#include "StructuralGraphicsMapping.h"

StructuralGraphicsMapping::StructuralGraphicsMapping(StructuralGraphicsEntity* parent)
  : StructuralGraphicsReference(parent)
{
  setnstType(Structural::Mapping);
}


StructuralGraphicsMapping::~StructuralGraphicsMapping()
{

}

QString StructuralGraphicsMapping::getComponent() const
{
  return component;
}

void StructuralGraphicsMapping::setComponent(QString component)
{
  this->component = component;
}

QString StructuralGraphicsMapping::getComponentUid() const
{
  return componentUid;
}

void StructuralGraphicsMapping::setComponentUid(QString componentUid)
{
  this->componentUid = componentUid;
}

QString StructuralGraphicsMapping::getSwitchPortUid() const
{
  return switchportUid;
}

void StructuralGraphicsMapping::setSwitchPortUid(QString switchportUid)
{
  this->switchportUid = switchportUid;
}

QString StructuralGraphicsMapping::getInterface() const
{
  return interface;
}

void StructuralGraphicsMapping::setInterface(QString interface)
{
  this->interface = interface;
}

QString StructuralGraphicsMapping::getInterfaceUid() const
{
  return interfaceUid;
}

void StructuralGraphicsMapping::setInterfaceUid(QString interfaceUid)
{
  this->interfaceUid = interfaceUid;
}

void StructuralGraphicsMapping::setProperties(const QMap<QString, QString> &props)
{
  StructuralGraphicsReference::setProperties(props);

  setComponent(props["component"]);

  if (props["componentUid"] != "")
    setComponentUid(props["componentUid"]);
  else
    setComponentUid("");

  setInterface(props["interface"]);

  if (props["interfaceUid"] != "")
    setInterfaceUid(props["interfaceUid"]);
  else
    setInterfaceUid("");
}

void StructuralGraphicsMapping::getProperties(QMap<QString, QString> &properties)
{
  StructuralGraphicsReference::getProperties(properties);

  properties["component"] = getComponent();
  properties["componentUid"] = getComponentUid();

  properties["interface"] = getInterface();
  properties["interfaceUid"] = getInterfaceUid();
}
