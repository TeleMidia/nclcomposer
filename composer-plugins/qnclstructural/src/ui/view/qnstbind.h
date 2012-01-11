#ifndef QNSTBIND_H
#define QNSTBIND_H

#include "qnstentity.h"

class QnstBind : public QnstEntity
{
public:
    QnstBind();

    ~QnstBind();

    QString getRole() const;

    void setRole(QString role);

    QString getComponent() const;

    void setComponent(QString component);

    QString getComponentUid() const;

    void setComponentUid(QString componentUid);

    QString getInterface() const;

    void setInterface(QString interface);

    QString getInterfaceUid() const;

    void setInterfaceUid(QString interfaceUid);

private:
    QString role;

    QString component;

    QString componentUID;

    QString interface;

    QString interfaceUID;
};

#endif // QNSTBIND_H
