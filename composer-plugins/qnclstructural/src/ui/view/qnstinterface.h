/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTINTERFACE_H
#define QNSTINTERFACE_H

#include <QAction>
#include <QMenu>

#include "qncginterface.h"

#include "qnstnode.h"
#include "qnstentity.h"

class QnstNode;

class QnstInterface : public QncgInterface, public QnstEntity
{
    Q_OBJECT

public:
    QnstInterface(QnstNode* parent = 0);

    ~QnstInterface();

    void setName(QString name);

    QMap<QString, QVector<qreal> > getAngles() const;

    void addAngle(QString uid, qreal angle);

    void removeAngle(QString uid, qreal angle);

public slots:
    void deleteEntity();

signals:
    void entitySelected(QnstEntity* e);

    void entityAdded(QnstEntity* e);

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

    QMap<QString, QVector<qreal> > angles;

private slots:
    void internalselection();
};

#endif // QNSTINTERFACE_H
