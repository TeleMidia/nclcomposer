#include "qnstgraphicsaction.h"

QnstGraphicsAction::QnstGraphicsAction(QnstGraphicsEntity* parent)
    : QnstGraphicsEdge(parent)
{
    setnstType(Qnst::Action);

    setAction(Qnst::NoActionType);
}

QnstGraphicsAction::~QnstGraphicsAction()
{

}

QnstAction QnstGraphicsAction::getAction()
{
    return action;
}

void QnstGraphicsAction::setAction(QnstAction action)
{
    this->action = action;

    switch(action){
    case Qnst::Start:
        setActionIcon(QnstActionIcon(":/icon/start"));
        break;

    case Qnst::Stop:
        setActionIcon(QnstActionIcon(":/icon/stop"));
        break;

    case Qnst::Resume:
        setActionIcon(QnstActionIcon(":/icon/resume"));
        break;

    case Qnst::Pause:
        setActionIcon(QnstActionIcon(":/icon/pause"));
        break;

    case Qnst::Set:
        setActionIcon(QnstActionIcon(":/icon/set"));
        break;

    case Qnst::NoActionType:
        setActionIcon(QnstActionIcon(":/icon/noaction"));
        break;
    }
}

QnstActionIcon QnstGraphicsAction::getActionIcon()
{
    return actionIcon;
}

void QnstGraphicsAction::setActionIcon(QnstActionIcon actionIcon)
{
    this->actionIcon = actionIcon;
}


void QnstGraphicsAction::draw(QPainter* painter)
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

            painter->drawLine(4+6,4+6, 4+12+getWidth()-24, 4+12+getHeight()-24);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawEllipse(4,4,12,12);
            painter->drawPixmap(4+getWidth()-24, 4+getHeight()-24, 24, 24, QPixmap(actionIcon));

        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+6+getWidth()-12,4+6, 4+12, 4+12+getHeight()-24);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawEllipse(4+getWidth()-12,4,12,12);
            painter->drawPixmap(4, 4+getHeight()-24, 24, 24, QPixmap(actionIcon));


        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+6, 4+6+getHeight()-12, 4+12+getWidth()-24, 4+12);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawEllipse(4, 4+getHeight()-12, 12, 12);
            painter->drawPixmap(4+getWidth()-24,4,24,24, QPixmap(actionIcon));

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+6+getWidth()-12, 4+6+getHeight()-12, 4+12, 4+12);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawEllipse(4+getWidth()-12, 4+getHeight()-12, 12, 12);
            painter->drawPixmap(4,4,24,24, QPixmap(actionIcon));
        }
    }
}

void QnstGraphicsAction::delineate(QPainterPath* painter) const
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
    //        painter->drawLine(4+6,4+6, 4+6+getWidth()-12-2, 4+6+getHeight()-12-2);

            painter->addEllipse(4,4,12,12);
            painter->addEllipse(4+getWidth()-24, 4+getHeight()-24, 24, 24);

        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
    //        painter->drawLine(4+6+getWidth()-12,4+6, 4+6, 4+6+getHeight()-12);

            painter->addEllipse(4+getWidth()-12,4,12,12);
            painter->addEllipse(4, 4+getHeight()-24, 24, 24);

        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
    //        painter->drawLine(4+6, 4+6+getHeight()-12, 4+6+getWidth()-12, 4+6);

            painter->addEllipse(4, 4+getHeight()-12, 12, 12);
            painter->addEllipse(4+getWidth()-24,4,24,24);

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
    //        painter->drawLine(4+6+getWidth()-12, 4+6+getHeight()-12, 4+6, 4+6);

            painter->addEllipse(4+getWidth()-12, 4+getHeight()-12, 12, 12);
            painter->addEllipse(4,4,24,24);
        }
    }
}
