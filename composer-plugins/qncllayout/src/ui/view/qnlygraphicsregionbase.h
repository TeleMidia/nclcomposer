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
#ifndef QNLYGRAPHICSREGIONBASE_H
#define QNLYGRAPHICSREGIONBASE_H

#include <QMenu>
#include <QAction>
#include <QGraphicsScene>
#include <QDebug>
#include <QActionGroup>
#include <QGraphicsSceneContextMenuEvent>
#include <QObject>
#include <QMap>
#include <QGraphicsSceneMouseEvent>
#include <QFileDialog>

#include "qnlygraphicsregion.h"
#include "qnlygraphicsgrid.h"

class QnlyGraphicsRegion;

class QnlyGraphicsRegionBase : public QGraphicsScene
{
    Q_OBJECT

public:
    QnlyGraphicsRegionBase(QObject* parent, QMenu* switchMenu);

    ~QnlyGraphicsRegionBase();

    QString getId() const;

    void setId(const QString &id);

    QString getUid() const;

    void setUid(const QString &uid);

    QString getRegion() const;

    void setRegion(const QString &region);

    QString getDevice() const;

    void setDevice(const QString &device);

    void addRegion(QnlyGraphicsRegion* region,
                   QnlyGraphicsRegion* parent,
                   const QMap<QString, QString> attributes);

    void changeRegion(QnlyGraphicsRegion* region,
                    const QMap<QString, QString> attributes);

    void selectRegion(QnlyGraphicsRegion* region);

    void hideRegion(QnlyGraphicsRegion* region);

    void updateActionText(QnlyGraphicsRegion *region);

    void removeRegion(QnlyGraphicsRegion* region);

    QGraphicsItem* getBackgroundItem();

    void changeResolution(int w, int h);

    bool isGridVisible();

    void setGridVisible(bool active);

signals:
    void regionAdditionRequested(const QString regionUID,
                     const QString parentUID,
                     const QString regionbaseUID,
                     QMap<QString, QString> attributes);

    void regionChangeRequested(const QString regionUID,
                               const QString regionbaseUID,
                               QMap<QString, QString> attributes);

    void regionSelectionRequested(
            const QString regionUID,
            const QString regionbaseUID);

    void regionDeletionRequested(
            const QString regionUID,
            const QString regionbaseUID);

    void regionBaseDeletionRequested(
            const QString regionbaseUID);

    void regionBaseSelectionRequested(const QString regionbaseUID);

    void mediaOverRegion(const QString &mediaId, const QString &regionUID);

    void regionbasePerformed();

    void gridVisibilityChanged(bool gridVisibility);

    void copyRequested(QnlyGraphicsRegion *);
    void pasteRequested();

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

public slots:
    void performRegion();

    void performDelete();

    void performExport();

protected slots:
    void performShow(QAction* action);

    void requestAdditionRegion(QnlyGraphicsRegion* parent);

    void requestRegionSelection(QnlyGraphicsRegion* region);

    void requestRegionDeletion(QnlyGraphicsRegion* region);

    void requestRegionChange(QnlyGraphicsRegion* region,
                                QMap<QString, QString> attributes);

    void requestMediaOverRegionAction(QString mediaId,
                                      QnlyGraphicsRegion* region);

    void performChangeResolution();

    void performGrid();

private:
    void createActions();

    void createMenus();

    void createConnections();

    QMenu* viewMenu;

    QMenu* insertMenu;

    QMenu* showMenu;

    QMenu* arrangeMenu;

    QMenu* screensizeMenu;

    QMenu* switchMenu;

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

    QAction* regionAction;

    QAction* regionbaseAction;

    QAction* bringfrontAction;

    QAction* bringforwardAction;

    QAction* sendbackwardAction;

    QAction* sendbackAction;

    QAction* propertiesAction;

    QAction* gridAction;

    QAction* action;

    // \todo This could be an array (and the user could also add its own size)
    QAction* re640x480;
    QAction* re800x600;
    QAction* re1024x768;
    QAction* re854x480;
    QAction* re1280x720;
    QAction* re1920x1080;
    QAction* re320x400;

    QActionGroup* screensizeGroup;

    QActionGroup* regionActionGroup;

    QGraphicsTextItem *graphicsRegionBaseId;

    QString id;

    QString uid;

    QString region;

    QString device;

    QGraphicsRectItem* bgrect;

    QnlyGraphicsGrid* grid;

    QnlyGraphicsRegion* selectedRegion;

    QMap<QString, QAction*> regionActions;

    QMap<QString, QnlyGraphicsRegion*> regions;
};

#endif // QNLYGRAPHICSREGIONBASE_H
