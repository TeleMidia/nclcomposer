/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTPORT_H
#define QNSTPORT_H

#include "qnstinterface.h"
#include "qnstportreference.h"

class QnstPort : public QnstInterface
{
public:
    QnstPort(QnstNode* parent = 0);

    ~QnstPort();

protected:
    virtual void link(QGraphicsSceneMouseEvent* event);
};

#endif // QNSTPORT_H
