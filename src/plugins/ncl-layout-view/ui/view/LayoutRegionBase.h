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

#include "LayoutRegion.h"
#include "LayoutGrid.h"

class LayoutRegion;

class LayoutRegionBase : public QGraphicsScene
{
  Q_OBJECT

public:
  explicit LayoutRegionBase(QObject* parent, QMenu* _switchMenu);
  virtual ~LayoutRegionBase();

  QString getId() const;
  void setId(const QString &_id);

  QString getUid() const;
  void setUid(const QString &_uid);

  QString getRegion() const;
  void setRegion(const QString &_region);

  QString getDevice() const;
  void setDevice(const QString &_device);

  void addRegion(LayoutRegion* _region,
                 LayoutRegion* parent,
                 const QMap<QString, QString> attributes);
  void changeRegion(LayoutRegion* _region,
                    const QMap<QString, QString> attributes);
  void selectRegion(LayoutRegion* _region);
  void hideRegion(LayoutRegion* _region);
  void updateActionText(LayoutRegion *_region);
  void removeRegion(LayoutRegion* _region);

  QGraphicsItem* getBackgroundItem();

  void changeResolution(int w, int h);


  bool isGridVisible();
  void setGridVisible(bool active);
  void updateSafeAreaPos();
  bool isSafeAreaVisible();
  void setSafeAreaVisible(bool active);

public slots:
  void performRegion();
  void performDelete();
  void performExport();
  void performSafeArea();

signals:
  void regionAdditionRequested(const QString regionUID,
                               const QString parentUID,
                               const QString regionbaseUID,
                               QMap<QString, QString> attributes);

  void regionChangeRequested(const QString regionUID,
                             const QString regionbaseUID,
                             QMap<QString, QString> attributes);

  void regionSelectionRequested( const QString regionUID,
                                 const QString regionbaseUID);

  void regionDeletionRequested( const QString regionUID,
                                const QString regionbaseUID);

  void regionBaseDeletionRequested(const QString regionbaseUID);

  void regionBaseSelectionRequested(const QString regionbaseUID);

  void mediaOverRegion(const QString &mediaId, const QString &regionUID);

  void regionbasePerformed();

  void copyRequested(LayoutRegion *);
  void cutRequested(LayoutRegion *);
  void pasteRequested();

  void gridVisibilityChanged(bool visible);
  void safeAreaVisibilityChanged(bool visible);

protected:
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  void mousePressEvent(QGraphicsSceneMouseEvent* event);
  void keyPressEvent( QKeyEvent * event );

  /*!
   * \brief createGrid creates the grid element that can be hide/unhide in the
   *  scene.
   */
  void createGrid();
  /*!
   * \brief createSafeArea creates the QGraphicsRectItem responsible to drawn
   *  safeArea information.
   *
   *  For more information about safe are you can look at: SMPTE ST 2046-1
   */
  void createSafeArea();
  /*!
   * \brief getSafeAreaRect returns a rect related that represents the safe area
   *  of a rectangle with size w x h.
   * \param w
   * \param h
   * \param w_perc
   * \param h_perc
   * \return
   */
  QRect getSafeAreaRect(int w, int h, double w_perc, double h_perc);

protected slots:
  void performShow(QAction* _action);

  void requestAdditionRegion(LayoutRegion* parent);
  void requestRegionSelection(LayoutRegion* _region);
  void requestRegionDeletion(LayoutRegion* _region);
  void requestRegionChange(LayoutRegion* _region,
                           QMap<QString, QString> attributes);
  void requestMediaOverRegionAction(QString mediaId,
                                    LayoutRegion* _region);
  void performChangeResolution();
  void performGrid();

private:
  void createActions();
  void createMenus();
  void createConnections();

  QMenu* _viewMenu;
  QMenu* _showMenu;
  QMenu* _arrangeMenu;
  QMenu* _screensizeMenu;
  QMenu* _switchMenu;
  QMenu* _contextMenu;

  QAction* _helpAction;
  QAction* _undoAction;
  QAction* _redoAction;
  QAction* _cutAction;
  QAction* _copyAction;
  QAction* _pasteAction;
  QAction* _deleteAction;
  QAction* _zoominAction;
  QAction* _zoomoutAction;
  QAction* _zoomresetAction;
  QAction* _hideAction;
  QAction* _fullscreenAction;
  QAction* _exportAction;
  QAction* _regionAction;
  QAction* _regionbaseAction;
  QAction* _bringfrontAction;
  QAction* _bringforwardAction;
  QAction* _sendbackwardAction;
  QAction* _sendbackAction;
  QAction* _propertiesAction;
  QAction* _gridAction;
  QAction* _safeAreaAction;
  QAction* _action;

  QList<QAction*> _resolutions;

  QActionGroup* _screensizeGroup;
  QActionGroup* _regionActionGroup;

  QGraphicsTextItem *_graphicsRegionBaseId;

  QString _id;
  QString _uid;
  QString _region;
  QString _device;

  QGraphicsRectItem* _bgrect;
  QGraphicsRectItem* _safeActionAreaRect;
  QGraphicsTextItem* _safeActionAreaText;
  QGraphicsRectItem* _safeTitleAreaRect;
  QGraphicsTextItem* _safeTitleAreaText;

  LayoutGrid* _grid;

  LayoutRegion* _selectedRegion;

  QMap<QString, QAction*> _regionActions;
  QMap<QString, LayoutRegion*> _regions;
};

#endif // QNLYGRAPHICSREGIONBASE_H
