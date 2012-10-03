#include "qnlygraphicsgrid.h"

QnlyGraphicsGrid::QnlyGraphicsGrid(QGraphicsItem* parent, QGraphicsScene* scene)
    : QGraphicsRectItem(parent, scene)
{

}

QnlyGraphicsGrid::~QnlyGraphicsGrid()
{

}

void QnlyGraphicsGrid::setStep(qreal step)
{
    this->step = step;
}

qreal QnlyGraphicsGrid::getStep()
{
    return step;
}

void QnlyGraphicsGrid::setPen(QPen pen)
{
    this->pen = pen;
}

QPen QnlyGraphicsGrid::getPen()
{
    return pen;
}

void QnlyGraphicsGrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen);

    QRectF r = rect();

    int w = 0;

    while (w < r.width()){
        painter->drawLine(w,0,w,r.height());
        w += step;
    }

    int h = 0;

    while (h < r.height()){
        painter->drawLine(0,h,r.width(),h);
        h  += step;
    }
}
