#include "qnstgraphicslink.h"

QnstGraphicsLink::QnstGraphicsLink(QnstGraphicsEntity* parent)
    : QnstGraphicsEdge(parent)
{
    setnstType(Qnst::Link);

    setAction(Qnst::NoActionType);
    setCondition(Qnst::NoConditionType);
}

QnstGraphicsLink::~QnstGraphicsLink()
{

}

QnstAction QnstGraphicsLink::getAction()
{
    return action;
}

void QnstGraphicsLink::setAction(QnstAction action)
{
    this->action = action;

    switch(action){
    case Qnst::Start:
        setActionIcon(QnstConditionIcon(":/icon/start"));
        break;

    case Qnst::Stop:
        setActionIcon(QnstConditionIcon(":/icon/stop"));
        break;

    case Qnst::Resume:
        setActionIcon(QnstConditionIcon(":/icon/resume"));
        break;

    case Qnst::Pause:
        setActionIcon(QnstConditionIcon(":/icon/pause"));
        break;

    case Qnst::Set:
        setActionIcon(QnstConditionIcon(":/icon/set"));
        break;

    case Qnst::NoActionType:
        setActionIcon(QnstConditionIcon(":/icon/noaction"));
        break;
    }
}

QnstActionIcon QnstGraphicsLink::getActionIcon()
{
    return actionIcon;
}

void QnstGraphicsLink::setActionIcon(QnstActionIcon actionIcon)
{
    this->actionIcon = actionIcon;
}

QnstCondition QnstGraphicsLink::getCondition()
{
    return condition;
}

void QnstGraphicsLink::setCondition(QnstCondition condition)
{
    this->condition = condition;

    switch(condition){
    case Qnst::onBegin:
        setConditionIcon(QnstConditionIcon(":/icon/onbegin"));
        break;

    case Qnst::onEnd:
        setConditionIcon(QnstConditionIcon(":/icon/onend"));
        break;

    case Qnst::onResume:
        setConditionIcon(QnstConditionIcon(":/icon/onresume"));
        break;

    case Qnst::onPause:
        setConditionIcon(QnstConditionIcon(":/icon/onpause"));
        break;

    case Qnst::onSelection:
        setConditionIcon(QnstConditionIcon(":/icon/onselection"));
        break;

    case Qnst::NoConditionType:
        setConditionIcon(QnstConditionIcon(":/icon/nocondition"));
        break;
    }
}

QnstConditionIcon QnstGraphicsLink::getConditionIcon()
{
    return conditionIcon;
}

void QnstGraphicsLink::setConditionIcon(QnstConditionIcon conditionIcon)
{
    this->conditionIcon = conditionIcon;
}

void QnstGraphicsLink::draw(QPainter* painter)
{
    if (getEntityA() != NULL && getEntityB() != NULL){
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

        QLineF line = QLineF(QPointF(getEntityA()->getLeft() + getEntityA()->getWidth()/2,
                                     getEntityA()->getTop() + getEntityA()->getHeight()/2),
                             QPointF(getEntityB()->getLeft() + getEntityB()->getWidth()/2,
                                     getEntityB()->getTop() + getEntityB()->getHeight()/2));

        if (getEntityA()->getncgType() == Qncg::Interface){
            line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
        }

        if (getEntityB()->getncgType() == Qncg::Interface){
            line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
        }

        QPointF pointa = line.p1();
        QPointF pointb = line.p2();

        if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+12,4+12, 4+12+getWidth()-24, 4+12+getHeight()-24);

            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4,4,24,24, QPixmap(conditionIcon));
            painter->drawPixmap(4+getWidth()-24, 4+getHeight()-24, 24, 24, QPixmap(actionIcon));

        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+12+getWidth()-24,4+12, 4+12, 4+12+getHeight()-24);

            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4+getWidth()-24,4,24,24, QPixmap(conditionIcon));
            painter->drawPixmap(4, 4+getHeight()-24, 24, 24, QPixmap(actionIcon));

        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+12, 4+12+getHeight()-24, 4+12+getWidth()-24, 4+12);

            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4, 4+getHeight()-24, 24, 24, QPixmap(conditionIcon));
            painter->drawPixmap(4+getWidth()-24,4,24,24, QPixmap(actionIcon));

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+12+getWidth()-24, 4+12+getHeight()-24, 4+12, 4+12);

            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4+getWidth()-24, 4+getHeight()-24, 24, 24, QPixmap(conditionIcon));
            painter->drawPixmap(4,4,24,24, QPixmap(actionIcon));
        }
    }
}

void QnstGraphicsLink::delineate(QPainterPath* painter) const
{
    if (getEntityA() != NULL && getEntityB() != NULL){
        QLineF line = QLineF(QPointF(getEntityA()->getLeft() + getEntityA()->getWidth()/2,
                                     getEntityA()->getTop() + getEntityA()->getHeight()/2),
                             QPointF(getEntityB()->getLeft() + getEntityB()->getWidth()/2,
                                     getEntityB()->getTop() + getEntityB()->getHeight()/2));

        if (getEntityA()->getncgType() == Qncg::Interface){
            line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
        }

        if (getEntityB()->getncgType() == Qncg::Interface){
            line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
        }

        QPointF pointa = line.p1();
        QPointF pointb = line.p2();

        if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
            if (isEntityAEnabled()){
                painter->addEllipse(4,4,24,24);
            }

            if (isEntityBEnabled()){
                painter->addEllipse(4+getWidth()-24, 4+getHeight()-24, 24, 24);
            }

        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
            if (isEntityAEnabled()){
                painter->addEllipse(4+getWidth()-24,4,24,24);
            }

            if (isEntityBEnabled()){
                painter->addEllipse(4, 4+getHeight()-24, 24, 24);
            }

        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
            if (isEntityAEnabled()){
                painter->addEllipse(4, 4+getHeight()-24, 24, 24);
            }

            if (isEntityBEnabled()){
                painter->addEllipse(4+getWidth()-24,4,24,24);
            }

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
            if (isEntityAEnabled()){
                painter->addEllipse(4+getWidth()-24, 4+getHeight()-24, 24, 24);
            }

            if (isEntityBEnabled()){
                painter->addEllipse(4,4,24,24);
            }
        }
    }
}

