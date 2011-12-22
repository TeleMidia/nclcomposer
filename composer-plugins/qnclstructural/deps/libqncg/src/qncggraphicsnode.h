#ifndef QNCGGRAPHICSNODE_H
#define QNCGGRAPHICSNODE_H

#include "qncggraphicsentity.h"

class QncgGraphicsNode : public QncgGraphicsEntity
{
public:
    QncgGraphicsNode(QncgGraphicsEntity* parent = 0);

    ~QncgGraphicsNode();

    virtual void adjust();

protected:
    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);
};

#endif // QNCGGRAPHICSNODE_H
