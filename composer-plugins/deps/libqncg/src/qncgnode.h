#ifndef QNCGNODE_H
#define QNCGNODE_H

#include <QGraphicsScene>
#include <QVector>

#include "qncgentity.h"
#include "qncgedge.h"

class QncgEdge;

class QncgNode : public QncgEntity
{
public:
    QncgNode(QncgEntity* parent = 0);

    ~QncgNode();

    virtual void adjust();

    void addBeginningEdge(QncgEdge* edge);

    void addEndingEdge(QncgEdge* edge);

protected:
    virtual void validate();

    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void link(QGraphicsSceneMouseEvent* event);

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);

private:
    QVector<QncgEdge*> bedges;

    QVector<QncgEdge*> eedges;
};

#endif // QNCGNODE_H
