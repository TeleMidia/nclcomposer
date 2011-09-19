/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNCGRECTANGLE_H
#define QNCGRECTANGLE_H

#include "qncgnode.h"

class QncgRectangle : public QncgNode
{
public:
    QncgRectangle(QncgEntity* parent = 0);

    ~QncgRectangle();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;
};

#endif // QNCGRECTANGLE_H
