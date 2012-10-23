#include "qnstgraphicsport.h"

QnstGraphicsPort::QnstGraphicsPort(QnstGraphicsEntity* parent)
  : QnstGraphicsInterface(parent)
{
  setnstType(Qnst::Port);

  setnstId("");
}

QnstGraphicsPort::~QnstGraphicsPort()
{

}

QString QnstGraphicsPort::getComponent() const
{
  return component;
}

void QnstGraphicsPort::setComponent(QString component)
{
  this->component = component;
}

QString QnstGraphicsPort::getComponentUid() const
{
  return componentUid;
}

void QnstGraphicsPort::setComponentUid(QString componentUid)
{
  this->componentUid = componentUid;
}

QString QnstGraphicsPort::getInterface() const
{
  return interface;
}

void QnstGraphicsPort::setInterface(QString interface)
{
  this->interface = interface;
}

QString QnstGraphicsPort::getInterfaceUid() const
{
  return interfaceUid;
}

void QnstGraphicsPort::setInterfaceUid(QString interfaceUid)
{
  this->interfaceUid = interfaceUid;
}

void QnstGraphicsPort::setProperties(const QMap<QString, QString> &properties)
{
  QnstGraphicsInterface::setProperties(properties);

  setComponent(properties["component"]);
  setInterface(properties["interface"]);

  if (properties["interfaceUid"] != "")
    setInterfaceUid(properties["interfaceUid"]);
  else
    setInterfaceUid("");

  if (properties["componentUid"] != "")
    setComponentUid(properties["componentUid"]);
  else
    setComponentUid("");
}

void QnstGraphicsPort::draw(QPainter* painter)
{
  painter->setBrush(Qt::black);

  painter->drawRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);

  // Draw MouseOver rectangle
  if (!isSelected() && hasMouseHover())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(QColor("#999999")), 0, Qt::DashLine)); // 0px = cosmetic border

    painter->drawRect(4, 4, getWidth(), getHeight());
  }

  if (isMoving())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

    painter->drawRect(getMoveLeft()+4-getLeft(),getMoveTop()+4-getTop(),getWidth()-1,getHeight()-1);
  }
}

void QnstGraphicsPort::delineate(QPainterPath* painter) const
{
  painter->addRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);
}
