#ifndef QNCGGRAPHICSNODE_H
#define QNCGGRAPHICSNODE_H

#include <cmath>

#include "qncggraphicsentity.h"

#define PI 3.14159265

class QncgGraphicsNode : public QncgGraphicsEntity
{
public:
    QncgGraphicsNode(QncgGraphicsEntity* parent = 0);

    ~QncgGraphicsNode();

    virtual void adjust();

    virtual void inside();

    virtual void outside();

    virtual void attract();

    virtual void repel();

    virtual void fit(qreal padding = 0);

    virtual void compact(qreal padding = 0);

    virtual void clock(qreal padding = 0);

protected:
    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);
};

#endif // QNCGGRAPHICSNODE_H
