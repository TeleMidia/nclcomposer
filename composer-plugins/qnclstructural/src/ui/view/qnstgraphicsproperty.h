#ifndef QNSTGRAPHICSPROPERTY_H
#define QNSTGRAPHICSPROPERTY_H

#include "qnstgraphicsinterface.h"

class QnstGraphicsProperty : public QnstGraphicsInterface
{
public:
    QnstGraphicsProperty(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsProperty();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTGRAPHICSPROPERTY_H
