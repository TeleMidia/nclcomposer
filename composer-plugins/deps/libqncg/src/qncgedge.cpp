/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "math.h"

#include "qncgedge.h"

QncgEdge::QncgEdge(QncgEntity* parent)
    : QncgEntity(parent)
{
    setEntityType(Qncg::Edge);

    setLinkable(true);

    setLinking(false);

    setbSpan(0);
    seteSpan(0);
    setAngle(0);
}

QncgEdge::~QncgEdge()
{

}

qreal QncgEdge::getAngle()
{
    return angle;
}

void QncgEdge::setAngle(qreal angle)
{
    this->angle = angle;
    this->adjustedangle = angle;
}

QPointF QncgEdge::getpBegin() const
{
    return pbegin;
}

QPointF QncgEdge::setpBegin(QPointF pbegin)
{
    this->pbegin = pbegin;
}

QPointF QncgEdge::getpEnd() const
{
    return pend;
}

QPointF QncgEdge::setpEnd(QPointF pend)
{
    this->pend = pend;
}

QncgEntity* QncgEdge::getnBegin()
{
    return nbegin;
}

QncgEntity* QncgEdge::setnBegin(QncgEntity* nbegin)
{
    this->nbegin = nbegin;
}

QncgEntity* QncgEdge::getnEnd()
{
    return nend;
}

QncgEntity* QncgEdge::setnEnd(QncgEntity* nend)
{
    this->nend = nend;
}

qreal QncgEdge::getbSpan() const
{
    return bspan;
}

qreal QncgEdge::setbSpan(qreal bspan)
{
    this->bspan = bspan;
}

qreal QncgEdge::geteSpan() const
{
    return espan;
}

qreal QncgEdge::seteSpan(qreal espan)
{
    this->espan = espan;
}

void QncgEdge::aux_adjust()
{
    if (pbegin.x() <= pend.x() && pbegin.y() <= pend.y()){

        setTop(pbegin.y()+4-bspan);
        setLeft(pbegin.x()+4-bspan);
        setWidth(pend.x()-pbegin.x()+bspan+espan);
        setHeight(pend.y()-pbegin.y()+bspan+espan);

    }else if (pbegin.x() > pend.x() && pbegin.y() < pend.y()){
        setTop(pbegin.y()+4-bspan);
        setLeft(pend.x()+4-bspan);
        setWidth(pbegin.x()-pend.x()+bspan+espan);
        setHeight(pend.y()-pbegin.y()+bspan+espan);

    }else if (pbegin.x() < pend.x() && pbegin.y() > pend.y()){
        setTop(pend.y()+4-bspan);
        setLeft(pbegin.x()+4-bspan);
        setWidth(pend.x()-pbegin.x()+bspan+espan);
        setHeight(pbegin.y()-pend.y()+bspan+espan);

    }else if (pbegin.x() > pend.x() && pbegin.y() > pend.y()){
        setTop(pend.y()+4-bspan);
        setLeft(pend.x()+4-bspan);
        setWidth(pbegin.x()-pend.x()+bspan+espan);
        setHeight(pbegin.y()-pend.y()+bspan+espan);
    }
}

qreal QncgEdge::getAdjustedAngle()
{
    return adjustedangle;
}

QPointF QncgEdge::arcPointAt(QLineF line, qreal at)
{
    qreal alfa = getAngle();

    qreal beta = (180 - alfa)/2 + (360 - line.angle());

    qreal PI = 3.14159265;

    qreal R = line.length()/(::sin(((alfa/2)*PI)/180)*2);


    QPointF center_p(line.p2().x() - ::sin((beta+alfa-90)*PI/180)*R,
                     line.p2().y() + ::sin((180-beta-alfa)*PI/180)*R);

    qreal arc_len = alfa*PI*R/180;

    qreal new_arc_len = arc_len - arc_len*at;


    qreal new_alfa = (180*new_arc_len)/(PI*R);


    qreal gama = (180-beta-alfa);


    QPointF new_start_p(center_p.x() + ::cos((new_alfa+gama)*PI/180)*R,
                        center_p.y() - ::sin((new_alfa+gama)*PI/180)*R);

    this->adjustedangle = new_alfa;

    return new_start_p;
}

