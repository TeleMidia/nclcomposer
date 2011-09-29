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
#include "qnstnode.h"
#include "QSvgRenderer"

#include "cmath"

#include "ui_qnstdbind.h"

#include <QBitmap>
#include <QDialog>
#include <QAction>
#include <QMenu>

class QnstEdge : public QncgEdge, public QnstEntity
{
    Q_OBJECT
public:
    QnstEdge(QncgEntity* parent = 0);

    ~QnstEdge();

    void setConditionType(int contitiontype);

    int getConditionType();

    void setActionType(int contitiontype);

    int getActionType();

public slots:
    void deleteEntity();

signals:
    void entityRemoved(QnstEntity* e);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

    QMenu* viewMenu;

    QMenu* showMenu;

    QMenu* arrangeMenu;

    QMenu* contextMenu;

    QAction* helpAction;

    QAction* undoAction;

    QAction* redoAction;

    QAction* cutAction;

    QAction* copyAction;

    QAction* pasteAction;

    QAction* deleteAction;

    QAction* zoominAction;

    QAction* zoomoutAction;

    QAction* zoomresetAction;

    QAction* hideAction;

    QAction* fullscreenAction;

    QAction* exportAction;

    QAction* bringfrontAction;

    QAction* bringforwardAction;

    QAction* sendbackwardAction;

    QAction* sendbackAction;

    QAction* propertiesAction;

private:
    void createActions();

    void createMenus();

    void createConnections();

    QString conditionimage;

    QString actionimage;

    int conditiontype;

    int actiontype;
};

#endif // QNSTEDGE_H
