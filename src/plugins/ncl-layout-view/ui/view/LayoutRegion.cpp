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
#include "LayoutRegion.h"

#include <QCursor>
#include <QMimeData>
#include <QGuiApplication>

#define KEYBOARD_MOVE_REGION_STEP 10.0
#define KEYBOARD_MOVE_REGION_STEP_SMALL 1.0

LayoutRegion::LayoutRegion(QMenu* switchMenu, LayoutRegion* parent)
  : QObject(parent), QGraphicsItem(parent)
{
  this->_switchMenu = switchMenu;
  this->_switchMenu->setEnabled(true);

  /* creating */
  createActions();
  createMenus();
  createConnections();

  /* setting defaults */
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsFocusable, true);

  setMoving(false);
  setCloning(false);
  setResizing(false);
  setSelected(false);

  setValidated(true);

  setId("");
  setUid("");
  setTitle("");
  setDescriptor("");

  setRelativeTop(0);
  setRelativeLeft(0);
  setRelativeRight(0);
  setRelativeBottom(0);

  setRelativeWidth(1);
  setRelativeHeight(1);

  setzIndex(-1);
  setzIndexTop(0);

  setColor("#E4E4E4");

  _isDragging = false;

  _controlPointSize = 8;
}

LayoutRegion::~LayoutRegion()
{
  delete(_viewMenu);
  delete(_arrangeMenu);
  delete(_contextMenu);
}

bool LayoutRegion::isMoving() const
{
  return _moving;
}

void LayoutRegion::setParent(LayoutRegion* region)
{
  QObject::setParent(region);
  setParentItem(region);
}

void LayoutRegion::setMoving(bool moving)
{
  this->_moving = moving;
  if(moving)
    setCursor(QCursor(Qt::ClosedHandCursor));
  else
    setCursor(QCursor(Qt::ArrowCursor));
}

bool LayoutRegion::isCloning() const
{
  return _cloning;
}
void LayoutRegion::setCloning(bool cloning)
{
  this->_cloning = cloning;
}

bool LayoutRegion::isResizing() const
{
  return _resizing;
}

void LayoutRegion::setResizing(bool resizing)
{
  this->_resizing = resizing;
}

bool LayoutRegion::isSelected() const
{
  return _selected;
}

void LayoutRegion::setSelected(bool selected)
{
  this->_selected = selected;

  if(this->_selected)
    setFocus(Qt::MouseFocusReason);
}

bool LayoutRegion::isValidated() const
{
  return _validated;
}

void LayoutRegion::setValidated(bool validated)
{
  this->_validated = validated;
}

QString LayoutRegion::getId() const
{
  return _id;
}

void LayoutRegion::setId(const QString &id)
{
  this->_id = id;

  QString descriptorId = "";
  if (!_descriptor.empty())
    descriptorId = " "+_descriptor.first();

  setToolTip(_title+" "+"("+_id+")"+descriptorId);
  if (parentItem() != NULL)
  {
    LayoutRegion* parent = (LayoutRegion*) parentItem();
    parent->updateActionText(this);
  }
  else
  {
    LayoutRegionBase* s = (LayoutRegionBase*) scene();

    if(s != NULL)
      s->updateActionText(this);
  }
}

QString LayoutRegion::getUid() const
{
  return _uid;
}

void LayoutRegion::setUid(const QString &uid)
{
  this->_uid = uid;
}

QString LayoutRegion::getTitle() const
{
  return _title;
}

void LayoutRegion::setTitle(const QString &title)
{
  this->_title = title;

  QString descriptorId = "";
  if (!_descriptor.empty())
    descriptorId = " "+_descriptor.first();

  setToolTip(_title+" "+"("+_id+")"+descriptorId);
}

QString LayoutRegion::getDescriptor() const
{
  return _descriptor.first();
}

void LayoutRegion::setDescriptor(const QString &descriptor)
{
  //this->_descriptor[0] = descriptor;
  this->_descriptor.push_front(descriptor);
  setToolTip(_title+" "+"("+_id+") "+_descriptor.first());
}

QString LayoutRegion::getColor() const
{
  return _color;
}

void LayoutRegion::setColor(const QString &color)
{
  this->_color = color;
}

bool LayoutRegion::isPainted() const
{
  return _painted;
}

void LayoutRegion::setPainted(bool painted)
{
  this->_painted = painted;
}

void LayoutRegion::setzIndex(int zindex)
{
  this->_zindex = zindex;

  setZValue(zindex);
}

int LayoutRegion::getzIndex() const
{
  return _zindex;
}

void LayoutRegion::setzIndexTop(qreal zindexTop)
{
  this->_zindexTop = zindexTop;
}

qreal LayoutRegion::getzIndexTop() const
{
  return _zindexTop;
}

qreal LayoutRegion::getTop() const
{
  return _top;
}

void LayoutRegion::setTop(qreal top)
{
  this->_top = top;

  setY(top);
}

qreal LayoutRegion::getMoveTop() const
{
  return _moveTop;
}

void LayoutRegion::setMoveTop(qreal moveTop)
{
  this->_moveTop = moveTop;
}

qreal LayoutRegion::getPressTop() const
{
  return _pressTop;
}

void LayoutRegion::setPressTop(qreal pressTop)
{
  this->_pressTop = pressTop;
}

qreal LayoutRegion::getResizeTop() const
{
  return _resizeTop;
}

void LayoutRegion::setResizeTop(qreal resizeTop)
{
  this->_resizeTop = resizeTop;
}

qreal LayoutRegion::getRelativeTop() const
{
  return _relativeTop;
}

void LayoutRegion::setRelativeTop(qreal relativeTop)
{
  this->_relativeTop = relativeTop;
}

qreal LayoutRegion::getLeft() const
{
  return _left;
}

void LayoutRegion::setLeft(qreal left)
{
  this->_left = left;

  setX(left);
}

qreal LayoutRegion::getMoveLeft() const
{
  return _moveLeft;
}

void LayoutRegion::setMoveLeft(qreal moveLeft)
{
  this->_moveLeft = moveLeft;
}

qreal LayoutRegion::getPressLeft() const
{
  return _pressLeft;
}

void LayoutRegion::setPressLeft(qreal pressLeft)
{
  this->_pressLeft = pressLeft;
}

qreal LayoutRegion::getResizeLeft() const
{
  return _resizeLeft;
}

void LayoutRegion::setResizeLeft(qreal resizeLeft)
{
  this->_resizeLeft = resizeLeft;
}

qreal LayoutRegion::getRelativeLeft() const
{
  return _relativeLeft;
}

void LayoutRegion::setRelativeLeft(qreal relativeLeft)
{
  this->_relativeLeft = relativeLeft;
}

qreal LayoutRegion::getRight() const
{
  return _right;
}

void LayoutRegion::setRight(qreal right)
{
  this->_right = right;
}

qreal LayoutRegion::getRelativeRight() const
{
  return _relativeRight;
}

void LayoutRegion::setRelativeRight(qreal relativeRight)
{
  this->_relativeRight = relativeRight;
}

qreal LayoutRegion::getBottom() const
{
  return _bottom;
}

void LayoutRegion::setBottom(qreal bottom)
{
  this->_bottom = bottom;
}

qreal LayoutRegion::getRelativeBottom() const
{
  return _relativeBottom;
}

void LayoutRegion::setRelativeBottom(qreal relativeBottom)
{
  this->_relativeBottom = relativeBottom;
}

qreal LayoutRegion::getWidth() const
{
  return _width;
}

void LayoutRegion::setWidth(qreal width)
{
  this->_width = width;
}

qreal LayoutRegion::getPressWidth() const
{
  return _pressWidth;
}

void LayoutRegion::setPressWidth(qreal pressWidth)
{
  this->_pressWidth = pressWidth;
}

qreal LayoutRegion::getResizeWidth() const
{
  return _resizeWidth;
}

bool LayoutRegion::hasChanged() const
{
  return _changed;
}

void LayoutRegion::setChanged(bool changed)
{
  this->_changed = changed;
}

void LayoutRegion::setResizeWidth(qreal resizeWidth)
{
  this->_resizeWidth = resizeWidth;
}

qreal LayoutRegion::getRelativeWidth() const
{
  return _relativeWidth;
}

void LayoutRegion::setRelativeWidth(qreal relativeWidth)
{
  this->_relativeWidth = relativeWidth;
}

qreal LayoutRegion::getHeight() const
{
  return _height;
}

void LayoutRegion::setHeight(qreal height)
{
  this->_height = height;
}

qreal LayoutRegion::getPressHeight() const
{
  return _pressHeight;
}

void LayoutRegion::setPressHeight(qreal pressHeight)
{
  this->_pressHeight = pressHeight;
}

qreal LayoutRegion::getResizeHeight() const
{
  return _resizeHeight;
}

void LayoutRegion::setResizeHeight(qreal resizeHeight)
{
  this->_resizeHeight = resizeHeight;
}

qreal LayoutRegion::getRelativeHeight() const
{
  return _relativeHeight;
}

void LayoutRegion::setRelativeHeight(qreal relativeHeight)
{
  this->_relativeHeight = relativeHeight;
}

LayoutRegion::QnlyResizeType LayoutRegion::getResizeType() const
{
  return _resizeType;
}

void LayoutRegion::setResizeType(const QnlyResizeType &resizeType)
{
  this->_resizeType = resizeType;
}

void LayoutRegion::LayoutRegion::createActions()
{
  // help action
  _helpAction = new QAction(this);
  _helpAction->setText(tr("Help"));

  _helpAction->setEnabled(false);
  _helpAction->setShortcut(QKeySequence("F1"));

  // undo action
  _undoAction = new QAction(this);
  _undoAction->setText(tr("Undo"));

  _undoAction->setEnabled(false);
  _undoAction->setShortcut(QKeySequence("Ctrl+Z"));

  // redo action
  _redoAction = new QAction(this);
  _redoAction->setText(tr("Redo"));

  _redoAction->setEnabled(false);
  _redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

  // cut action
  _cutAction = new QAction(this);
  _cutAction->setText(tr("Cut"));

  _cutAction->setEnabled(true);
  // cutAction->setShortcut(QKeySequence("Ctrl+X"));

  // copy action
  _copyAction = new QAction(this);
  _copyAction->setText(tr("Copy"));
  _copyAction->setEnabled(true);

  // paste action
  _pasteAction = new QAction(this);
  _pasteAction->setText(tr("Paste"));

  _pasteAction->setEnabled(true);
  // pasteAction->setShortcut(QKeySequence("Ctrl+V"));

  // delete action
  _deleteAction = new QAction(this);
  _deleteAction->setText(tr("&Delete"));
  _deleteAction->setIcon(QIcon(":/icon/minus"));

  _deleteAction->setEnabled(true);
  _deleteAction->setShortcut(QKeySequence("Del"));

  // zoomin action
  _zoominAction = new QAction(this);
  _zoominAction->setText(tr("Zoom In"));

  _zoominAction->setEnabled(true);
  _zoominAction->setShortcut(QKeySequence("Ctrl++"));

  // zoomout action
  _zoomoutAction = new QAction(this);
  _zoomoutAction->setText(tr("Zoom Out"));

  _zoomoutAction->setEnabled(true);
  _zoomoutAction->setShortcut(QKeySequence("Ctrl+-"));

  // reset action
  _zoomresetAction = new QAction(this);
  _zoomresetAction->setText(tr("Reset"));

  _zoomresetAction->setEnabled(true);
  _zoomresetAction->setShortcut(QKeySequence("Ctrl+0"));

  // fullscreen action
  _fullscreenAction = new QAction(this);
  _fullscreenAction->setText(tr("Full Screen"));

  _fullscreenAction->setEnabled(true);
  _fullscreenAction->setShortcut(QKeySequence("F11"));

  // export action
  _exportAction = new QAction(this);
  _exportAction->setText(tr("Export..."));

  _exportAction->setEnabled(true);

  // region action
  _regionAction = new QAction(this);
  _regionAction->setText(tr("Add &Region"));
  _regionAction->setIcon(QIcon(":/icon/plus"));

  _regionAction->setEnabled(true);

  // regionbase action
  _regionbaseAction = new QAction(this);
  _regionbaseAction->setText(tr("Add Region &Base"));
  _regionbaseAction->setIcon(QIcon(":/icon/plus"));

  _regionbaseAction->setEnabled(true);

  // bring to front action
  _bringfrontAction = new QAction(this);
  _bringfrontAction->setText(tr("Bring to Front"));

  _bringfrontAction->setEnabled(false);
  _bringfrontAction->setShortcut(QKeySequence("Shift+Ctrl+]"));

  // bring forward action
  _bringforwardAction = new QAction(this);
  _bringforwardAction->setText(tr("Bring Forward"));

  _bringforwardAction->setEnabled(false);
  _bringforwardAction->setShortcut(QKeySequence("Ctrl+]"));

  // send backward action
  _sendbackwardAction = new QAction(this);
  _sendbackwardAction->setText(tr("Send Backward"));

  _sendbackwardAction->setEnabled(false);
  _sendbackwardAction->setShortcut(QKeySequence("Ctrl+["));

  // send to back action
  _sendbackAction = new QAction(this);
  _sendbackAction->setText(tr("Send to Back"));

  _sendbackAction->setEnabled(false);
  _sendbackAction->setShortcut(QKeySequence("Shift+Ctrl+["));

  // hide action
  _hideAction = new QAction(this);
  _hideAction->setText(tr("Hide"));

  _hideAction->setEnabled(true);

  // properties action
  _propertiesAction = new QAction(this);
  _propertiesAction->setText(tr("Properties"));

  _propertiesAction->setEnabled(true);

  _regionActionGroup = new QActionGroup(this);
  _regionActionGroup->setExclusive(false);

  setAcceptDrops(true);
}

