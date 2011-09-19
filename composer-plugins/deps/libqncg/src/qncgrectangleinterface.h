/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNCGRECTANGLEINTERFACE_H
#define QNCGRECTANGLEINTERFACE_H

#include "qncginterface.h"

class QncgRectangleInterface : public QncgInterface
{
public:
    QncgRectangleInterface(QncgEntity* parent = 0);

    ~QncgRectangleInterface();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;
};

#endif // QNCGRECTANGLEINTERFACE_H
