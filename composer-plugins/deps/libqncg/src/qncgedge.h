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

    qreal getAdjustedAngle();

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

    virtual QPointF arcPointAt(QLineF line, qreal at);

    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void link(QGraphicsSceneMouseEvent* event);

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);

private:
    qreal angle;

    qreal adjustedangle;

    QPointF pend;

    QPointF pbegin;

    QncgEntity* nend;

    QncgEntity* nbegin;

    qreal bspan;

    qreal espan;
};

#endif // QNCGEDGE_H
