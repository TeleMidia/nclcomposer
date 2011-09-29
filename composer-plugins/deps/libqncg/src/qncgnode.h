/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNCGNODE_H
#define QNCGNODE_H

#include <QGraphicsScene>
#include <QVector>

#include "qncgentity.h"
#include "qncgedge.h"

class QncgEdge;

class QncgNode : public QncgEntity
{
public:
    QncgNode(QncgEntity* parent = 0);

    ~QncgNode();

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

#endif // QNCGNODE_H
