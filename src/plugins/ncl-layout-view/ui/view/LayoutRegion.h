/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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
#ifndef QNLYGRAPHICSREGION_H
#define QNLYGRAPHICSREGION_H

#include <QAction>
#include <QActionGroup>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMenu>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QRectF>
#include <QSizeF>
#include <QStyleOptionGraphicsItem>
#include <QTextDocument>
#include <QWidget>

#include "LayoutRegionBase.h"

class LayoutRegionBase;

#define ROUND_DOUBLE(x) (x < 0.0 || x > 100.0) ? x = 0.0 : x

class LayoutRegion : public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES (QGraphicsItem)

public:
  explicit LayoutRegion (QMenu *_switchMenu, LayoutRegion *parent = 0);
  virtual ~LayoutRegion ();

  enum QnlyResizeType
  {
    Top,
    TopRight,
    Right,
    BottomRight,
    Bottom,
    BottomLeft,
    Left,
    TopLeft
  };

  bool isMoving () const;
  void setMoving (bool _moving);

  bool isCloning () const;
  void setCloning (bool cloning);

  bool isResizing () const;
  void setResizing (bool resizing);

  void setParent (LayoutRegion *region);

  bool isSelected () const;
  void setSelected (bool selected);

  bool isPainted () const;
  void setPainted (bool painted);

  bool hasChanged () const;
  void setChanged (bool changed);

  bool isValidated () const;
  void setValidated (bool validated);

  QString getId () const;
  void setId (const QString &id);

  QString getUid () const;
  void setUid (const QString &uid);

  QString getTitle () const;
  void setTitle (const QString &title);

  QString getDescriptor () const;
  void setDescriptor (const QString &descriptor);

  QString getColor () const;
  void setColor (const QString &color);

  int getzIndex () const;
  void setzIndex (int zindex);
  qreal getzIndexTop () const;

  qreal getTop () const;
  qreal getMoveTop () const;
  qreal getPressTop () const;
  qreal getResizeTop () const;

  qreal getRelativeTop () const;
  void setRelativeTop (qreal _relativeTop);

  qreal getLeft () const;
  qreal getMoveLeft () const;
  qreal getPressLeft () const;
  qreal getResizeLeft () const;

  qreal getRelativeLeft () const;
  void setRelativeLeft (qreal _relativeLeft);

  qreal getRight () const;

  qreal getRelativeRight () const;
  void setRelativeRight (qreal relativeRight);

  qreal getBottom () const;

  qreal getRelativeBottom () const;
  void setRelativeBottom (qreal relativeBottom);

  qreal getWidth () const;
  qreal getPressWidth () const;
  qreal getResizeWidth () const;

  qreal getRelativeWidth () const;
  void setRelativeWidth (qreal relativeWidth);

  qreal getHeight () const;
  qreal getPressHeight () const;
  qreal getResizeHeight () const;

  qreal getRelativeHeight () const;
  void setRelativeHeight (qreal relativeHeight);

  QnlyResizeType getResizeType () const;

  void adjust (bool repaint = true);

  void addRegion (LayoutRegion *region);
  void removeRegion (LayoutRegion *region);
  void hideRegion (LayoutRegion *region);
  void updateActionText (LayoutRegion *region);
  void updateCursor (QGraphicsSceneMouseEvent *event);
  void setGridAction (QAction *action);

  QMap<QString, QString> getAttributes ();
  QMap<QString, QString> getCloneAttributes ();

public slots:
  void performRegion ();
  void performDelete ();
  void performCopy ();
  void performCut ();
  void performPaste ();

signals:
  void regionAdditionRequested (LayoutRegion *parent);
  void regionSelectionRequested (LayoutRegion *region);
  void regionDeletionRequested (LayoutRegion *region);
  void regionChangeRequested (LayoutRegion *region,
                              QMap<QString, QString> attributes);
  void dragMediaOverRegion (QString mediaid, LayoutRegion *region);
  void regionbasePerformed ();
  void copyRequested (LayoutRegion *);
  void cutRequested (LayoutRegion *);
  void pasteRequested ();

