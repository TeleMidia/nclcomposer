/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTPROPERTY_H
#define QNSTPROPERTY_H

#include "qnstinterface.h"

class QnstProperty : public QnstInterface
{
public:
    QnstProperty(QnstNode* parent = 0);

    ~QnstProperty();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTPROPERTY_H