void LayoutRegion::setGridAction(QAction* action)
{
  _gridAction = action;

  _showMenu->addAction(_gridAction);

}

void LayoutRegion::createMenus()
{
  // view menu
  _viewMenu = new QMenu();
  _viewMenu->setTitle(tr("View"));

  _viewMenu->setEnabled(true);

  _viewMenu->addAction(_zoominAction);
  _viewMenu->addAction(_zoomoutAction);
  _viewMenu->addAction(_zoomresetAction);
  _viewMenu->addSeparator();
  _viewMenu->addAction(_fullscreenAction);

  // show menu
  _showMenu = new QMenu();
  _showMenu->setTitle(tr("Show"));

  _showMenu->setEnabled(true);

  // arrange menu
  _arrangeMenu = new QMenu();
  _arrangeMenu->setTitle(tr("Arrange"));

  _arrangeMenu->setEnabled(false);

  _arrangeMenu->addAction(_bringfrontAction);
  _arrangeMenu->addAction(_bringforwardAction);
  _arrangeMenu->addAction(_sendbackwardAction);
  _arrangeMenu->addAction(_sendbackAction);

  // context menu
  _contextMenu = new QMenu();

  _contextMenu->addAction(_regionAction);
  // contextMenu->addAction(regionbaseAction);
  _contextMenu->addSeparator();
  _contextMenu->addAction(_deleteAction);
  _contextMenu->addSeparator();
  //    contextMenu->addAction(undoAction);
  //    contextMenu->addAction(redoAction);
  //    contextMenu->addSeparator();
  _contextMenu->addAction(_cutAction);
  _contextMenu->addAction(_copyAction);
  _contextMenu->addAction(_pasteAction);
  _contextMenu->addSeparator();
  _contextMenu->addAction(_exportAction);
  _contextMenu->addSeparator();
  // contextMenu->addMenu(viewMenu); // disabled for while

  _contextMenu->addMenu(_showMenu);
  // contextMenu->addMenu(arrangeMenu); // disabled for while
  _contextMenu->addSeparator();
  _contextMenu->addAction(_hideAction);
  _contextMenu->addSeparator();
  _contextMenu->addMenu(_switchMenu);

  _contextMenu->addSeparator();
  _contextMenu->addAction(_helpAction);
  // contextMenu->addSeparator(); //disabled for now
  // contextMenu->addAction(propertiesAction); //disabled for now
}

void LayoutRegion::createConnections()
{
  connect(_regionActionGroup, SIGNAL(triggered(QAction*)),
          SLOT(performShow(QAction*)));

  connect(_regionbaseAction, SIGNAL(triggered()),
          SIGNAL(regionbasePerformed()));

  connect(_regionAction, SIGNAL(triggered()),
          SLOT(performRegion()));

  connect(_hideAction, SIGNAL(triggered()),
          SLOT(performHide()));

  connect(_deleteAction, SIGNAL(triggered()),
          SLOT(performDelete()));

  connect(_exportAction, SIGNAL(triggered()),
          SLOT(performExport()));

  connect(_copyAction, SIGNAL(triggered()),
          SLOT(performCopy()));

  connect(_cutAction, SIGNAL(triggered()),
          SLOT(performCut()));

  connect(_pasteAction, SIGNAL(triggered()),
          SLOT(performPaste()));
}

void LayoutRegion::updateActionText(LayoutRegion *region)
{
  // Update Show Menu
  if(_regionActions.contains(region->getUid()))
    _regionActions[region->getUid()]->setText(region->getId());
}

void LayoutRegion::hideRegion(LayoutRegion* region)
{
  _regionActions[region->getUid()]->trigger();
}

void LayoutRegion::performDelete()
{
  emit regionDeletionRequested(this);
}

void LayoutRegion::performExport()
{
  QString location =
      QFileDialog::getSaveFileName(NULL, "Export...", "",
                                   tr("Images (*.png)"));

  if (location != "")
  {
    qreal w = getWidth();
    qreal h = getHeight();

    QImage image(w, h, QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&image);

    QPointF p = mapToScene(0,0);

    scene()->render(&painter, QRect(), QRect(p.x(),p.y(),w,h));

    painter.end();

    image.save(location, "PNG");
  }
}

