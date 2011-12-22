#include "qnstgraphicsport.h"

QnstGraphicsPort::QnstGraphicsPort(QnstGraphicsEntity* parent)
    : QnstGraphicsInterface(parent)
{
    setnstType(Qnst::Port);
}

QnstGraphicsPort::~QnstGraphicsPort()
{

}

void QnstGraphicsPort::draw(QPainter* painter)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter->drawPixmap(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8, QPixmap(":/icon/port"));

    if (isMoving()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawRect(getMoveLeft()+4-getLeft(),getMoveTop()+4-getTop(),getWidth()-1,getHeight()-1);
    }
}

void QnstGraphicsPort::delineate(QPainterPath* painter) const
{
    painter->addRect(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-8);
}
