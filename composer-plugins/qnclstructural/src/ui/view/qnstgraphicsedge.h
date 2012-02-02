#ifndef QNSTGRAPHICSEDGE_H
#define QNSTGRAPHICSEDGE_H

#define PI 3.14159265

#include <cmath>
#include <QLineF>

#include "qnstgraphicsentity.h"

class QnstGraphicsEdge : public QnstGraphicsEntity
{
public:
    QnstGraphicsEdge(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsEdge();

    QnstGraphicsEntity* getEntityA() const;

    void setEntityA(QnstGraphicsEntity* entity);

    QnstGraphicsEntity* getEntityB() const;

    void setEntityB(QnstGraphicsEntity* entity);

    bool isEntityAEnabled() const;

    void setEntityAEnabled(bool enable);

    bool isEntityBEnabled() const;

    void setEntityBEnabled(bool enable);

    virtual void adjust();

    void aux_adjust(QPointF pointa, QPointF pointb);

    QPointF arcPointAt(QLineF line, qreal at, bool toend = true);

    qreal getAngle();

    void setAngle(qreal angle);

    qreal getAdjAngle();

    void setAdjAngle(qreal adjangle);

protected:
    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);

private:
    qreal angle;

    qreal adjustedangle;

    bool entityaenabled;

    bool entitybenabled;

    QnstGraphicsEntity* entitya;

    QnstGraphicsEntity* entityb;
};

#endif // QNSTGRAPHICSEDGE_H