void LayoutRegion::performHide()
{
  if (parentItem() != NULL)
  {
    LayoutRegion* parent = (LayoutRegion*) parentItem();

    parent->hideRegion(this);
  }
  else
  {
    LayoutRegionBase* s = (LayoutRegionBase*) scene();

    s->hideRegion(this);
  }
}

void LayoutRegion::performRegion()
{
  emit regionAdditionRequested(this);
}

void LayoutRegion::performShow(QAction* action)
{
  if (!action->isChecked())
  {
    _regions[_regionActions.key(action)]->setVisible(false);
  }
  else
  {
    _regions[_regionActions.key(action)]->setVisible(true);
  }
}

void LayoutRegion::performCopy()
{
  qWarning () << "LayoutRegion::performCopy";
  emit copyRequested(this);
}

void LayoutRegion::performCut()
{
  qWarning () << "LayoutRegion::performCut";
  emit cutRequested(this);
}

void LayoutRegion::performPaste()
{
  emit pasteRequested();
}

void LayoutRegion::removeRegion(LayoutRegion* region)
{
  if(_regionActions.contains(region->getUid()))
  {
    QAction *action = _regionActions[region->getUid()];

    _showMenu->removeAction(action);

    _regionActionGroup->removeAction(action);
  }

  _regionActions.remove(region->getUid());
}

void LayoutRegion::addRegion(LayoutRegion* region)
{
  if (region != NULL)
  {
    region->setParent(this);

    region->setGridAction(_gridAction);

    _regions[region->getUid()] = region;

    QAction* action = new QAction(this);
    action->setText(region->getId());

    _showMenu->addAction(action);

    _showMenu->insertAction(_showMenu->actions().front(), action);

    if (_showMenu->actions().size() <= 2){
      _showMenu->insertSeparator(_showMenu->actions().back());
    }

    action->setCheckable(true);
    action->setChecked(true);
    action->setEnabled(true);

    _regionActionGroup->addAction(action);

    _regionActions[region->getUid()] = action;
  }
}

void LayoutRegion::move(QGraphicsSceneMouseEvent* event)
{
  /* setting */
  qreal x = _left;
  qreal y = _top;

  /* setting minimal position */
  qreal minx;
  qreal miny;

//  if (parentItem() != NULL)
//  {
//    minx = 4;
//    miny = 4;
//  }
//  else
//  {
    minx = 0;
    miny = 0;
//  }

  /* setting maximal position */
  qreal maxx;
  qreal maxy;

  if (parentItem() != NULL)
  {
    maxx = parentItem()->boundingRect().width() - _width - _controlPointSize;
    maxy = parentItem()->boundingRect().height() - _height - _controlPointSize;
  }
  else
  {
    maxx = scene()->width() - _width;
    maxy = scene()->height() - _height;
  }

  /* setting delta */
  qreal dx = event->pos().x() - _pressLeft; // (x1 - x0)
  qreal dy = event->pos().y() - _pressTop;  // (y1 - y0)

  /* setting next position */
  qreal nextx = x + dx;
  qreal nexty = y + dy;

  /* adjusting */
  if (nextx < minx)
  {
    nextx = minx;
  }

  if (nexty < miny)
  {
    nexty = miny;
  }

  if (nextx > maxx)
  {
    nextx = maxx;
  }

  if (nexty > maxy)
  {
    nexty = maxy;
  }

  /* moving */
  setMoveTop(nexty);
  setMoveLeft(nextx);

  scene()->update();
}


