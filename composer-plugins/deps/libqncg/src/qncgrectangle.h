#ifndef QNCGRECTANGLE_H
#define QNCGRECTANGLE_H

#include "qncgnode.h"

class QncgRectangle : public QncgNode
{
public:
    QncgRectangle(QncgEntity* parent = 0);

    ~QncgRectangle();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;
};

#endif // QNCGRECTANGLE_H
