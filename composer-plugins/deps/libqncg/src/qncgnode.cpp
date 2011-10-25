/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>. 
 */
#include "qncgnode.h"

QncgNode::QncgNode(QncgEntity* parent)
    : QncgEntity(parent)
{
    setEntityType(Qncg::Node);

}

QncgNode::~QncgNode()
{

}

QVector<QncgEdge*> QncgNode::getBeginningEdges() const
{
    return bedges;
}

void QncgNode::removeBeginningEdge(QncgEdge* edge)
{
    int index = bedges.indexOf(edge);

    if (index >= 0){
        bedges.remove(index);
    }
}

QVector<QncgEdge*> QncgNode::getEndingEdges() const
{
    return eedges;
}

void QncgNode::removeEndingEdge(QncgEdge* edge)
{
    int index = eedges.indexOf(edge);

    if (index >= 0){
        eedges.remove(index);
    }
}

void QncgNode::adjust()
{
    scene()->update();

    if (parentItem() != NULL){
        QncgNode* pn = (QncgNode*) parentItem();



        QPointF ppbegin(getLeft()+getWidth()/2,
                        getTop()+getHeight()/2);

        QPointF ppend(pn->getWidth()/2,
                      pn->getHeight()/2);

//        qDebug() << getLeft() << "x" << getTop();
//        qDebug() << getWidth() << "x" << getHeight();
//        qDebug() << ppbegin << "x" << ppend;

        QLineF line(ppbegin, ppend);

        qreal vb = 0.0;

        QPointF nextp;

        int c = 0;

        qreal lasttop = getTop();
        qreal lastleft = getLeft();

        bool s = false;

        if (isSelected()){
            s = true;

            setSelected(false);
        }

        while(!collidesWithItem(parentItem(),Qt::ContainsItemShape)){
            vb += 0.01;

            nextp = line.pointAt(vb);

            setLeft(getLeft()+nextp.x()-ppbegin.x());

            setTop(getTop()+nextp.y()-ppbegin.y());

            scene()->update();

            c++;

            if (c > 120){
                break;

                c = -1;
            }

        }

        if (c < 0){
            setTop(lasttop);
            setLeft(lastleft);
        }

        if (s){
            setSelected(true);
        }

    }

    foreach (QncgEdge* edge, bedges){
        edge->adjust();
    }

    foreach (QncgEdge* edge, eedges){
        edge->adjust();
    }

    scene()->update();
}

void QncgNode::validate()
{

}

void QncgNode::link(QGraphicsSceneMouseEvent* event)
{

}

void QncgNode::move(QGraphicsSceneMouseEvent* event)
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

void QncgNode::addBeginningEdge(QncgEdge* edge)
{
    bedges.append(edge);
}

void QncgNode::addEndingEdge(QncgEdge* edge)
{
    eedges.append(edge);
}

void QncgNode::resize(QGraphicsSceneMouseEvent* event)
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
