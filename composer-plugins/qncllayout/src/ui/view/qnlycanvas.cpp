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
#include "qnlycanvas.h"

QnlyCanvas::QnlyCanvas(QWidget* parent) : QGraphicsView(parent)
{
    setBackgroundBrush(QBrush(QColor("#999")));
}

QnlyCanvas::~QnlyCanvas()
{

}

void QnlyCanvas::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

//    QSize size = event->size();

//    QGraphicsScene* s = scene();

//    s->setSceneRect(0,0,size.width(),size.height());

//    foreach(QGraphicsItem* item, s->items())
//    {
//        QnlyGraphicsRegion* region = (QnlyGraphicsRegion*) item;

//        region->adjust();
//    }
}
