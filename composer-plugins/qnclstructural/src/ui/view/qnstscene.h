/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTSCENE_H
#define QNSTSCENE_H

#include "QGraphicsScene"
#include "QMenu"
#include "QAction"
#include "QGraphicsSceneContextMenuEvent"

#include "qnstbody.h"

class QnstScene : public QGraphicsScene
{
    Q_OBJECT

public:
    QnstScene(QObject* parent = 0);

    ~QnstScene();

public slots:
    void addBody();

    void link(bool);

signals:
    void entityAdded(QnstEntity* e);

    void entityRemoved(QString e);

    void entityChanged(QnstEntity* e);

    void entitySelected(QnstEntity* e);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

private:
    void createActions();

    void createMenus();

    void createConnections();

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

    QAction* mediaAction;

    QAction* portAction;

    QAction* propertyAction;

    QAction* areaAction;

    QAction* aggregatorAction;

    QAction* bringfrontAction;

    QAction* bringforwardAction;

    QAction* sendbackwardAction;

    QAction* sendbackAction;

    QAction* linkAction;

    QAction* propertiesAction;
};

#endif // QNSTSCENE_H
