#include "qnstgraphicsproperty.h"

QnstGraphicsProperty::QnstGraphicsProperty(QnstGraphicsEntity* parent)
    : QnstGraphicsInterface(parent)
{
    setnstType(Qnst::Property);
}

QnstGraphicsProperty::~QnstGraphicsProperty()
{

}

void QnstGraphicsProperty::draw(QPainter* painter)
{
    painter->setBrush(QBrush(QColor("#999999")));

    painter->drawRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);

    if (isMoving()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawRect(getMoveLeft()+4-getLeft(),getMoveTop()+4-getTop(),getWidth()-1,getHeight()-1);
    }
}

void QnstGraphicsProperty::delineate(QPainterPath* painter) const
{
    painter->addRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);
}
