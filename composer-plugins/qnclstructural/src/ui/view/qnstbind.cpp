#include "qnstbind.h"

QnstBind::QnstBind()
{
    setnstType(Qnst::Bind);
}

QnstBind::~QnstBind()
{

}

QString QnstBind::getRole() const
{
    return role;
}

void QnstBind::setRole(QString role)
{
    this->role = role;
}

QString QnstBind::getComponent() const
{
    return component;
}

void QnstBind::setComponent(QString component)
{
    this->component = component;
}

QString QnstBind::getComponentUid() const
{
    return componentUID;
}

void QnstBind::setComponentUid(QString componentUID)
{
    this->componentUID = componentUID;
}

QString QnstBind::getInterface() const
{
    return interface;
}

void QnstBind::setInterface(QString interface)
{
    this->interface = interface;
}

QString QnstBind::getInterfaceUid() const
{
    return interfaceUID;
}

void QnstBind::setInterfaceUid(QString interfaceUID)
{
    this->interfaceUID = interfaceUID;
}
