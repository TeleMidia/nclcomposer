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
#include "LayoutCanvas.h"

#include <QScrollBar>

#include <cmath>

LayoutCanvas::LayoutCanvas(QWidget* parent) : QGraphicsView(parent)
{
  _zoomStep = 0;
  setBackgroundBrush(QBrush(QColor("#eee")));
}

LayoutCanvas::~LayoutCanvas()
{

}

void LayoutCanvas::resizeEvent(QResizeEvent* event)
{
  QGraphicsView::resizeEvent(event);

  /* QSize size = event->size();
  QGraphicsScene* s = scene();
  s->setSceneRect(0, 0, size.width(), size.height());

  foreach(QGraphicsItem* item, s->items())
  {
    LayoutRegion* region = (LayoutRegion*) item;
    region->adjust();
  }*/
}

void LayoutCanvas::wheelEvent(QWheelEvent *event)
{
  if(event->modifiers() == Qt::ControlModifier)
  {
    if (event->delta() > 0)
      performZoomIn();
    else
      performZoomOut();

    event->accept();
  }
  else
  {
    // call the parent wheelEvent
    QGraphicsView::wheelEvent(event);
  }
}

void LayoutCanvas::keyPressEvent(QKeyEvent *event)
{

  if(event->modifiers() & Qt::ControlModifier)
  {
    if(event->key() == Qt::Key_0)
    {
      performZoomReset();
      event->accept();
    }

    if(event->key() == Qt::Key_Minus)
    {
      performZoomOut();
      event->accept();
    }

    if(event->key() == Qt::Key_Plus)
    {
      performZoomIn();
      event->accept();
      /* ctrl+plus was not working properly as a shortcut for action*/
    }
  }

  QGraphicsView::keyPressEvent(event);
}

void LayoutCanvas::performZoomIn()
{
  if (_zoomStep > 0)
  {
    _zoomStep--;
    resetMatrix();
    scale(1 - _zoomStep*0.05, 1 - _zoomStep*0.05);
  }
}

void LayoutCanvas::performZoomOut()
{
  if (_zoomStep*0.05 < 0.9)
  {
    _zoomStep++;
    resetMatrix();
    scale(1 - _zoomStep*0.05, 1 - _zoomStep*0.05);
  }
}

void LayoutCanvas::performZoomReset()
{
  _zoomStep = 0;
  resetMatrix();
}
