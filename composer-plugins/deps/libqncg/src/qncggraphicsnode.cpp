#include "qncggraphicsnode.h"

QncgGraphicsNode::QncgGraphicsNode(QncgGraphicsEntity* parent)
    : QncgGraphicsEntity(parent)
{
    setncgType(Qncg::Node);
}

QncgGraphicsNode::~QncgGraphicsNode()
{

}

void QncgGraphicsNode::adjust()
{
    if (scene() != NULL){
        scene()->update();
    }
}

void QncgGraphicsNode::inside()
{
    QncgGraphicsEntity* parent = getncgGraphicsParent();

    if (parent != NULL){
        QPointF pa(getLeft()+getWidth()/2, getTop()+getHeight()/2);
        QPointF pb(parent->getWidth()/2, parent->getHeight()/2);

        QLineF line(pa, pb);

        qreal n = 0;

        qreal i = 0.0;

        qreal top = getTop();
        qreal left = getLeft();

        setSelectable(false); update();

        while(!collidesWithItem(parent,Qt::ContainsItemShape)){
            i += 0.1;

            setTop(getTop()+line.pointAt(i).y()-pa.y());
            setLeft(getLeft()+line.pointAt(i).x()-pa.x());

            if (++n > 100){
                n = -1; break;
            }
        }

        setSelectable(true); update();

        if (n < 0){
            setTop(top);
            setLeft(left);
        }
    }
}

void QncgGraphicsNode::outside()
{
    QncgGraphicsEntity* parent = getncgGraphicsParent();

    if (parent != NULL){
        QPointF pa(getLeft()+getWidth()/2, getTop()+getHeight()/2);
        QPointF pb(parent->getWidth()/2, parent->getHeight()/2);

        QLineF line(pa, pb);

        qreal n = 0;

        qreal i = 0.0;

        qreal top = getTop();
        qreal left = getLeft();

        setSelectable(false); update();

        while(collidesWithItem(parent,Qt::IntersectsItemShape)){
            i -= 0.1;

            setTop(getTop()+line.pointAt(i).y()-pa.y());
            setLeft(getLeft()+line.pointAt(i).x()-pa.x());

            if (++n > 100){
                n = -1; break;
            }
        }

        setSelectable(true); update();

        if (n < 0){
            setTop(top);
            setLeft(left);
        }
    }
}

void QncgGraphicsNode::attract()
{
    foreach(QncgGraphicsEntity* entity, getncgGraphicsEntities()){
        if (entity->getncgType() == Qncg::Node){
            ((QncgGraphicsNode*) entity)->inside();
        }
    }
}

void QncgGraphicsNode::repel()
{
    foreach(QncgGraphicsEntity* entity, getncgGraphicsEntities()){
        if (entity->getncgType() == Qncg::Node){
            ((QncgGraphicsNode*) entity)->outside();
        }
    }
}

void QncgGraphicsNode::fit(qreal padding)
{
    qreal top = getTop();
    qreal left = getLeft();
    qreal width = getWidth();
    qreal height = getHeight();

    qreal nextTop = top;
    qreal nextLeft = left;
    qreal nextWidth = width;
    qreal nextHeight = height;

    foreach(QncgGraphicsEntity* entity, getncgGraphicsEntities()){
        if ((entity->getTop()-4) + top < nextTop + padding){
            nextTop = (entity->getTop()-4) + top - padding;
        }

        if ((entity->getLeft()-4) + left < nextLeft + padding){
            nextLeft = (entity->getLeft()-4) + left - padding;
        }

        if ((entity->getLeft()-4) + entity->getWidth() > nextWidth - padding){
            nextWidth = (entity->getLeft()-4) + entity->getWidth() + padding;
        }

        if ((entity->getTop()-4) + entity->getHeight() > nextHeight - padding){
            nextHeight = (entity->getTop()-4) + entity->getHeight() + padding;
        }
    }

    if (nextWidth != width){
        setWidth(nextWidth);
    }

    if (nextHeight != height){
        setHeight(nextHeight);
    }

    if (nextTop != top){
        foreach(QncgGraphicsEntity* entity, getncgGraphicsEntities()){
            entity->setTop(entity->getTop() + (top - nextTop));
        }

        setHeight(getHeight() + (top - nextTop));
        setTop(nextTop);
    }

    if (nextLeft != left){
        foreach(QncgGraphicsEntity* entity, getncgGraphicsEntities()){
            entity->setLeft(entity->getLeft() + (left - nextLeft));
        }

        setWidth(getWidth() + (left - nextLeft));
        setLeft(nextLeft);
    }

    QncgGraphicsNode* parent = (QncgGraphicsNode*) getncgGraphicsParent();

    if (parent != NULL){
        parent->fit(padding);
    }
}

