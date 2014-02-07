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
#ifndef QNLYVIEW_H
#define QNLYVIEW_H

#include <QStackedWidget>
#include <QContextMenuEvent>
#include <QResizeEvent>
#include <QMenu>
#include <QAction>
#include <QMap>
#include <QSize>
#include <QUuid>
#include <QActionGroup>
#include <QDebug>
#include <QGraphicsView>

#include "Canvas.h"
#include "Region.h"
#include "RegionBase.h"


class View : public QStackedWidget
{
    Q_OBJECT

public:
    explicit View(QWidget* parent = 0);
    virtual ~View();

    RegionBase *getSelectedRegionBase();
    Region *getSelectedRegion();

    bool gridVisibility;
    QMap <QString, QString> copiedRegionAttrs;

public slots:
    void addRegion(const QString regionUID,
                   const QString parentUID,
                   const QString regionbaseUID,
                   const QMap<QString, QString> attributes);

    void removeRegion(const QString regionUID,
                      const QString regionbaseUID);

    void changeRegion(const QString regionUID,
                      const QString regionbaseUID,
                      const QMap<QString, QString> attributes);

    void selectRegion(const QString regionUID,
                      const QString regionbaseUID);

    void addRegionBase(const QString regionbaseUID,
                       const QMap<QString, QString> attributes);

    void removeRegionBase(const QString regionbaseUID);

    void changeRegionBase(const QString regionbaseUID,
                          const QMap<QString, QString> attributes);

    void selectRegionBase(const QString regionbaseUID);

    void performSwitch(QAction* action);

    void performRegionBase();

    void requestRegionAddition(const QString regionUID,
                               const QString parentUID,
                               const QString regionbaseUID,
                               QMap<QString, QString> attributes);

    void requestRegionChange(const QString regionUID,
                             const QString regionbaseUID,
                             QMap<QString, QString> attributes);

    void setGridVisible(bool visible);

    void snapshot();

    void performCopy(Region *);

    void performPaste();

signals:
    void regionAdded(const QString regionUID,
                     const QString parentUID,
                     const QString regionbaseUID,
                     const QMap<QString, QString> attributes);

    void regionRemoved(const QString regionUID,
                       const QString regionbaseUID);

    void regionSelected(const QString regionUID,
                        const QString regionbaseUID);

    void regionChanged(const QString &regionUID,
                       const QString &regionbaseUID,
                       const QMap<QString, QString> &attributes);

    void regionBaseAdded(const QString &regionbaseUID,
                         const QMap<QString, QString> &attributes);

    void regionBaseRemoved(const QString regionbaseUID);

    void regionBaseSelected(const QString regionbaseUID);

    void regionBaseChanged(const QString &regionbaseUID,
                           const QMap<QString, QString> &attributes);

    void mediaOverRegionAction(QString mediaID, QString regionUID);

    void gridVisibilityChanged(bool visibility);

protected slots:
    void addRegion(Region* region,
                   Region* parent,
                   RegionBase* regionBase,
                   const QMap<QString, QString> attributes);

    void removeRegion(Region* region,
                      RegionBase* regionBase);

    void changeRegion(Region* region,
                      RegionBase* regionBase,
                      const QMap<QString, QString> attributes);

    void selectRegion(Region* region,
                      RegionBase* regionBase);

    void addRegionBase(RegionBase* regionBase,
                       const QMap<QString, QString> attributes);

    void removeRegionBase(RegionBase* regionBase);

    void changeRegionBase(RegionBase* regionBase,
                          const QMap<QString, QString> attributes);

    void selectRegionBase(RegionBase* regionBase);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

private:
    void createActions();
    void createMenus();
    void createConnections();

    QMenu* viewMenu;
    QMenu* insertMenu;
    QMenu* showMenu;
    QMenu* arrangeMenu;
    QMenu* switchMenu;
    QMenu* contextMenu;
    // \todo This actions must be in a place that will be shared by
    //       qnlyview and qnlymainwindow
    QAction* helpAction;
    // QAction* undoAction;  // Temporaly disabled
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

    QActionGroup* regionbaseActionGroup;

    Region* selectedRegion;
    RegionBase* selectedRegionBase;

    int nregions;
    int nregionbases;

    QMap<QString, QAction*> regionbaseActions;
    QMap<QString, Region*> regions;
    QMap<QString, RegionBase*> regionbases;
};

#endif // QNLYVIEW_H