void LayoutRegion::resize(QGraphicsSceneMouseEvent* event)
{
  /* setting bounds */
  qreal x = _left;
  qreal y = _top;
  qreal w = _width;
  qreal h = _height;

  /*setting aspect ratio,used with shift modifier on resize*/
  qreal aspectratio = _width/_height;

  /* setting minimal bounds */
  qreal minx;
  qreal miny;

//  if (parentItem() != NULL)
//  {
//    minx = 4;
//    miny = 4;
//  }
//  else
//  {
    minx = 0;
    miny = 0;
//  }

  /* setting maximal bounds */
//  qreal maxx;
//  qreal maxy;
  qreal maxw;
  qreal maxh;

  if (parentItem() != NULL)
  {
    maxw = parentItem()->boundingRect().width() - _controlPointSize;
    maxh = parentItem()->boundingRect().height() - _controlPointSize;
  }
  else
  {
//    maxx = scene()->width() - width;
//    maxy = scene()->height() - height;
    maxw = scene()->width();
    maxh = scene()->height();
  }

  /* setting delta */
  qreal dx = event->pos().x() - _pressLeft;    // (x1 - x0)
  qreal dy = event->pos().y() - _pressTop;     // (y1 - y0)
  qreal dw = -dx;
  qreal dh = -dy;

  /* setting next bounds */
  qreal nextx = x + dx;
  qreal nexty = y + dy;
  qreal nextw = w + dw;
  qreal nexth = h + dh;

  /* adjusting */
  switch(_resizeType)
  {
    /* adjusting TOPLEFT */
    case LayoutRegion::TopLeft:
      {
        if (nextx < minx)
        {
          nextx = minx;
          nextw = x + w - minx;
        }
        if(QGuiApplication::keyboardModifiers() == Qt::ShiftModifier) /*maintain ratio*/
        {
          nexth = nextw/aspectratio;
          nexty = y + h - nexth;
          if (nexty < miny)
          {
            nexth = y + h - miny;
            nextw = nexth * aspectratio;
            nexty = miny;
            nextx = x + w - nextw;
          }
        }
        else
        {
          if (nexty < miny)
          {
            nexty = miny;
            nexth = y + h - miny;
          }
        }
        break;
      }
    /* adjusting TOP */
    case LayoutRegion::Top:
      {
        if (nexty < miny)
        {
          nexty = miny;
          nexth = y + h - miny;
        }
        nextx = x; // fixed x

        if(QGuiApplication::keyboardModifiers() == Qt::ShiftModifier) /*maintain ratio*/
        {
          nextw = nexth*aspectratio;

          if (x + nextw > maxw)
          {
            nextw = maxw - x;
            nexth = nextw/aspectratio;
            nexty = y + h - nexth;
          }
        }
        else
        {
          nextw = w; // fixed width
        }

        break;
      }

    /* adjusting TOPRIGHT */
    case LayoutRegion::TopRight:
      {
        nextx = x; // fixed x

        nextw = w - dw;
        if (x + nextw > maxw)
        {
          nextw = maxw - x;
        }

        if(QGuiApplication::keyboardModifiers() == Qt::ShiftModifier) /*maintain ratio*/
        {
          nexth = nextw/aspectratio;
          nexty = y + h - nexth;
          if (nexty < miny)
          {
            nexth = y + h - miny;
            nextw = nexth * aspectratio;
            nexty = miny;
            nextx = x;
          }
        }
        else
        {
          if (nexty < miny)
          {
            nexty = miny;
            nexth = y + h - miny;
          }
        }
        break;
      }

    /* adjusting RIGHT */
    case LayoutRegion::Right:
      {
        nextx = x; // fixed x

        nextw = w - dw;
        if (x + nextw > maxw)
        {
          nextw = maxw - x;
        }

        nexty = y; // fixed y

        if(QGuiApplication::keyboardModifiers() == Qt::ShiftModifier) /*maintain ratio*/
        {
          nexth = nextw/aspectratio;
          if (y + nexth > maxh)
          {
            nexth = maxh - y;
            nextw = nexth * aspectratio;
            nexty = y;
          }
        }
        else
        {
          nexth = h; // fixed height
        }

        break;
      }

      /* adjusting BOTTOMRIGHT */
    case LayoutRegion::BottomRight:
      {
        nextx = x; // fixed x

        nextw = w - dw;
        if (x + nextw > maxw){
          nextw = maxw - x;
        }

        nexty = y; // fixed y

        if(QGuiApplication::keyboardModifiers() == Qt::ShiftModifier) /*maintain ratio*/
        {
          nexth = nextw/aspectratio;
          if (y + nexth > maxh)
          {
            nexth = maxh - y;
            nextw = nexth * aspectratio;
          }
        }
        else
        {
          nexth = h - dh;
          if (y + nexth > maxh)
          {
            nexth = maxh - y;
          }
        }
        break;
      }

      /* adjusting BOTTOM */
    case LayoutRegion::Bottom:
      {
        nexty = y; // fixed y

        nexth = h - dh;
        if (y + nexth > maxh)
        {
          nexth = maxh - y;
        }

        nextx = x; // fixed x
        if(QGuiApplication::keyboardModifiers() == Qt::ShiftModifier) /*maintain ratio*/
        {
          nextw = nexth*aspectratio;
          if (x + nextw > maxw)
          {
            nextw = maxw - x;
            nexth = nextw/aspectratio;
            nexty = y;
          }
        }
        else
        {
          nextw = w; // fixed width
        }

        break;
      }

      /* adjusting BOTTOMLEFT */
    case LayoutRegion::BottomLeft:
      {
        if (nextx < minx)
        {
          nextx = minx;
          nextw = x + w - minx;
        }

        nexty = y; // fixed y

        if(QGuiApplication::keyboardModifiers() == Qt::ShiftModifier) /*maintain ratio*/
        {
          nexth = nextw/aspectratio;
          if (y + nexth > maxh)
          {
            nexth = maxh - y;
            nextw = nexth * aspectratio;
            nextx = x + w - nextw;
          }
        }
        else
        {
          nexth = h - dh;
          if (y + nexth > maxh)
          {
            nexth = maxh - y;
          }
        }
        break;
      }

      /* adjusting LEFT */
    case LayoutRegion::Left:
      {
        if (nextx < minx)
        {
          nextx = minx;
          nextw = x + w - minx;
        }

        nexty = y; // fixed y
        if(QGuiApplication::keyboardModifiers() == Qt::ShiftModifier) /*maintain ratio*/
        {
          nexth = nextw/aspectratio;
          if (y + nexth > maxh)
          {
            nexth = maxh - y;
            nextw = nexth * aspectratio;
            nextx = x + w - nextw;
          }
        }
        else
        {
          nexth = h; // fixed height
        }

        break;
      }
  }

  /* resizing */
  setResizeTop(nexty);
  setResizeLeft(nextx);
  setResizeWidth(nextw);
  setResizeHeight(nexth);

  scene()->update();
}

void LayoutRegion::adjust(bool repaint)
{
  if (parentItem() != NULL)
  {
    LayoutRegion* item = (LayoutRegion*) parentItem();

    setTop(qRound(item->getHeight()*_relativeTop));
    setLeft(qRound(item->getWidth()*_relativeLeft));

    setWidth(qRound(item->getWidth()*_relativeWidth));
    setHeight(qRound(item->getHeight()*_relativeHeight));

    setRight(item->getWidth() - (_left+_width));      // not use to paint
    setBottom(item->getHeight() - (_top+_height));    // not use to paint

  }
  else
  {
    setTop(qRound(scene()->height()*_relativeTop));
    setLeft(qRound(scene()->width()*_relativeLeft));

    setWidth(qRound(scene()->width()*_relativeWidth));
    setHeight(qRound(scene()->height()*_relativeHeight));

    setRight(scene()->width() - (_left+_width));      // not use to paint
    setBottom(scene()->height() - (_top+_height));    // not use to paint
  }

  for (int i=0;i<childItems().size();++i)
  {
    LayoutRegion* item = (LayoutRegion*) childItems().at(i);

    item->adjust(false);
  }

  if (repaint)
  {
    scene()->update();
  }
}

QPainterPath LayoutRegion::shape() const
{
  QPainterPath path;

  path.addRect(0, 0, _width, _height);

  if (_selected)
  {
    path.setFillRule(Qt::WindingFill);

    path.addRect(0, 0, 8, 8);                                  // topleft
    path.addRect((_width+8)/2 - 4,0,8,8);                    // top
    path.addRect((_width+8) - 8,0,8,8);                      // topright
    path.addRect((_width+8) - 8,(_height+8)/2 - 4,8,8);       // right
    path.addRect((_width+8) - 8,(_height+8) - 8,8,8);         // bottomright
    path.addRect((_width+8)/2 - 4,(_height+8) - 8,8,8);       // bottom
    path.addRect(0,(_height+8) - 8,8,8);                     // bottomleft
    path.addRect(0,(_height+8)/2 - 4,8,8);                   // left
  }

  return path;
}

