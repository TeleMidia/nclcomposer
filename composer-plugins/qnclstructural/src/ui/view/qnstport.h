#ifndef QNSTPORT_H
#define QNSTPORT_H

#include "qnstinterface.h"
#include "qnstportreference.h"

class QnstPort : public QnstInterface
{
public:
    QnstPort(QnstNode* parent = 0);

    ~QnstPort();

protected:
    virtual void link(QGraphicsSceneMouseEvent* event);
};

#endif // QNSTPORT_H
