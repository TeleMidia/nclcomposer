/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNCGEDGE_H
#define QNCGEDGE_H

#include "qncgentity.h"

#include "qncgnode.h"

class QncgEdge : public QncgEntity
{
public:
    QncgEdge(QncgEntity* parent = 0);

    ~QncgEdge();

    qreal getAngle();

    void setAngle(qreal angle);

    QPointF getpBegin() const;

    QPointF setpBegin(QPointF pbegin);

    QPointF getpEnd() const;

    QPointF setpEnd(QPointF pend);

    QncgEntity* getnBegin();

    QncgEntity* setnBegin(QncgEntity* nbegin);

    QncgEntity* getnEnd();

    QncgEntity* setnEnd(QncgEntity* nend);

    qreal getbSpan() const;

    qreal setbSpan(qreal bspan);

    qreal geteSpan() const;

    qreal seteSpan(qreal bepan);

    virtual void adjust();

protected:
    virtual void validate();

    virtual void aux_adjust();

    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void link(QGraphicsSceneMouseEvent* event);

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);

private:
    qreal angle;

    QPointF pend;

    QPointF pbegin;

    QncgEntity* nend;

    QncgEntity* nbegin;

    qreal bspan;

    qreal espan;
};

#endif // QNCGEDGE_H
