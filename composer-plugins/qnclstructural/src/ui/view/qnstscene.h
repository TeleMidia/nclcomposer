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
#ifndef QNSTSCENE_H
#define QNSTSCENE_H

#include <QMenu>
#include <QAction>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>

#include <QGraphicsScene>

#include <QDebug>

#include "qnstentity.h"
#include "qnstgraphicsbody.h"

class QnstScene : public QGraphicsScene
{
    Q_OBJECT

public:
    QnstScene(QObject* parent = 0);

    ~QnstScene();

    void setLinking(bool linking);

    bool isLinking();

signals:
    void entityAdded(QnstEntity* entity);

    void entityRemoved(QnstEntity* entity);

    void entityChanged(QnstEntity* entity);

    void entitySelected(QnstEntity* entity);

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

protected slots:
    void performBody();

private:
    void createActions();

    void createMenus();

    void createConnections();

    bool linking;

    QMenu* viewMenu;

    QMenu* insertMenu;

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

    QAction* bodyAction;

    QAction* contextAction;

    QAction* switchAction;

    QAction* imageAction;

    QAction* videoAction;

    QAction* audioAction;

    QAction* textAction;

    QAction* scriptAction;

    QAction* settingsAction;

    QAction* portAction;

    QAction* propertyAction;

    QAction* areaAction;

    QAction* aggregatorAction;

    QAction* bringfrontAction;

    QAction* bringforwardAction;

    QAction* sendbackwardAction;

    QAction* sendbackAction;

    QAction* propertiesAction;
};

#endif // QNSTSCENE_H
