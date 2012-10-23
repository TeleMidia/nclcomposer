#include "qnstgraphicsmapping.h"

QnstGraphicsMapping::QnstGraphicsMapping(QnstGraphicsEntity* parent)
  : QnstGraphicsReference(parent)
{
  setnstType(Qnst::Mapping);
}


QnstGraphicsMapping::~QnstGraphicsMapping()
{

}

QString QnstGraphicsMapping::getComponent() const
{
  return component;
}

void QnstGraphicsMapping::setComponent(QString component)
{
  this->component = component;
}

QString QnstGraphicsMapping::getComponentUid() const
{
  return componentUid;
}

void QnstGraphicsMapping::setComponentUid(QString componentUid)
{
  this->componentUid = componentUid;
}

QString QnstGraphicsMapping::getSwitchPortUid() const
{
  return switchportUid;
}

void QnstGraphicsMapping::setSwitchPortUid(QString switchportUid)
{
  this->switchportUid = switchportUid;
}

QString QnstGraphicsMapping::getInterface() const
{
  return interface;
}

void QnstGraphicsMapping::setInterface(QString interface)
{
  this->interface = interface;
}

QString QnstGraphicsMapping::getInterfaceUid() const
{
  return interfaceUid;
}

void QnstGraphicsMapping::setInterfaceUid(QString interfaceUid)
{
  this->interfaceUid = interfaceUid;
}

void QnstGraphicsMapping::setProperties(const QMap<QString, QString> &props)
{
  QnstGraphicsReference::setProperties(props);

  setComponent(props["component"]);
  setInterface(props["interface"]);

  if (props["interfaceUid"] != "")
    setInterfaceUid(props["interfaceUid"]);
  else
    setInterfaceUid("");

  if (props["componentUid"] != "")
    setComponentUid(props["componentUid"]);
  else
    setComponentUid("");
}
