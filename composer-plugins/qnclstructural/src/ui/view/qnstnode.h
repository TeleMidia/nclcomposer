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
#ifndef QNSTNODE_H
#define QNSTNODE_H

#include "QMenu"
#include "QAction"
#include "QObject"
#include <QGraphicsSceneMouseEvent>
#include "QGraphicsSceneContextMenuEvent"

#include "qncgnode.h"
#include "qnstedge.h"
#include "qnstinterface.h"
#include "qnstentity.h"

#include "ui_qnstdbind.h"

#include <QDialog>

class QnstInterface;

class QnstNode : public QncgNode, public QnstEntity
{
    Q_OBJECT

public:
    QnstNode(QnstNode* parent = 0);

    ~QnstNode();

    QString getColor() const;

    void setColor(QString color);

    QString getBorderColor() const;

    void setBordertColor(QString borderColor);

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

    virtual void link(QGraphicsSceneMouseEvent* event);

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

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

    QAction* propertiesAction;

private:
    void createActions();

    void createMenus();

    void createConnections();

    QString color;

    QString borderColor;

    QMap<QString, QVector<qreal> > angles;

private slots:
    void internalselection();
};

#endif // QNSTNODE_H
