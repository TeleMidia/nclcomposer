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
            setTop(pointa.y()-4);
            setLeft(pointa.x()-4);
            setWidth((pointb.x()-4)-(pointa.x()-4) + 8);
            setHeight((pointb.y()-4)-(pointa.y()-4) + 8);

        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
            setTop(pointa.y()-4);
            setLeft(pointb.x()-4);
            setWidth((pointa.x()-4)-(pointb.x()-4) + 8);
            setHeight((pointb.y()-4)-(pointa.y()-4) + 8);

        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
            setTop(pointb.y()-4);
            setLeft((pointa.x()-4));
            setWidth((pointb.x()-4)-(pointa.x()-4) + 8);
            setHeight((pointa.y()-4)-(pointb.y()-4) + 8);

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
            setTop(pointb.y()-4);
            setLeft(pointb.x()-4);
            setWidth((pointa.x()-4)-(pointb.x()-4) + 8);
            setHeight((pointa.y()-4)-(pointb.y()-4) + 8);
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
                    setTop(pointa.y()-4);
                    setLeft(pointa.x()-4);
                    setWidth((pointb.x()-4)-(pointa.x()-4) + 8);
                    setHeight((pointb.y()-4)-(pointa.y()-4) + 8);

                }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
                    setTop(pointa.y()-4);
                    setLeft(pointb.x()-4);
                    setWidth((pointa.x()-4)-(pointb.x()-4) + 8);
                    setHeight((pointb.y()-4)-(pointa.y()-4) + 8);

                }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
                    setTop(pointb.y()-4);
                    setLeft((pointa.x()-4));
                    setWidth((pointb.x()-4)-(pointa.x()-4) + 8);
                    setHeight((pointa.y()-4)-(pointb.y()-4) + 8);

                }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
                    setTop(pointb.y()-4);
                    setLeft(pointb.x()-4);
                    setWidth((pointa.x()-4)-(pointb.x()-4) + 8);
                    setHeight((pointa.y()-4)-(pointb.y()-4) + 8);
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

            painter->drawLine(4+8,4+8, 4+4+getWidth()-8, 4+4+getHeight()-8);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4,4,16,16, QPixmap(conditionIcon));


        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+8+getWidth()-16,4+4, 4+4, 4+4+getHeight()-8);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4+getWidth()-16,4,16,16, QPixmap(conditionIcon));

        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+8, 4+8+getHeight()-16, 4+4+getWidth()-8, 4+4);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4, 4+getHeight()-16, 16, 16, QPixmap(conditionIcon));

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));

            painter->drawLine(4+8+getWidth()-16, 4+8+getHeight()-16, 4+4, 4+4);

            painter->setBrush(QBrush(QColor("#000000")));
            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4+getWidth()-16, 4+getHeight()-16, 16, 16, QPixmap(conditionIcon));
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
    //        painter->drawLine(4+4,4+4, 4+4+getWidth()-8-2, 4+4+getHeight()-8-2);

            painter->addEllipse(4,4,16,16);

        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
    //        painter->drawLine(4+4+getWidth()-8,4+4, 4+4, 4+4+getHeight()-8);

            painter->addEllipse(4+getWidth()-16,4,16,16);


        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
    //        painter->drawLine(4+4, 4+4+getHeight()-8, 4+4+getWidth()-8, 4+4);

            painter->addEllipse(4, 4+getHeight()-16, 16, 16);

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
    //        painter->drawLine(4+4+getWidth()-8, 4+4+getHeight()-8, 4+4, 4+4);

            painter->addEllipse(4+getWidth()-16, 4+getHeight()-16, 16, 16);
        }
    }
}
