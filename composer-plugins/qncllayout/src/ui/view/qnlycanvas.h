/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNLYCANVAS_H
#define QNLYCANVAS_H

#include <QGraphicsView>
#include <QResizeEvent>

#include "qnlygraphicsregion.h"

class QnlyCanvas : public QGraphicsView
{
public:
    QnlyCanvas(QWidget* parent = 0);

    ~QnlyCanvas();

protected:
    virtual void resizeEvent(QResizeEvent* event);
};

#endif // QNLYCANVAS_H
