#include "qnstgraphicsport.h"

QnstGraphicsPort::QnstGraphicsPort(QnstGraphicsEntity* parent)
    : QnstGraphicsInterface(parent)
{
    setnstType(Qnst::Port);
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

void QnstGraphicsPort::draw(QPainter* painter)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter->drawPixmap(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8, QPixmap(":/icon/port"));

    if (isMoving()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawRect(getMoveLeft()+4-getLeft(),getMoveTop()+4-getTop(),getWidth()-1,getHeight()-1);
    }
}

void QnstGraphicsPort::delineate(QPainterPath* painter) const
{
    painter->addRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);
}