protected:
  void setTop (qreal top);
  void setMoveTop (qreal moveTop);
  void setPressTop (qreal pressTop);
  void setResizeTop (qreal resizeTop);
  void setLeft (qreal left);
  void setMoveLeft (qreal moveLeft);
  void setPressLeft (qreal pressLeft);
  void setResizeLeft (qreal resizeLeft);
  void setRight (qreal right);
  void setBottom (qreal bottom);
  void setWidth (qreal width);
  void setPressWidth (qreal pressWidth);
  void setResizeWidth (qreal resizeWidth);
  void setHeight (qreal height);
  void setPressHeight (qreal pressHeight);
  void setResizeHeight (qreal resizeHeight);
  void setzIndexTop (qreal zindexTop);

  void setResizeType (const QnlyResizeType &_resizeType);

  virtual void move (QGraphicsSceneMouseEvent *event);
  virtual void resize (QGraphicsSceneMouseEvent *event);

  virtual QPainterPath shape () const;
  virtual QRectF boundingRect () const;
  virtual void paint (QPainter *painter,
                      const QStyleOptionGraphicsItem *option, QWidget *widget);

  virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
  virtual void mousePressEvent (QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent *event);
  virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent *event);

  void keyReleaseEvent (QKeyEvent *event);
  void keyPressEvent (QKeyEvent *event);
  void dragEnterEvent (QGraphicsSceneDragDropEvent *event);
  void dragLeaveEvent (QGraphicsSceneDragDropEvent *event);
  void dropEvent (QGraphicsSceneDragDropEvent *event);

protected slots:
  void performShow (QAction *action);
  void performHide ();
  void performExport ();

private:
  void createActions ();
  void createMenus ();
  void createConnections ();

  bool _moving;
  bool _cloning;
  bool _resizing;
  bool _selected;
  bool _validated;
  bool _painted;
  bool _changed;
  bool _isDragging;

  qreal _top;
  qreal _moveTop;
  qreal _pressTop;
  qreal _resizeTop;
  qreal _relativeTop;

  qreal _left;
  qreal _moveLeft;
  qreal _pressLeft;
  qreal _resizeLeft;
  qreal _relativeLeft;

  qreal _right;
  qreal _relativeRight;

  qreal _bottom;
  qreal _relativeBottom;

  qreal _width;
  qreal _pressWidth;
  qreal _resizeWidth;
  qreal _relativeWidth;

  qreal _height;
  qreal _pressHeight;
  qreal _resizeHeight;
  qreal _relativeHeight;

  qreal _zindex;
  qreal _zindexTop;

  QMenu *_viewMenu;
  QMenu *_showMenu;
  QMenu *_arrangeMenu;
  QMenu *_switchMenu;
  QMenu *_contextMenu;

  QAction *_helpAction;
  QAction *_undoAction;
  QAction *_redoAction;
  QAction *_cutAction;
  QAction *_copyAction;
  QAction *_pasteAction;
  QAction *_deleteAction;
  QAction *_zoominAction;
  QAction *_zoomoutAction;
  QAction *_zoomresetAction;
  QAction *_hideAction;
  QAction *_fullscreenAction;
  QAction *_exportAction;
  QAction *_regionAction;
  QAction *_regionbaseAction;
  QAction *_bringfrontAction;
  QAction *_bringforwardAction;
  QAction *_sendbackwardAction;
  QAction *_sendbackAction;
  QAction *_propertiesAction;
  QAction *_gridAction;

  qreal _controlPointSize; // Resize controller squares that appear when the
                           // region is selected.

  QString _id;
  QString _uid;
  QString _title;
  QString _color;
  // QString _descriptor;

  QList<QString> _descriptor;
  QnlyResizeType _resizeType;

  QActionGroup *_regionActionGroup;

  QMap<QString, QAction *> _regionActions;
  QMap<QString, LayoutRegion *> _regions;
  QMap<QString, QString> _cloneAttributes;
};

#endif // QNLYGRAPHICSREGION_H
