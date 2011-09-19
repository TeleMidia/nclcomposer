/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTEDGE_H
#define QNSTEDGE_H

#include "qncgedge.h"

#include "qncgentity.h"
#include "qnstentity.h"
#include "QSvgRenderer"

#include "cmath"

#include "ui_qnstdbind.h"

#include <QBitmap>
#include <QDialog>

class QnstEdge : public QncgEdge, public QnstEntity
{
public:
    QnstEdge(QncgEntity* parent = 0);

    ~QnstEdge();

    void setConditionType(int contitiontype);

    int getConditionType();

    void setActionType(int contitiontype);

    int getActionType();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

private:
    QString conditionimage;

    QString actionimage;

    int conditiontype;

    int actiontype;
};

#endif // QNSTEDGE_H
