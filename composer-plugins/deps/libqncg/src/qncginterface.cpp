#include "qncginterface.h"

#include <QGraphicsLineItem>

QncgInterface::QncgInterface(QncgEntity* parent)
    : QncgEntity(parent)
{
    setEntityType(Qncg::Interface);
}

QncgInterface::~QncgInterface()
{

}

void QncgInterface::addBeginningEdge(QncgEdge* edge)
{
    bedges.append(edge);
}

void QncgInterface::addEndingEdge(QncgEdge* edge)
{
    eedges.append(edge);
}


void QncgInterface::adjust()
{
    QGraphicsItem* p = parentItem();

    QPointF mpp(p->boundingRect().width()/2, p->boundingRect().height()/2);
    QPointF mip(getLeft() + getWidth()/2, getTop() + getHeight()/2);

    QLineF ml(mpp,mip);

    if (p->contains(mip)){

        qreal at = 0.01;

        QPointF np = ml.pointAt(at);

        while(p->contains(np)){
            at = at + 0.01;

            np = ml.pointAt(at);
        }

        setTop(np.y()-getHeight()/2);
        setLeft(np.x()-getWidth()/2);

    }else{

        qreal at = 1;

        QPointF np = ml.pointAt(at);

        while(!p->contains(np)){
            at = at - 0.01;

            np = ml.pointAt(at);
        }

        setTop(np.y()-getHeight()/2);
        setLeft(np.x()-getWidth()/2);
    }

    foreach (QncgEdge* edge, bedges){
        edge->adjust();
    }

    foreach (QncgEdge* edge, eedges){
        edge->adjust();
    }
//    scene()->addItem(new QGraphicsLineItem(QLineF(p->mapToScene(mpp),p->mapToScene(mip))));

    scene()->update();
}

void QncgInterface::validate()
{

}

void QncgInterface::link(QGraphicsSceneMouseEvent* event)
{

}

void QncgInterface::move(QGraphicsSceneMouseEvent* event)
{
    /* setting */
    qreal x = getLeft();
    qreal y = getTop();

    /* setting minimal position */
    qreal minx;
    qreal miny;

    if (parentItem() != NULL){
        minx = 4;
        miny = 4;
    }else{
        minx = 0;
        miny = 0;
    }

    /* setting maximal position */
    qreal maxx;
    qreal maxy;

    if (parentItem() != NULL){
        maxx = parentItem()->boundingRect().width() - getWidth() - 4;
        maxy = parentItem()->boundingRect().height() - getHeight() - 4;
    }else{
        maxx = scene()->width() - getWidth();
        maxy = scene()->height() - getHeight();
    }

    /* setting delta */
    qreal dx = event->pos().x() - getPressLeft(); // (x1 - x0)
    qreal dy = event->pos().y() - getPressTop();  // (y1 - y0)

    /* setting next position */
    qreal nextx = x + dx;
    qreal nexty = y + dy;

    /* adjusting */
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

    /* moving */
    setMoveTop(nexty);
    setMoveLeft(nextx);

    scene()->update();
}

void QncgInterface::resize(QGraphicsSceneMouseEvent* event)
{
    /* setting bounds */
    qreal x = getLeft();
    qreal y = getTop();
    qreal w = getWidth();
    qreal h = getHeight();

    /* setting minimal bounds */
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

    /* setting maximal bounds */
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

    /* setting delta */
    qreal dx = event->pos().x() - getPressLeft();    // (x1 - x0)
    qreal dy = event->pos().y() - getPressTop();     // (y1 - y0)
    qreal dw = -dx;
    qreal dh = -dy;

    /* setting next bounds */
    qreal nextx = x + dx;
    qreal nexty = y + dy;
    qreal nextw = w + dw;
    qreal nexth = h + dh;

    /* adjusting */
    switch(getResizeType()){

    /* adjusting TOPLEFT */
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

    /* adjusting TOP */
    case Qncg::Top:{
        nextx = x; // fixed x
        nextw = w; // fixed width

//        if (nexty < miny){
//            nexty = miny;
//            nexth = y + h - miny;
//        }

        break;
    }

    /* adjusting TOPRIGHT */
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

    /* adjusting RIGHT */
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

    /* adjusting BOTTOMRIGHT */
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

    /* adjusting BOTTOM */
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

    /* adjusting BOTTOMLEFT */
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

    /* adjusting LEFT */
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

    /* resizing */
    setResizeTop(nexty);
    setResizeLeft(nextx);
    setResizeWidth(nextw);
    setResizeHeight(nexth);

    scene()->update();
}