void QncgEdge::adjust()
{
    setpBegin(QPointF(nbegin->getLeft()+nbegin->getWidth()/2,
                      nbegin->getTop()+nbegin->getHeight()/2));

    setpEnd(QPointF(nend->getLeft()+nend->getWidth()/2,
                    nend->getTop()+nend->getHeight()/2));

    if (nend->getEntityType() == Qncg::Interface){
        setpEnd(parentItem()->mapFromItem(nend->parentItem(),pend));
    }

    aux_adjust();

    // packing : still dangerous!!
    // when adjust the edge for start or end node, the collide function compare
    // for both begin and end point of the edge. this create a loop condition.
    // solution: when adjust for start node compare only with begin point. same
    // for the end.

    bool bs = false;
    bool es = false;

    if (getnBegin()->isSelected()){
        bs = true;

        getnBegin()->setSelected(false);
    }

    if (getnEnd()->isSelected()){
        es = true;

        getnEnd()->setSelected(false);
    }

    QLineF line(pbegin, pend);

    // end node
    qreal ve = 1.0;

    int ce = 0;

    while(getnEnd()->collidesWithItem(this)){
        ve -= 0.5;

        if (angle != 0){
            pend = arcPointAt(line, ve);
        }else{
            pend = line.pointAt(ve);
        }

        aux_adjust();

        scene()->update();

        ce++;

        qDebug() << "VE:" << ve << "CE:" << ce << "PEND:" << pend;

        if (ce > 100){
            break;
        }
    }

    qDebug() << "### END 0.5";

    while(!getnEnd()->collidesWithItem(this)){
        ve += 0.1;

        if (angle != 0){
            pend = arcPointAt(line, ve);
        }else{
            pend = line.pointAt(ve);
        }

        aux_adjust();

        scene()->update();

        ce++;

        qDebug() << "VE:" << ve << "CE:" << ce << "PEND:" << pend;

        if (ce > 100){
            break;
        }
    }

    qDebug() << "### END 0.1";

    while(getnEnd()->collidesWithItem(this)){
        ve -= 0.001;

        if (angle != 0){
            pend = arcPointAt(line, ve);
        }else{
            pend = line.pointAt(ve);
        }

        aux_adjust();

        scene()->update();

        ce++;

        qDebug() << "VE:" << ve << "CE:" << ce << "PEND:" << pend;

        if (ce > 100){
            break;
        }
    }

    qDebug() << "### END 0.001";

    if (angle != 0){
        pend = arcPointAt(line, ve+0.001);
    }else{
        pend = line.pointAt(ve+0.001);
    }


    aux_adjust();

    scene()->update();

    qDebug() << "CE:" << ce;

    // start node
    qreal vb = 0.0;

    int cb = 0;

    while(getnBegin()->collidesWithItem(this)){
        vb += 0.5;

        if (angle != 0){
            pbegin = arcPointAt(line, vb);
        }else{
            pbegin = line.pointAt(vb);
        }

        aux_adjust();

        scene()->update();

        cb++;

        qDebug() << "VB:" << vb << "CB:" << ce << "PBEGIN:" << pbegin;

        if (cb > 100){
            break;
        }
    }

    qDebug() << "### BEGIN 0.5";

    while(!getnBegin()->collidesWithItem(this)){
        vb -= 0.1;

        if (angle != 0){
            pbegin = arcPointAt(line, vb);
        }else{
            pbegin = line.pointAt(vb);
        }

        aux_adjust();

        scene()->update();

        cb++;

        qDebug() << "VB:" << vb << "CB:" << ce << "PBEGIN:" << pbegin;

        if (cb > 100){
            break;
        }
    }

    qDebug() << "### BEGIN 0.1";

    while(getnBegin()->collidesWithItem(this)){
        vb += 0.001;

        if (angle != 0){
            pbegin = arcPointAt(line, vb);
        }else{
            pbegin = line.pointAt(vb);
        }

        aux_adjust();

        scene()->update();

        cb++;

        qDebug() << "VB:" << vb << "CB:" << ce << "PBEGIN:" << pbegin;

        if (cb > 100){
            break;
        }
    }

    qDebug() << "### BEGIN 0.001";

    if (angle != 0){
        pbegin = arcPointAt(line, vb-0.001);
    }else{
        pbegin = line.pointAt(vb-0.001);
    }

    aux_adjust();

    scene()->update();

    qDebug() << "CB:" << cb;

    if (bs){
        getnBegin()->setSelected(true);
    }

    if (es){
        getnEnd()->setSelected(true);
    }
}

void QncgEdge::validate()
{

}

void QncgEdge::link(QGraphicsSceneMouseEvent* event)
{

}

void QncgEdge::move(QGraphicsSceneMouseEvent* event)
{

}

void QncgEdge::resize(QGraphicsSceneMouseEvent* event)
{

}
