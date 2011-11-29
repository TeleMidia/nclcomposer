#include "qncggraphicsinterface.h"

QncgGraphicsInterface::QncgGraphicsInterface(QncgGraphicsEntity* parent)
    : QncgGraphicsEntity(parent)
{
    setncgType(Qncg::Interface);
}

QncgGraphicsInterface::~QncgGraphicsInterface()
{

}

void QncgGraphicsInterface::adjust()
{
    QncgGraphicsEntity* parent = getncgGraphicsParent();

    QPointF pa(parent->getWidth()/2, parent->getHeight()/2);
    QPointF pb(getLeft() + getWidth()/2, getTop() + getHeight()/2);

    QLineF line(pa,pb);

    if (parent->contains(pb)){
        qreal i = 1.0;

        QPointF pn = pb;

        // attention! infinity loop condition.
        while(parent->contains(pn)){
            i += 0.01;

            pn = line.pointAt(i);
        }

        setTop(pn.y() - getHeight()/2);
        setLeft(pn.x() - getWidth()/2);

    }else{

        qreal i = 1.0;

        QPointF pn = pb;

        // attention! infinity loop condition.
        while(!parent->contains(pn)){
            i -= 0.01;

            pn = line.pointAt(i);
        }

        setTop(pn.y() - getHeight()/2);
        setLeft(pn.x() - getWidth()/2);
    }

    scene()->update();
}

void QncgGraphicsInterface::move(QGraphicsSceneMouseEvent* event)
{
    // setting
    qreal x = getLeft();
    qreal y = getTop();

    // setting minimal position
    qreal minx;
    qreal miny;

    if (parentItem() != NULL){
        minx = 4;
        miny = 4;
    }else{
        minx = 0;
        miny = 0;
    }

    // setting maximal position
    qreal maxx;
    qreal maxy;

    if (parentItem() != NULL){
        maxx = parentItem()->boundingRect().width() - getWidth() - 4;
        maxy = parentItem()->boundingRect().height() - getHeight() - 4;
    }else{
        maxx = scene()->width() - getWidth();
        maxy = scene()->height() - getHeight();
    }

    // setting delta
    qreal dx = event->pos().x() - getPressLeft(); // (x1 - x0)
    qreal dy = event->pos().y() - getPressTop();  // (y1 - y0)

    // setting next position
    qreal nextx = x + dx;
    qreal nexty = y + dy;

    // adjusting
//    if (nextx < minx){
//        nextx = minx;
//    }

//    if (nexty < miny){
//        nexty = miny;h
//    }

//    if (nextx > maxx){
//        nextx = maxx;
//    }

//    if (nexty > maxy){
//        nexty = maxy;
//    }

    // moving
    setMoveTop(nexty);
    setMoveLeft(nextx);

    scene()->update();
}

void QncgGraphicsInterface::resize(QGraphicsSceneMouseEvent* event)
{
    // setting bounds
    qreal x = getLeft();
    qreal y = getTop();
    qreal w = getWidth();
    qreal h = getHeight();

    // setting minimal bounds
    qreal minx;
    qreal miny;
    qreal minw;
    qreal minh;

    if (parentItem() != NULL){
        minx = 4;
        miny = 4;
        minw = -1; // not used
        minh = -1; // not used
    }else{
        minx = 0;
        miny = 0;
        minw = -1; // not used
        minh = -1; // not used
    }

    // setting maximal bounds
    qreal maxx;
    qreal maxy;
    qreal maxw;
    qreal maxh;

    if (parentItem() != NULL){
        maxx = parentItem()->boundingRect().width() - getWidth() - 4;
        maxy = parentItem()->boundingRect().height() - getHeight() - 4;
        maxw = parentItem()->boundingRect().width() - 4;
        maxh = parentItem()->boundingRect().height() - 4;
    }else{
        maxx = scene()->width() - getWidth();
        maxy = scene()->height() - getHeight();
        maxw = scene()->width();
        maxh = scene()->height();
    }

    // setting delta
    qreal dx = event->pos().x() - getPressLeft();    // (x1 - x0)
    qreal dy = event->pos().y() - getPressTop();     // (y1 - y0)
    qreal dw = -dx;
    qreal dh = -dy;

    // setting next bounds
    qreal nextx = x + dx;
    qreal nexty = y + dy;
    qreal nextw = w + dw;
    qreal nexth = h + dh;

    // adjusting
    switch(getncgResize()){

    // adjusting TOPLEFT
    case Qncg::TopLeft:{
//        if (nextx < minx){
//            nextx = minx;
//            nextw = x + w - minx;
//        }

//        if (nexty < miny){
//            nexty = miny;
//            nexth = y + h - miny;
//        }

        break;
    }

    // adjusting TOP
    case Qncg::Top:{
        nextx = x; // fixed x
        nextw = w; // fixed width

//        if (nexty < miny){
//            nexty = miny;
//            nexth = y + h - miny;
//        }

        break;
    }

    // adjusting TOPRIGHT
    case Qncg::TopRight:{
        nextx = x; // fixed x

        nextw = w - dw;
//        if (x + nextw > maxw){
//            nextw = maxw - x;
//        }

//        if (nexty < miny){
//            nexty = miny;
//            nexth = y + h - miny;
//        }

        break;
    }

    // adjusting RIGHT
    case Qncg::Right:{
        nextx = x; // fixed x

        nextw = w - dw;
//        if (x + nextw > maxw){
//            nextw = maxw - x;
//        }

        nexty = y; // fixed y
        nexth = h; // fixed height

        break;
    }

    // adjusting BOTTOMRIGHT
    case Qncg::BottomRight:{
        nextx = x; // fixed x

        nextw = w - dw;
//        if (x + nextw > maxw){
//            nextw = maxw - x;
//        }

        nexty = y; // fixed y

        nexth = h - dh;
//        if (y + nexth > maxh){
//            nexth = maxh - y;
//        }

        break;
    }

    // adjusting BOTTOM
    case Qncg::Bottom:{
        nextx = x; // fixed x
        nextw = w; // fixed width

        nexty = y; // fixed y

        nexth = h - dh;
//        if (y + nexth > maxh){
//            nexth = maxh - y;
//        }

        break;
    }

    // adjusting BOTTOMLEFT
    case Qncg::BottomLeft:{
//        if (nextx < minx){
//            nextx = minx;
//            nextw = x + w - minx;
//        }

        nexty = y; // fixed y

        nexth = h - dh;
//        if (y + nexth > maxh){
//            nexth = maxh - y;
//        }

        break;
    }

    // adjusting LEFT
    case Qncg::Left:{
//        if (nextx < minx){
//            nextx = minx;
//            nextw = x + w - minx;
//        }

        nexty = y; // fixed y
        nexth = h; // fixed height

        break;
    }
    }

    // resizing
    setResizeTop(nexty);
    setResizeLeft(nextx);
    setResizeWidth(nextw);
    setResizeHeight(nexth);

    scene()->update();
}
