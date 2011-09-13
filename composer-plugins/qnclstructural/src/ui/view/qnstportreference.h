#ifndef QNSTPORTREFERENCE_H
#define QNSTPORTREFERENCE_H

#include <cmath>
#include "qncgedge.h"

class QnstPortReference : public QncgEdge
{
public:
    QnstPortReference(QncgEntity* parent = 0);

    ~QnstPortReference();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTPORTREFERENCE_H