QRectF LayoutRegion::boundingRect() const
{
  QRectF bounds;

  bounds.setX(- _controlPointSize/2);
  bounds.setY(- _controlPointSize/2);
  bounds.setWidth(_width + _controlPointSize);
  bounds.setHeight(_height + _controlPointSize);

  return bounds;
}

void LayoutRegion::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
  Q_UNUSED(option)
  Q_UNUSED(widget)

  QString color = this->_color;

  if(_isDragging)
    color = "#C4D8E2"; // FIXME: This should be configurable

  painter->setBrush(QColor(color));

  if (_selected)
    painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));
  else
    painter->setPen(QPen(QBrush(Qt::black), 0));

  painter->drawRect(0, 0, _width, _height);

  if (_moving)
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0));    // 0px = cosmetic border
    painter->drawRect(_moveLeft - _left, _moveTop - _top, _width, _height);
  }
  else if (_cloning)
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0));    // 0px = cosmetic border
    painter->drawRect(_moveLeft - _left, _moveTop - _top, _width, _height);
  }
  else if (_resizing)
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0));// 0px = cosmetic border
    painter->drawRect(_resizeLeft - _left, _resizeTop - _top,_resizeWidth,_resizeHeight);
  }
  else if (_selected)
  {
    painter->setBrush(QBrush(Qt::white));
    // 0px = cosmetic border
    painter->setPen(QPen(QBrush(QBrush(Qt::black)), 0));

    painter->drawRect(-_controlPointSize/2, -_controlPointSize/2, _controlPointSize, _controlPointSize);                                 // topleft
    painter->drawRect(_width/2 - _controlPointSize/2, -_controlPointSize/2, _controlPointSize, _controlPointSize);                   // top
    painter->drawRect(_width - _controlPointSize/2, -_controlPointSize/2, _controlPointSize ,_controlPointSize);                     // topright
    painter->drawRect(_width - _controlPointSize/2, _height/2 -_controlPointSize/2, _controlPointSize, _controlPointSize);      // right
    painter->drawRect(_width - _controlPointSize/2, _height - _controlPointSize/2, _controlPointSize, _controlPointSize);        // bottomright
    painter->drawRect(_width/2 - _controlPointSize/2, _height -_controlPointSize/2, _controlPointSize, _controlPointSize);      // bottom
    painter->drawRect(-_controlPointSize/2, _height -_controlPointSize/2, _controlPointSize, _controlPointSize);                    // bottomleft
    painter->drawRect(-_controlPointSize/2, _height/2-_controlPointSize/2, _controlPointSize, _controlPointSize);                  // left
  }

  QString text = "";

  if (_width >= 25 && _height >= 25 && _id != "")
  {
    text = _title+" "+"("+_id+")";
  }

  painter->drawText(4+6, 4+6, _width-1-4-6, _height-1-4-6, Qt::AlignLeft, text);
}

void LayoutRegion::updateCursor(QGraphicsSceneMouseEvent* event)
{
  Qt::CursorShape newShape;
  if(!_resizing && !_moving)
  {
    QPointF pos = mapFromScene(event->scenePos());

    // in the middle (UP or DOWN)
    if (QRectF(_width/2 -_controlPointSize/2, -_controlPointSize/2, _controlPointSize, _controlPointSize).contains(pos) ||
        QRectF(_width/2 -_controlPointSize/2, _height-_controlPointSize/2, _controlPointSize, _controlPointSize).contains(pos))
    {
      newShape = Qt::SizeVerCursor;
    }

    // TOPLEFT or BOTTOM RIGHT
    else if (QRectF(-_controlPointSize/2,-_controlPointSize/2, _controlPointSize, _controlPointSize).contains(pos) ||
             QRectF(_width-_controlPointSize/2, _height-_controlPointSize/2, _controlPointSize, _controlPointSize).contains(pos))

    {
      newShape = Qt::SizeFDiagCursor;
    }

    // TOPRIGHT or BOTTOMLEFT
    else if (QRectF(_width-_controlPointSize/2, -_controlPointSize/2, _controlPointSize, _controlPointSize).contains(pos) ||
             QRectF(-_controlPointSize/2, _height-_controlPointSize/2, _controlPointSize, _controlPointSize).contains(pos))
    {
      newShape = Qt::SizeBDiagCursor;
    }

    // RIGHT OR LEFT
    else if (QRectF(_width-_controlPointSize/2, _height/2-_controlPointSize/2, _controlPointSize, _controlPointSize).contains(pos) ||
             QRectF(-_controlPointSize/2, _height/2-_controlPointSize/2, _controlPointSize, _controlPointSize).contains(pos))
    {
      newShape = Qt::SizeHorCursor;
    }

    else
    {
      newShape = Qt::ArrowCursor;
    }

    if(newShape != cursor().shape())
    {
      setCursor(newShape);
    }
  }
}

void LayoutRegion::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  if (_moving)
    move(event);
  else if (_cloning)
    move(event); //
  else if (_resizing)
    resize(event);
}

