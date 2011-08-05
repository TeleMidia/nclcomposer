/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "qnlycanvas.h"

QnlyCanvas::QnlyCanvas(QWidget* parent) : QGraphicsView(parent)
{

}

QnlyCanvas::~QnlyCanvas()
{

}

void QnlyCanvas::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

    QSize size = event->size();

    QGraphicsScene* s = scene();

    s->setSceneRect(0,0,size.width(),size.height());

    foreach(QGraphicsItem* item, s->items())
    {
        QnlyGraphicsRegion* region = (QnlyGraphicsRegion*) item;

        region->adjust();
    }
}
