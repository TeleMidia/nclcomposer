#ifndef QNSTGRAPHICSNODE_H
#define QNSTGRAPHICSNODE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>

#include "qnstgraphicsentity.h"
#include "qnstgraphicsedge.h"

class QnstGraphicsNode : public QnstGraphicsEntity
{
public:
    QnstGraphicsNode(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsNode();

    QVector<QnstGraphicsEdge*> getnstGraphicsEdges();

    void addnstGraphicsEdge(QnstGraphicsEdge* edge);

    void removenstGraphicsEdge(QnstGraphicsEdge* edge);

    virtual void fit(qreal padding);

    virtual void inside();

    virtual void adjust();

    bool hasMouseHover();

    void setMouseHover(bool hover);

protected:
    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);

    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:
    QVector<QnstGraphicsEdge*> edges;

    bool hover;
};

#endif // QNSTGRAPHICSNODE_H
