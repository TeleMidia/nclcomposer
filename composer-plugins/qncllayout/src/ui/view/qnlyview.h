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

#include "qnlycanvas.h"
#include "qnlygraphicsregion.h"
#include "qnlygraphicsregionbase.h"


class QnlyView : public QStackedWidget
{
    Q_OBJECT

public:
    QnlyView(QWidget* parent = 0);

    virtual ~QnlyView();

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

signals:
    void regionAdded(const QString regionUID,
                     const QString parentUID,
                     const QString regionbaseUID,
                     const QMap<QString, QString> attributes);

    void regionRemoved(const QString regionUID,
                       const QString regionbaseUID);

    void regionSelected(const QString regionUID,
                        const QString regionbaseUID);

    void regionChanged(const QString regionUID,
                       const QString regionbaseUID,
                       const QMap<QString, QString> attributes);

    void regionBaseAdded(const QString regionbaseUID,
                         const QMap<QString, QString> attributes);

    void regionBaseRemoved(const QString regionbaseUID);

    void regionBaseSelected(const QString regionbaseUID);

    void regionBaseChanged(const QString regionbaseUID,
                           const QMap<QString, QString> attributes);

protected slots:
    void addRegion(QnlyGraphicsRegion* region,
                   QnlyGraphicsRegion* parent,
                   QnlyGraphicsRegionBase* regionBase,
                   const QMap<QString, QString> attributes);

    void removeRegion(QnlyGraphicsRegion* region,
                      QnlyGraphicsRegionBase* regionBase);

    void changeRegion(QnlyGraphicsRegion* region,
                      QnlyGraphicsRegionBase* regionBase,
                      const QMap<QString, QString> attributes);

    void selectRegion(QnlyGraphicsRegion* region,
                      QnlyGraphicsRegionBase* regionBase);

    void addRegionBase(QnlyGraphicsRegionBase* regionBase,
                       const QMap<QString, QString> attributes);

    void removeRegionBase(QnlyGraphicsRegionBase* regionBase);

    void changeRegionBase(QnlyGraphicsRegionBase* regionBase,
                          const QMap<QString, QString> attributes);

    void selectRegionBase(QnlyGraphicsRegionBase* regionBase);

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

    QnlyGraphicsRegion* selectedRegion;

    QnlyGraphicsRegionBase* selectedRegionBase;

    int nregions;

    int nregionbases;

    QActionGroup* regionbaseActionGroup;

    QMap<QString, QAction*> regionbaseActions;

    QMap<QString, QnlyGraphicsRegion*> regions;

    QMap<QString, QnlyGraphicsRegionBase*> regionbases;
};

#endif // QNLYVIEW_H
