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

void QnstGraphicsComposition::setnstId(QString id)
{
    QnstGraphicsNode::setnstId(id);

    QString tip = "";
    QString name = (getnstId() != "" ? getnstId() : "?");

    if (getnstType() == Qnst::Context){
        tip += "Context ("+name+")";

    }else if (getnstType() == Qnst::Switch){
        tip += "Switch ("+name+")";

    }else if (getnstType() == Qnst::Body){
        tip += "Body ("+name+")";

    }else{
        tip += "Composition ("+name+")";
    }

    setToolTip(tip);
}

void QnstGraphicsComposition::draw(QPainter* painter)
{
//    if (!isSelected() && hasMouseHover() && getnstType() != Qnst::Body){
//        painter->setBrush(Qt::NoBrush);
//        painter->setPen(QPen(QBrush(QColor("#999999")), 0, Qt::DashLine)); // 0px = cosmetic border

//        painter->drawRect(4, 4, getWidth(), getHeight());
//    }

    painter->setRenderHint(QPainter::Antialiasing,true);

    painter->setBrush(QColor(color));
    painter->setPen(QPen(QBrush(Qt::black), 0));

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
