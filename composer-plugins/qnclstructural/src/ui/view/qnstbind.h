#ifndef QNSTBIND_H
#define QNSTBIND_H

#include "qnstentity.h"

#include <QMap>

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

    QMap<QString, QString> getParams();

    QString getParam(QString name);

    void setParam(QString name, QString value);

private:
    QString role;

    QString component;

    QString componentUID;

    QString interface;

    QString interfaceUID;

    QMap<QString, QString> params;
};

#endif // QNSTBIND_H
