#include "interface.h"

Interface::Interface(QncgGraphicsEntity* parent)
    : QncgGraphicsInterface(parent)
{
    connect(this, SIGNAL(entityChanged()), SLOT(requestEntityChange()));
    connect(this, SIGNAL(entitySelected()), SLOT(requestEntitySelection()));
}

Interface::~Interface()
{

}

void Interface::requestEntityChange()
{
    emit entityChanged(this);
}

void Interface::requestEntitySelection()
{
    emit entitySelected(this);
}

void Interface::draw(QPainter* painter)
{
//    painter->setRenderHint(QPainter::Antialiasing,true);

    painter->setBrush(QBrush(QColor("#000000")));
    painter->drawRect(4+20/2, 4+20/2, getWidth()-1-20, getHeight()-1-20);

//    painter->setRenderHint(QPainter::Antialiasing,false);

    if (isMoving()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawRect(getMoveLeft()+4-getLeft(),getMoveTop()+4-getTop(),getWidth()-1,getHeight()-1);

    }else if (isResizing()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawRect(getResizeLeft()+4-getLeft(),getResizeTop()+4-getTop(),getResizeWidth()-1,getResizeHeight()-1);
    }
}

void Interface::delineate(QPainterPath* painter) const
{
    painter->addRect(4+20/2, 4+20/2, getWidth()-20, getHeight()-20);
}
