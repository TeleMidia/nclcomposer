#ifndef QNCGRECTANGLEINTERFACE_H
#define QNCGRECTANGLEINTERFACE_H

#include "qncginterface.h"

class QncgRectangleInterface : public QncgInterface
{
public:
    QncgRectangleInterface(QncgEntity* parent = 0);

    ~QncgRectangleInterface();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;
};

#endif // QNCGRECTANGLEINTERFACE_H