void LayoutRegion::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  if (event->button() == Qt::RightButton)
  {
    event->ignore();

  }
  else if (event->button() == Qt::LeftButton)
  {
    emit regionSelectionRequested(this);


    setPressTop(event->pos().y());
    setPressLeft(event->pos().x());
    setPressWidth(_width);
    setPressHeight(_height);

    /* avoid flickering on first move */
    setMoveTop(_top);
    setMoveLeft(_left);

    /* avoid flickering on first resize */
    setResizeTop(_top);
    setResizeLeft(_left);
    setResizeWidth(_width);
    setResizeHeight(_height);

    /* if over TOPLEFT resize region */
    if (QRectF(-_controlPointSize/2, -_controlPointSize/2, _controlPointSize, _controlPointSize).contains(event->pos()))
    {
      setResizeType(LayoutRegion::TopLeft);
      setResizing(true);
    }
    /* if over TOP resize region */
    else if (QRectF(_width/2 -_controlPointSize/2, -_controlPointSize/2, _controlPointSize, _controlPointSize).contains(event->pos()))
    {
      setResizeType(LayoutRegion::Top);
      setResizing(true);
    }
    /* if over TOPRIGHT resize region */
    else if (QRectF(_width-_controlPointSize/2, -_controlPointSize/2, _controlPointSize, _controlPointSize).contains(event->pos()))
    {
      setResizeType(LayoutRegion::TopRight);
      setResizing(true);
    }
    /* if over RIGHT resize region */
    else if (QRectF(_width-_controlPointSize/2,_height/2-_controlPointSize/2, _controlPointSize, _controlPointSize).contains(event->pos()))
    {
      setResizeType(LayoutRegion::Right);
      setResizing(true);
    }
    /* if over BOTTOMRIGHT resize region */
    else if (QRectF(_width-_controlPointSize/2, _height-_controlPointSize/2, _controlPointSize, _controlPointSize).contains(event->pos()))
    {
      setResizeType(LayoutRegion::BottomRight);
      setResizing(true);
    }
    /* if over BOTTOM resize region */
    else if (QRectF(_width/2-_controlPointSize/2, _height-_controlPointSize/2, _controlPointSize, _controlPointSize).contains(event->pos()))
    {
      setResizeType(LayoutRegion::Bottom);
      setResizing(true);
    }
    /* if over BOTTOMLEFT resize region */
    else if (QRectF(-_controlPointSize/2, _height-_controlPointSize/2, _controlPointSize, _controlPointSize).contains(event->pos()))
    {
      setResizeType(LayoutRegion::BottomLeft);
      setResizing(true);
    }
    /* if over LEFT resize region */
    else if (QRectF(-_controlPointSize/2, _height/2-_controlPointSize/2, _controlPointSize, _controlPointSize).contains(event->pos()))
    {
      setResizeType(LayoutRegion::Left);
      setResizing(true);
    }
    /* if not over any resize region */
    else
    {
      if(QGuiApplication::keyboardModifiers() == Qt::ControlModifier) /*clone if control is held*/
      {
        setCloning(true);
      }
      else
      {
        setMoving(true);
      }
    }
  }

  event->accept();

  QGraphicsItem::mousePressEvent(event);
}

void LayoutRegion::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  if (_moving)
  {
    setMoving(false);

    if ((_top != _moveTop || _left != _moveLeft))
    {
      QMap<QString, QString> attributes;
      double value = 0.0;
      qreal parentW, parentH;

      if (parentItem() != NULL)
      {
        parentW = ((LayoutRegion*) parentItem())->getWidth();
        parentH = ((LayoutRegion*) parentItem())->getHeight();
      }
      else
      {
        parentW = scene()->width();
        parentH = scene()->height();
      }

      value = (_moveTop/parentH) * 100;
      ROUND_DOUBLE(value);
      attributes["top"] = QString::number(value, 'f', 2) + "%";

      value = (_moveLeft/parentW) * 100;
      ROUND_DOUBLE(value);
      attributes["left"] = QString::number(value, 'f', 2) + "%";

      value = (1 - ((_moveLeft/parentW)+(_width/parentW))) * 100;
      ROUND_DOUBLE(value);
      attributes["right"] = QString::number(value, 'f', 2) + "%";

      value = (1 - ((_moveTop/parentH)+(_height/parentH)))*100;
      ROUND_DOUBLE(value);
      attributes["bottom"] = QString::number(value, 'f', 2) + "%";

      attributes["zIndex"] = QString::number(getzIndex());

      setChanged(true);
      emit regionChangeRequested(this, attributes);
    }
  }

  if (_cloning)
  {
    emit copyRequested(this);//cloning overwrites what is currently in "clipboard"

    if ((_top != _moveTop || _left != _moveLeft))
    {
      double value = 0.0;
      qreal parentW, parentH;

      if (parentItem() != NULL)
      {
        parentW = ((LayoutRegion*) parentItem())->getWidth();
        parentH = ((LayoutRegion*) parentItem())->getHeight();
      }
      else
      {
        parentW = scene()->width();
        parentH = scene()->height();
      }

      value = (_moveTop/parentH) * 100;
      ROUND_DOUBLE(value);
      _cloneAttributes["top"] = QString::number(value, 'f', 2) + "%";

      value = (_moveLeft/parentW) * 100;
      ROUND_DOUBLE(value);
      _cloneAttributes["left"] = QString::number(value, 'f', 2) + "%";

      value = (1 - ((_moveLeft/parentW)+(_width/parentW))) * 100;
      ROUND_DOUBLE(value);
      _cloneAttributes["right"] = QString::number(value, 'f', 2) + "%";

      value = (1 - ((_moveTop/parentH)+(_height/parentH)))*100;
      ROUND_DOUBLE(value);
      _cloneAttributes["bottom"] = QString::number(value, 'f', 2) + "%";

      _cloneAttributes["zIndex"] = QString::number(getzIndex());
    }


    /* performing paste */
    emit performPaste();

    setCloning(false);
  }

  if (_resizing)
  {
    setResizing(false);

    if ((_top != _resizeTop || _left != _resizeLeft ||
         _width != _resizeWidth || _height != _resizeHeight))
    {
      if (_resizeTop > _top + _height)
      {
        setResizeHeight(_resizeTop - (_top + _height));
        setResizeTop(_top + _height);
      }

      if (_resizeLeft > _left + _width)
      {
        setResizeWidth(_resizeLeft - (_left + _width));
        setResizeLeft(_left + _width);
      }

      if (_resizeWidth < 0)
      {
        setResizeLeft(_resizeLeft + _resizeWidth);
        setResizeWidth(-_resizeWidth);
      }

      if (_resizeHeight < 0)
      {
        setResizeTop(_resizeTop + _resizeHeight);
        setResizeHeight(-_resizeHeight);
      }

      QMap<QString, QString> attrs;
      qreal parentW, parentH;
      double value = 0.0;

      if(parentItem() != NULL)
      {
        parentW = ((LayoutRegion*)parentItem())->getWidth();
        parentH = ((LayoutRegion*)parentItem())->getHeight();
      }
      else
      {
        parentW = (scene())->width();
        parentH = (scene())->height();
      }

      value = (_resizeTop/parentH)*100;
      ROUND_DOUBLE(value);
      attrs["top"] = QString::number(value, 'f', 2) + "%";

      value = (_resizeLeft/parentW)*100;
      ROUND_DOUBLE(value);
      attrs["left"] = QString::number(value, 'f', 2) + "%";

      value = (_resizeHeight/parentH)*100;
      ROUND_DOUBLE(value);
      attrs["height"] = QString::number(value, 'f', 2) + "%";

      value = (_resizeWidth/parentW)*100;
      ROUND_DOUBLE(value);
      attrs["width"] = QString::number(value, 'f', 2) + "%";

      value = (1 - ((_resizeLeft/parentW)+
                    (_resizeWidth/parentW)))*100;
      ROUND_DOUBLE(value);
      attrs["right"] = QString::number(value, 'f', 2) + "%";

      value = (1 - ((_resizeTop/parentH)+ (_resizeHeight/parentH)))*100;
      ROUND_DOUBLE(value);
      attrs["bottom"] = QString::number(value, 'f', 2) + "%";

      attrs["zIndex"] = QString::number(getzIndex());

      setChanged(true);
      emit regionChangeRequested(this, attrs);
    }
  }

  adjust();

  QGraphicsItem::mouseReleaseEvent(event);
}

