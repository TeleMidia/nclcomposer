#ifndef QNCGGRAPHICSINTERFACE_H
#define QNCGGRAPHICSINTERFACE_H

#include "qncggraphicsentity.h"

class QncgGraphicsInterface : public QncgGraphicsEntity
{
public:
    QncgGraphicsInterface(QncgGraphicsEntity* parent = 0);

    ~QncgGraphicsInterface();

    virtual void adjust();

protected:
    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);
};

#endif // QNCGGRAPHICSINTERFACE_H
