#ifndef QNSTGRAPHICSPORT_H
#define QNSTGRAPHICSPORT_H

#include "qnstgraphicsinterface.h"

class QnstGraphicsPort : public QnstGraphicsInterface
{
public:
    QnstGraphicsPort(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsPort();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTGRAPHICSPORT_H
