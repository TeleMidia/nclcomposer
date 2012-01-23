#include "qnstgraphicsarea.h"

QnstGraphicsArea::QnstGraphicsArea(QnstGraphicsEntity* parent)
    : QnstGraphicsInterface(parent)
{
    setnstType(Qnst::Area);

    setnstId("");
}

QnstGraphicsArea::~QnstGraphicsArea()
{

}

void QnstGraphicsArea::draw(QPainter* painter)
{
    painter->setBrush(QBrush(QColor("#F4A460")));

    painter->drawRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);

    if (isMoving()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawRect(getMoveLeft()+4-getLeft(),getMoveTop()+4-getTop(),getWidth()-1,getHeight()-1);
    }
}

void QnstGraphicsArea::delineate(QPainterPath* painter) const
{
    painter->addRect(4, 4, getWidth(), getHeight());
}
