/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "qnstproperty.h"

QnstProperty::QnstProperty(QnstNode* parent)
    : QnstInterface(parent)
{
    setnstType(Qnst::Property);
}

QnstProperty::~QnstProperty()
{

}

void QnstProperty::draw(QPainter* painter)
{
    painter->setBrush(QBrush("#EEEEEE"));

    QPolygon poly;
    poly.append(QPoint(4,4+getHeight()/2));
    poly.append(QPoint(4+getWidth()/2,4));
    poly.append(QPoint(4+getWidth(),4+getHeight()/2-1));
    poly.append(QPoint(4+getWidth()/2,4+getHeight()-1));

    if (isSelected()){

        painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));
        painter->drawPolygon(poly);
    }else{
        painter->setPen(QPen(QBrush(Qt::black), 0));
        painter->drawPolygon(poly);
    }

//    painter->setRenderHint(QPainter::Antialiasing,false);

    if (isMoving()){
        QPolygon poly2;
        poly2.append(QPoint(getMoveLeft()+4-getLeft(),getMoveTop()+4-getTop()+getHeight()/2));
        poly2.append(QPoint(getMoveLeft()+4-getLeft()+getWidth()/2,getMoveTop()+4-getTop()));
        poly2.append(QPoint(getMoveLeft()+4-getLeft()+getWidth(),getMoveTop()+4-getTop()+getHeight()/2));
        poly2.append(QPoint(getMoveLeft()+4-getLeft()+getWidth()/2,getMoveTop()+4-getTop()+getHeight()));

        if (parentItem() != NULL){
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(Qt::black), 0));// 0px = cosmetic border

            painter->drawPolygon(poly2);
        }else{
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(Qt::black), 0));// 0px = cosmetic border

            painter->drawPolygon(poly2);
        }

    }
}

void QnstProperty::delineate(QPainterPath* painter) const
{
    QPolygon poly;
    poly.append(QPoint(4,4+getHeight()/2));
    poly.append(QPoint(4+getWidth()/2,4));
    poly.append(QPoint(4+getWidth(),4+getHeight()/2));
    poly.append(QPoint(4+getWidth()/2,4+getHeight()));

    painter->addPolygon(poly);
}
