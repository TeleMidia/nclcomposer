#ifndef QNSTGRAPHICSCOMPOSITION_H
#define QNSTGRAPHICSCOMPOSITION_H

#include "qnst.h"
#include "qnstgraphicsnode.h"

#include <QGraphicsSceneMouseEvent>

class QnstGraphicsComposition : public QnstGraphicsNode
{
public:
    QnstGraphicsComposition(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsComposition();

    QString getColor() const;

    void setColor(QString color);

    void setnstId(QString id);

    void setCollpsed(bool collapsed);

    bool isCollpsed();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    virtual bool createEntity(Qnst::EntityType type);

private:
    bool collapsed;

    QString color;

    QString dropsrc;

    qreal lastw;

    qreal lasth;

    QnstGraphicsNode* tmp;
};

#endif // QNSTGRAPHICSCOMPOSITION_H
