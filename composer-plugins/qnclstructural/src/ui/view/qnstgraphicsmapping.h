#ifndef QNSTGRAPHICSMAPPING_H
#define QNSTGRAPHICSMAPPING_H

#include "qnstgraphicsreference.h"

class QnstGraphicsMapping : public QnstGraphicsReference
{
public:
    QnstGraphicsMapping(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsMapping();

    QString getComponent() const;

    void setComponent(QString component);

    QString getComponentUid() const;

    void setComponentUid(QString componentUid);

    QString getInterface() const;

    void setInterface(QString interface);

    QString getInterfaceUid() const;

    void setInterfaceUid(QString interfaceUid);


private:
    QString component;

    QString componentUid;

    QString interface;

    QString interfaceUid;
};

#endif // QNSTGRAPHICSMAPPING_H
