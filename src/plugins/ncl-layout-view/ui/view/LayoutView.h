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

#include "LayoutCanvas.h"
#include "LayoutRegion.h"
#include "LayoutRegionBase.h"

class LayoutView : public QStackedWidget
{
  Q_OBJECT

public:
  explicit LayoutView(QWidget* parent = 0);
  virtual ~LayoutView();

  LayoutRegionBase *getSelectedRegionBase();
  LayoutRegion *getSelectedRegion();

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

  void doSnapshot();

  void performCopy();
  void performCopy(LayoutRegion *);
  void performCut();
  void performCut(LayoutRegion *);
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
  void addRegion(LayoutRegion* region,
                 LayoutRegion* parent,
                 LayoutRegionBase* regionBase,
                 const QMap<QString, QString> attributes);

  void removeRegion(LayoutRegion* region,
                    LayoutRegionBase* regionBase);

  void changeRegion(LayoutRegion* region,
                    LayoutRegionBase* regionBase,
                    const QMap<QString, QString> attributes);

  void selectRegion(LayoutRegion* region,
                    LayoutRegionBase* regionBase);

  void addRegionBase(LayoutRegionBase* regionBase,
                     const QMap<QString, QString> attributes);

  void removeRegionBase(LayoutRegionBase* regionBase);

  void changeRegionBase(LayoutRegionBase* regionBase,
                        const QMap<QString, QString> attributes);

  void selectRegionBase(LayoutRegionBase* regionBase);

protected:
  virtual void contextMenuEvent(QContextMenuEvent *event);

private:
  void createActions();
  void createMenus();
  void createConnections();

  QMenu* viewMenu;
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

  LayoutRegion* selectedRegion;
  LayoutRegionBase* selectedRegionBase;

  int nregions;
  int nregionbases;

  QMap<QString, QAction*> regionbaseActions;
  QMap<QString, LayoutRegion*> regions;
  QMap<QString, LayoutRegionBase*> regionbases;
};

#endif // QNLYVIEW_H
