#ifndef QNSTGRAPHICSINTERFACE_H
#define QNSTGRAPHICSINTERFACE_H

#include "qnstgraphicsentity.h"
#include "qnstgraphicsedge.h"

class QnstGraphicsInterface : public QnstGraphicsEntity
{
public:
    QnstGraphicsInterface(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsInterface();

    QVector<QnstGraphicsEdge*> getnstGraphicsEdges();

    void addnstGraphicsEdge(QnstGraphicsEdge* edge);

    void removenstGraphicsEdge(QnstGraphicsEdge* edge);

    virtual void adjust(bool avoidCollision = false);

    void setnstId(QString id);

protected:
    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);

private:
    QVector<QnstGraphicsEdge*> links;
};

#endif // QNSTGRAPHICSINTERFACE_H
