#ifndef QNSTGRAPHICSCONDITION_H
#define QNSTGRAPHICSCONDITION_H

#include "qnstgraphicsedge.h"

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

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

private:
    QnstCondition condition;

    QnstConditionIcon conditionIcon;
};

#endif // QNSTGRAPHICSCONDITION_H
