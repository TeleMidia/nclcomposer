#include "StructuralGraphicsPort.h"

StructuralGraphicsPort::StructuralGraphicsPort(StructuralGraphicsEntity* parent)
  : StructuralGraphicsInterface(parent)
{
  setnstType(Structural::Port);

  setnstId("");
}

StructuralGraphicsPort::~StructuralGraphicsPort()
{

}

QString StructuralGraphicsPort::getComponent() const
{
  return component;
}

void StructuralGraphicsPort::setComponent(QString component)
{
  this->component = component;
}

QString StructuralGraphicsPort::getComponentUid() const
{
  return componentUid;
}

void StructuralGraphicsPort::setComponentUid(QString componentUid)
{
  this->componentUid = componentUid;
}

QString StructuralGraphicsPort::getInterface() const
{
  return interface;
}

void StructuralGraphicsPort::setInterface(QString interface)
{
  this->interface = interface;
}

QString StructuralGraphicsPort::getInterfaceUid() const
{
  return interfaceUid;
}

void StructuralGraphicsPort::setInterfaceUid(QString interfaceUid)
{
  this->interfaceUid = interfaceUid;
}

void StructuralGraphicsPort::draw(QPainter* painter)
{
  painter->setBrush(Qt::black);

  painter->drawRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);

  drawMouseHoverHighlight(painter); // This should not be HERE!!

  if(hasError)
  {
    painter->drawPixmap((getWidth()-16)/2 + 12, (getHeight()-8)/2 + 4, 12, 12, QPixmap(":/icon/delete"));
  }

  if (isMoving())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

    painter->drawRect(getMoveLeft()+4-getLeft(),getMoveTop()+4-getTop(),getWidth()-1,getHeight()-1);
  }
}

void StructuralGraphicsPort::delineate(QPainterPath* painter) const
{
  painter->addRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);
}

void StructuralGraphicsPort::setProperties(const QMap<QString, QString> &properties)
{
  StructuralGraphicsInterface::setProperties(properties);

  setComponent(properties["component"]);

  if (properties["componentUid"] != "")
    setComponentUid(properties["componentUid"]);
  else
    setComponentUid("");

  setInterface(properties["interface"]);

  if (properties["interfaceUid"] != "")
    setInterfaceUid(properties["interfaceUid"]);
  else
    setInterfaceUid("");
}

void StructuralGraphicsPort::getProperties(QMap<QString, QString> &properties)
{
  StructuralGraphicsInterface::getProperties(properties);

  properties["component"] = getComponent();
  properties["componentUid"] = getComponentUid();

  properties["interface"] = getInterface();
  properties["interfaceUid"] = getInterfaceUid();

}
