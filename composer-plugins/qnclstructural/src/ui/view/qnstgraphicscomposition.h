#ifndef QNSTGRAPHICSCOMPOSITION_H
#define QNSTGRAPHICSCOMPOSITION_H

#include "qnst.h"
#include "qnstgraphicsnode.h"

class QnstGraphicsComposition : public QnstGraphicsNode
{
public:
    QnstGraphicsComposition(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsComposition();

    QString getColor() const;

    void setColor(QString color);

    void setnstId(QString id);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

    virtual bool createEntity(Qnst::EntityType type);

private:
    QString color;

    QString dropsrc;
};

#endif // QNSTGRAPHICSCOMPOSITION_H
