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
#ifndef QNCGINTERFACE_H
#define QNCGINTERFACE_H

#include "qncgentity.h"
#include "qncgedge.h"

class QncgInterface : public QncgEntity
{
public:
    QncgInterface(QncgEntity* parent = 0);

    ~QncgInterface();

    virtual void adjust();

    void addBeginningEdge(QncgEdge* edge);

    void removeBeginningEdge(QncgEdge* edge);

    QVector<QncgEdge*> getBeginningEdges() const;

    void addEndingEdge(QncgEdge* edge);

    void removeEndingEdge(QncgEdge* edge);

    QVector<QncgEdge*> getEndingEdges() const;


protected:
    virtual void validate();

    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void link(QGraphicsSceneMouseEvent* event);

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);

private:
    QVector<QncgEdge*> bedges;

    QVector<QncgEdge*> eedges;
};

#endif // QNCGINTERFACE_H
