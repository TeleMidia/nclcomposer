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
