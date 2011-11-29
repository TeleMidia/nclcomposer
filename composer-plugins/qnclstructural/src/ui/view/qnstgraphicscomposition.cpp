#include "qnstgraphicscomposition.h"

QnstGraphicsComposition::QnstGraphicsComposition(QnstGraphicsNode* parent)
    : QnstGraphicsNode(parent)
{
    setnstType(Qnst::Compostion);

    setColor("#EEEEEE");


}

QnstGraphicsComposition::~QnstGraphicsComposition()
{

}

QString QnstGraphicsComposition::getColor() const
{
    return color;
}

void QnstGraphicsComposition::setColor(QString color)
{
    this->color = color;
}

void QnstGraphicsComposition::draw(QPainter* painter)
{
    painter->setRenderHint(QPainter::Antialiasing,true);

    painter->setBrush(QColor(color));

    painter->drawEllipse(4, 4, getWidth()-1, getHeight()-1);

    if (isMoving()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawEllipse(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(), getWidth()-1, getHeight()-1);

    }else if (isResizing()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawEllipse(getResizeLeft()+4-getLeft(), getResizeTop()+4-getTop(), getResizeWidth()-1, getResizeHeight()-1);
    }
}

void QnstGraphicsComposition::delineate(QPainterPath* painter) const
{
    painter->addEllipse(4, 4, getWidth(), getHeight());
}
