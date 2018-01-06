/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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
#ifndef QNLYCANVAS_H
#define QNLYCANVAS_H

#include <QGraphicsView>
#include <QResizeEvent>

#include "LayoutRegion.h"

class LayoutCanvas : public QGraphicsView
{
public:
  LayoutCanvas (QWidget *parent = 0);
  ~LayoutCanvas ();
  void performZoomIn ();
  void performZoomOut ();
  void performZoomReset ();

private:
  int _zoomStep;

protected:
  virtual void keyPressEvent (QKeyEvent *event);
  virtual void resizeEvent (QResizeEvent *event);
  virtual void wheelEvent (QWheelEvent *event);
};

#endif // QNLYCANVAS_H
