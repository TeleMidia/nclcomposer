#ifndef QNSTGRAPHICSACTION_H
#define QNSTGRAPHICSACTION_H

#include "qnstgraphicsedge.h"

typedef QString QnstActionIcon;
typedef Qnst::ActionType QnstAction;

class QnstGraphicsAction : public QnstGraphicsEdge
{
public:
    QnstGraphicsAction(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsAction();

    QnstAction getAction();

    void setAction(QnstAction action);

    QnstActionIcon getActionIcon();

    void setActionIcon(QnstActionIcon actionIcon);

    void adjust();

    void aux_adjust(QPointF pointa, QPointF pointb);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

private:
    QnstAction action;

    QnstActionIcon actionIcon;
};

#endif // QNSTGRAPHICSACTION_H