void LayoutRegion::keyPressEvent( QKeyEvent * event )
{
  qWarning() << event << event->key();

  if ( event->key() == Qt::Key_Delete ||
       event->key() == Qt::Key_Backspace )
  {
    performDelete();
    event->accept();
  }
  else
  {
    QGraphicsItem::keyPressEvent(event);
  }
}

void LayoutRegion::keyReleaseEvent( QKeyEvent * event )
{
  QGraphicsItem::keyReleaseEvent(event);

  QMap <QString, QString> attrs = getAttributes();
  double value = 0.0;

  double step = event->modifiers().testFlag(Qt::ShiftModifier) ?
        KEYBOARD_MOVE_REGION_STEP_SMALL : KEYBOARD_MOVE_REGION_STEP;

  qreal parentW, parentH;
  if(parentItem() != NULL)
  {
    parentW = ((LayoutRegion*)parentItem())->getWidth();
    parentH = ((LayoutRegion*)parentItem())->getHeight();
  }
  else
  {
    parentW = scene()->width();
    parentH = scene()->height();
  }

  if (isSelected())
  {
    switch (event->key())
    {
      case Qt::Key_Left:
        value = ( (x() - step) / parentW) * 100;
        ROUND_DOUBLE(value);
        if ((value + _relativeWidth * 100) > 100.0)
          value = (1.0 - _relativeWidth) * 100.0;
        attrs ["left"] = QString::number( value, 'f', 2) + "%";
        break;
      case Qt::Key_Right:
        value = ( (x() + step) / parentW) * 100;
        ROUND_DOUBLE(value);
        if ((value + _relativeWidth * 100) > 100.0)
          value = (1.0 - _relativeWidth) * 100.0;
        attrs ["left"] = QString::number( value, 'f', 2) + "%";
        break;
      case Qt::Key_Down:
        value = ( (y() + step) / parentH) * 100;
        ROUND_DOUBLE(value);
        if ((value + _relativeHeight * 100) > 100.0)
          value = (1.0 - _relativeHeight) * 100.0;
        attrs ["top"] = QString::number( value, 'f', 2) + "%";
        break;
      case Qt::Key_Up:
        value = ( (y() - step) / parentH) * 100;
        ROUND_DOUBLE(value);
        if ((value + _relativeHeight * 100) > 100.0)
          value = (1.0 - _relativeHeight) * 100.0;
        attrs ["top"] = QString::number( value, 'f', 2) + "%";
        break;
      default:
        break;
    }

    event->accept();
    emit regionChangeRequested(this, attrs);
  }
}

void LayoutRegion::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  QGraphicsItem::contextMenuEvent(event);

  if (!event->isAccepted())
  {
    emit regionSelectionRequested(this);

    _contextMenu->exec(event->screenPos());

    event->accept();
  }
}

void LayoutRegion::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
  if (event->mimeData()->hasFormat("nclcomposer/mediaid"))
  {
    _isDragging = true;
    event->acceptProposedAction();

    scene()->update(); // FIXME: Can we do better?
  }
}

void LayoutRegion::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
  Q_UNUSED(event)

  _isDragging = false;

  scene()->update();// FIXME: Can we do better?
}

void LayoutRegion::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  _isDragging = false;

  scene()->update();

  emit dragMediaOverRegion(event->mimeData()->data("nclcomposer/mediaid"),
                           this);


}

QMap <QString, QString> LayoutRegion::getAttributes()
{
  // setting
  QMap<QString, QString> full;

  double value = 0.0;
  full["id"] = getId();
  full["color"] = getColor();

  value = getRelativeTop()*100;
  ROUND_DOUBLE(value);
  full["top"] = QString::number(value, 'f', 2) + "%";

  value = getRelativeLeft()*100;
  ROUND_DOUBLE(value);
  full["left"] = QString::number(value, 'f', 2) + "%";

  value = getRelativeWidth()*100;
  ROUND_DOUBLE(value);
  full["width"] = QString::number(value, 'f', 2) + "%";

  value = getRelativeHeight()*100;
  ROUND_DOUBLE(value);
  full["height"] = QString::number(value, 'f', 2) + "%";

  value = getzIndex();
  full["zIndex"] = QString::number(value);

  return full;
}

QMap <QString, QString> LayoutRegion::getCloneAttributes()
{
  return _cloneAttributes;
}
