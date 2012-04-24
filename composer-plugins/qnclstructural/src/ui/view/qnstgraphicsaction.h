#ifndef QNSTGRAPHICSACTION_H
#define QNSTGRAPHICSACTION_H

#include "qnstgraphicsedge.h"

#include "qnstgraphicsbinddialog.h"

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

    void setParams(QMap<QString, QString> params);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    QnstAction action;

    QnstActionIcon actionIcon;

    QMap<QString, QString> params;

    QnstGraphicsBindDialog* dialog;
};

#endif // QNSTGRAPHICSACTION_H
