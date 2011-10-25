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
