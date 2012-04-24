#ifndef QNSTGRAPHICSCONDITION_H
#define QNSTGRAPHICSCONDITION_H

#include "qnstgraphicsedge.h"

#include "qnstgraphicsbinddialog.h"

typedef QString QnstConditionIcon;
typedef Qnst::ConditionType QnstCondition;

class QnstGraphicsCondition : public QnstGraphicsEdge
{
public:
    QnstGraphicsCondition(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsCondition();

    QnstCondition getCondition();

    void setCondition(QnstCondition condition);

    QnstConditionIcon getConditionIcon();

    void setConditionIcon(QnstConditionIcon conditionIcon);

    void adjust();

    void aux_adjust(QPointF pointa, QPointF pointb);

    void setParams(QMap<QString, QString> params);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    QnstCondition condition;

    QnstConditionIcon conditionIcon;

    QMap<QString, QString> params;

    QnstGraphicsBindDialog* dialog;
};

#endif // QNSTGRAPHICSCONDITION_H
