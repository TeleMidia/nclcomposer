#include "qnstgraphicscondition.h"

QnstGraphicsCondition::QnstGraphicsCondition(QnstGraphicsEntity* parent)
    : QnstGraphicsEdge(parent)
{
    setnstType(Qnst::Condition);

    setCondition(Qnst::NoConditionType);
}

QnstGraphicsCondition::~QnstGraphicsCondition()
{

}

QnstCondition QnstGraphicsCondition::getCondition()
{
    return condition;
}

void QnstGraphicsCondition::setCondition(QnstCondition condition)
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

QnstConditionIcon QnstGraphicsCondition::getConditionIcon()
{
    return conditionIcon;
}

void QnstGraphicsCondition::setConditionIcon(QnstConditionIcon conditionIcon)
{
    this->conditionIcon = conditionIcon;
}

void QnstGraphicsCondition::adjust()
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
            setTop(pointa.y()-6);
            setLeft(pointa.x()-6);
            setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
            setHeight((pointb.y()-6)-(pointa.y()-6) + 12);

        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
            setTop(pointa.y()-6);
            setLeft(pointb.x()-6);
            setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
            setHeight((pointb.y()-6)-(pointa.y()-6) + 12);

        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
            setTop(pointb.y()-6);
            setLeft((pointa.x()-6));
            setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
            setHeight((pointa.y()-6)-(pointb.y()-6) + 12);

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
            setTop(pointb.y()-6);
            setLeft(pointb.x()-6);
            setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
            setHeight((pointa.y()-6)-(pointb.y()-6) + 12);
        }

        getEntityA()->setSelectable(false);

        qreal index;

        if (pointa != pointb){

            index = 0;

            int n = 0;

            while(getEntityA()->collidesWithItem(this)){
                index += 0.01;

                pointa = line.pointAt(index);

                if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
                    setTop(pointa.y()-6);
                    setLeft(pointa.x()-6);
                    setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
                    setHeight((pointb.y()-6)-(pointa.y()-6) + 12);

                }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
                    setTop(pointa.y()-6);
                    setLeft(pointb.x()-6);
                    setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
                    setHeight((pointb.y()-6)-(pointa.y()-6) + 12);

                }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
                    setTop(pointb.y()-6);
                    setLeft((pointa.x()-6));
                    setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
                    setHeight((pointa.y()-6)-(pointb.y()-6) + 12);

                }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
                    setTop(pointb.y()-6);
                    setLeft(pointb.x()-6);
                    setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
                    setHeight((pointa.y()-6)-(pointb.y()-6) + 12);
                }

                if (++n > 100){ // avoiding infinity loop
                    break;
                }
            }
        }

        getEntityA()->setSelectable(true);

        if (scene() != NULL){
            scene()->update();
        }
    }
}

void QnstGraphicsCondition::draw(QPainter* painter)
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

            painter->drawLine(4+12,4+12, 4+6+getWidth()-12, 4+6+getHeight()-12);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4,4,24,24, QPixmap(conditionIcon));


        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+12+getWidth()-24,4+6, 4+6, 4+6+getHeight()-12);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4+getWidth()-24,4,24,24, QPixmap(conditionIcon));

        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+12, 4+12+getHeight()-24, 4+6+getWidth()-12, 4+6);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4, 4+getHeight()-24, 24, 24, QPixmap(conditionIcon));

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+12+getWidth()-24, 4+12+getHeight()-24, 4+6, 4+6);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4+getWidth()-24, 4+getHeight()-24, 24, 24, QPixmap(conditionIcon));
        }
    }
}

void QnstGraphicsCondition::delineate(QPainterPath* painter) const
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

            painter->addEllipse(4,4,24,24);

        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
    //        painter->drawLine(4+6+getWidth()-12,4+6, 4+6, 4+6+getHeight()-12);

            painter->addEllipse(4+getWidth()-24,4,24,24);


        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
    //        painter->drawLine(4+6, 4+6+getHeight()-12, 4+6+getWidth()-12, 4+6);

            painter->addEllipse(4, 4+getHeight()-24, 24, 24);

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
    //        painter->drawLine(4+6+getWidth()-12, 4+6+getHeight()-12, 4+6, 4+6);

            painter->addEllipse(4+getWidth()-24, 4+getHeight()-24, 24, 24);
        }
    }
}
