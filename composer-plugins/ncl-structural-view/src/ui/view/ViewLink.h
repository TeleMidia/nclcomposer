#ifndef QNSTVIEWLINK_H
#define QNSTVIEWLINK_H

#define PI 3.14159265

#include <cmath>

#include "Entity.h"

class ViewLink : public QnstEntity
{
public:
    ViewLink(QnstEntity* parent = 0);

    ~ViewLink();

    QLineF getLine() const;

    void setLine(QLineF line);

    virtual void adjust(bool avoidCollision = false);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);

private:
    QLineF line;
};

#endif // QNSTVIEWLINK_H
