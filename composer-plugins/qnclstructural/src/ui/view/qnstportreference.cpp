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
#include "qnstportreference.h"

QnstPortReference::QnstPortReference(QncgEntity* parent)
    : QncgEdge(parent)
{
    setResizable(false);
    setSelectable(false);

    setnstType(Qnst::Refer);

    setbSpan(6);
}

QnstPortReference::~QnstPortReference()
{

}

void QnstPortReference::draw(QPainter* painter)
{
    QPointF p1;

    painter->setBrush(Qt::black);

    if (getpBegin().x() <= getpEnd().x() && getpBegin().y() <= getpEnd().y()){
        painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));
        painter->drawLine(6,6,getWidth(),getHeight());

        painter->setPen(QPen(QBrush(Qt::black), 0));
        painter->drawEllipse(0,0,12,12);
//        painter->drawEllipse(getWidth()-16,getHeight()-16,32,32);

        p1.setX(getWidth());
        p1.setY(getHeight());

    }else if (getpBegin().x() > getpEnd().x() && getpBegin().y() < getpEnd().y()){
        painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));
        painter->drawLine(getWidth()-6,6,0,getHeight());

        painter->setPen(QPen(QBrush(Qt::black), 0));
        painter->drawEllipse(getWidth()-12,0,12,12);
//        painter->drawEllipse(-8,getHeight()-8,16,16);

        p1.setX(0);
        p1.setY(getHeight());

    }else if (getpBegin().x() < getpEnd().x() && getpBegin().y() > getpEnd().y()){
        painter->setPen(QPen(QBrush(Qt::black),0, Qt::DashLine));
        painter->drawLine(6,getHeight()-6,getWidth(),0);

        painter->setPen(QPen(QBrush(Qt::black), 0));
        painter->drawEllipse(0,getHeight()-12,12,12);
//        painter->drawEllipse(getWidth()-8,-8,16,16);

        p1.setX(getWidth());
        p1.setY(0);

    }else if (getpBegin().x() > getpEnd().x() && getpBegin().y() > getpEnd().y()){
        painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));
        painter->drawLine(getWidth()-6,getHeight()-6,0,0);

        painter->setPen(QPen(QBrush(Qt::black), 0));
        painter->drawEllipse(getWidth()-12,getHeight()-12,12,12);
//        painter->drawEllipse(-8,-8,16,16);

        p1.setX(0);
        p1.setY(0);
    }

    QLineF myline(getpBegin(),getpEnd());

    QVector<QPointF> v;

    qreal PI = 3.1415926535897932;

    double angle = ::acos(myline.dx() / myline.length());

    if (myline.dy() >= 0){
         angle = (PI * 2) - angle;
    }

//    qreal alfa = myline.angle() - 180 - 90;

//    p1.setX(p1.x() - ::sin(alfa*PI/180) * 12);
//    p1.setY(p1.y() - ::cos(alfa*PI/180) * 12);

    QPointF p2 = p1 - QPointF(sin(angle + PI / 3) * 12,
                              cos(angle + PI / 3) * 12);
    QPointF p3 = p1 - QPointF(sin(angle + PI - PI / 3) * 12,
                              cos(angle + PI - PI / 3) * 12);

    painter->setBrush(Qt::black);
    painter->setPen(Qt::NoPen);

    v.append(p1);
    v.append(p2);
    v.append(p3);

    QPolygonF p(v);

    painter->drawPolygon(p);
}

void QnstPortReference::delineate(QPainterPath* painter) const
{
    QPointF p1;

    if (getpBegin().x() <= getpEnd().x() && getpBegin().y() <= getpEnd().y()){
        painter->addEllipse(0,0,12,12);

        p1.setX(getWidth());
        p1.setY(getHeight());

    }else if (getpBegin().x() > getpEnd().x() && getpBegin().y() < getpEnd().y()){
        painter->addEllipse(getWidth()-12,0,12,12);

        p1.setX(0);
        p1.setY(getHeight());

    }else if (getpBegin().x() < getpEnd().x() && getpBegin().y() > getpEnd().y()){
        painter->addEllipse(0,getHeight()-12,12,12);

        p1.setX(getWidth());
        p1.setY(0);

    }else if (getpBegin().x() > getpEnd().x() && getpBegin().y() > getpEnd().y()){
        painter->addEllipse(getWidth()-12,getHeight()-12,12,12);

        p1.setX(0);
        p1.setY(0);
    }

    QLineF myline(getpBegin(),getpEnd());

    QVector<QPointF> v;

    qreal PI = 3.1415926535897932;

    double angle = ::acos(myline.dx() / myline.length());

    if (myline.dy() >= 0){
         angle = (PI * 2) - angle;
    }

    QPointF p2 = p1 - QPointF(sin(angle + PI / 3) * 12,
                              cos(angle + PI / 3) * 12);
    QPointF p3 = p1 - QPointF(sin(angle + PI - PI / 3) * 12,
                              cos(angle + PI - PI / 3) * 12);

    v.append(p1);
    v.append(p2);
    v.append(p3);

    QPolygonF p(v);

    painter->addPolygon(p);
}