void QncgGraphicsNode::compact(qreal padding)
{
    qreal top = getTop();
    qreal left = getLeft();
    qreal width = getWidth();
    qreal height = getHeight();

    qreal nextTop = height;
    qreal nextLeft = width;
    qreal nextWidth = 0;
    qreal nextHeight = 0;

    foreach(QncgGraphicsEntity* entity, getncgGraphicsEntities()){
        if ((entity->getTop()-4) < nextTop){
            nextTop = (entity->getTop()-4);
        }

        if ((entity->getLeft()-4) < nextLeft){
            nextLeft = (entity->getLeft()-4);
        }

        if ((entity->getLeft()-4) + entity->getWidth() > nextWidth){
            nextWidth = (entity->getLeft()-4) + entity->getWidth();
        }

        if ((entity->getTop()-4) + entity->getHeight() > nextHeight){
            nextHeight = (entity->getTop()-4) + entity->getHeight();
        }
    }

    if (nextWidth != 0){
        setWidth(nextWidth + padding);
    }

    if (nextHeight != 0){
        setHeight(nextHeight + padding);
    }

    if (nextTop != height){
        foreach(QncgGraphicsEntity* entity, getncgGraphicsEntities()){
            entity->setTop(entity->getTop() - (nextTop - padding));
        }

        setHeight(getHeight() - (nextTop - padding));
        setTop(top + nextTop - padding);
    }

    if (nextLeft != width){
        foreach(QncgGraphicsEntity* entity, getncgGraphicsEntities()){
            entity->setLeft(entity->getLeft() - (nextLeft - padding));
        }

        setWidth(getWidth() - (nextLeft - padding));
        setLeft(left + nextLeft - padding);
    }
}

void QncgGraphicsNode::clock(qreal padding)
{
    QVector<QncgGraphicsEntity*> entities = getncgGraphicsEntities();




    if (entities.size() == 1){
        QncgGraphicsEntity* entity = entities.at(0);

        entity->setTop(getHeight()/2 - entity->getHeight()/2);
        entity->setLeft(getWidth()/2 - entity->getWidth()/2);

    }else if (entities.size() > 1){
        for (int i = 0; i < entities.size(); i++){

            QncgGraphicsEntity* entity = entities.at(i);

            qreal length = 0;

            if (entity->getWidth() > length){
                length = entity->getWidth();
            }

            if (entity->getHeight() > length){
                length = entity->getHeight();
            }

            qreal A = 360/entities.size();

            qreal R = (length+padding) / (2*sin((A/2)*(PI/180)));

            entity->setTop(getHeight()/2 - entity->getHeight()/2);
            entity->setLeft(getWidth()/2 - entity->getWidth()/2 - R);

            QPointF pa = QPointF(getWidth()/2, getHeight()/2);
            QPointF pb = QPointF(entity->getLeft() + entity->getWidth()/2, entity->getTop() + entity->getHeight()/2);

            QLineF line(pa, pb);

            line.setAngle(line.angle() + i*A);

            entity->setTop(line.p2().y() - entity->getHeight()/2);
            entity->setLeft(line.p2().x()  - entity->getWidth()/2);
        }
    }
}

void QncgGraphicsNode::move(QGraphicsSceneMouseEvent* event)
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

void QncgGraphicsNode::resize(QGraphicsSceneMouseEvent* event)
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