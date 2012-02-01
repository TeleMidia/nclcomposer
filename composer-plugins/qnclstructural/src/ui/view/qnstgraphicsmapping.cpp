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
