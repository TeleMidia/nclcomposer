#ifndef QNCGINTERFACE_H
#define QNCGINTERFACE_H

#include "qncgentity.h"
#include "qncgedge.h"

class QncgInterface : public QncgEntity
{
public:
    QncgInterface(QncgEntity* parent = 0);

    ~QncgInterface();

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

#endif // QNCGINTERFACE_H
