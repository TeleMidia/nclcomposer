/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTPORTREFERENCE_H
#define QNSTPORTREFERENCE_H

#include <cmath>
#include "qncgedge.h"

class QnstPortReference : public QncgEdge
{
public:
    QnstPortReference(QncgEntity* parent = 0);

    ~QnstPortReference();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTPORTREFERENCE_H
