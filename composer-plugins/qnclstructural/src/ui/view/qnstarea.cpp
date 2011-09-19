/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "qnstarea.h"

QnstArea::QnstArea(QnstNode* parent)
    : QnstInterface(parent)
{
    setnstType(Qnst::Area);
}

QnstArea::~QnstArea()
{

}

void QnstArea::draw(QPainter* painter)
{
    painter->setBrush(QBrush("#EEEEEE"));

    if (isSelected()){

        painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));
        painter->drawEllipse(4, 4, getWidth()-1, getHeight()-1);
    }else{
        painter->drawEllipse(4, 4, getWidth()-1, getHeight()-1);
    }

    if (isMoving()){
        if (parentItem() != NULL){
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(Qt::black), 0));// 0px = cosmetic border

            painter->drawEllipse(getMoveLeft()+4-getLeft(),
                                 getMoveTop()+4-getTop(),
                                 getWidth()-1,
                                 getHeight()-1);
        }else{
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(Qt::black), 0));// 0px = cosmetic border

            painter->drawEllipse(getMoveLeft()+4-getLeft(),
                              getMoveTop()+4-getTop(),
                              getWidth()-1,
                              getHeight()-1);
        }

    }else if (isResizing()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawEllipse(getResizeLeft()+4-getLeft(),
                             getResizeTop()+4-getTop(),
                             getResizeWidth()-1,
                             getResizeHeight()-1);
    }
}

void QnstArea::delineate(QPainterPath* painter) const
{
    painter->addEllipse(4, 4, getWidth()-1, getHeight()-1);
}
