#ifndef QNSTGRAPHICSNODE_H
#define QNSTGRAPHICSNODE_H

#include <QMenu>
#include <QAction>
#include <QVector>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>

#include <QDebug>

#include "qncggraphicsnode.h"

#include "core/qnstentity.h"

class QnstGraphicsNode : public QncgGraphicsNode, public QnstEntity
{
    Q_OBJECT

public:
    QnstGraphicsNode(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsNode();

signals:
    void entityAdded(QnstEntity* entity);

    void entityRemoved(QnstEntity* entity);

    void entityChanged(QnstEntity* entity);

    void entitySelected(QnstEntity* entity);

protected:
    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

protected slots:
    void requestEntityChange();

    void requestEntitySelection();
};

#endif // QNSTGRAPHICSNODE_H
