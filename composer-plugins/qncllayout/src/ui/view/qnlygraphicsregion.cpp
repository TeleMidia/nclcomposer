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
#include "qnlygraphicsregion.h"

#include <QCursor>

QnlyGraphicsRegion::QnlyGraphicsRegion(QMenu* switchMenu,
                                       QnlyGraphicsRegion* parent)
  : QObject(parent), QGraphicsItem(parent)
{   
  this->switchMenu = switchMenu;

  this->switchMenu->setEnabled(true);

  /* creating */
  createActions();
  createMenus();
  createConnections();

  /* setting defaults */
  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsFocusable, true);

  setMoving(false);
  setResizing(false);
  setSelected(false);

  setValidated(true);

  setId("");
  setUid("");
  setTitle("");

  setRelativeTop(0);
  setRelativeLeft(0);
  setRelativeRight(0);
  setRelativeBottom(0);

  setRelativeWidth(1);
  setRelativeHeight(1);

  setzIndex(-1);
  setzIndexTop(0);

  setColor("#E4E4E4");
}

QnlyGraphicsRegion::~QnlyGraphicsRegion()
{
  delete(viewMenu);
  delete(insertMenu);
  delete(arrangeMenu);
  delete(contextMenu);
}

bool QnlyGraphicsRegion::isMoving() const
{
  return moving;
}

void QnlyGraphicsRegion::setParent(QnlyGraphicsRegion* region)
{
  QObject::setParent(region);
  setParentItem(region);
}

void QnlyGraphicsRegion::setMoving(bool moving)
{
  this->moving = moving;
  if(moving)
    setCursor(QCursor(Qt::ClosedHandCursor));
  else
    setCursor(QCursor(Qt::ArrowCursor));
}

bool QnlyGraphicsRegion::isResizing() const
{
  return resizing;
}

void QnlyGraphicsRegion::setResizing(bool resizing)
{
  this->resizing = resizing;
}

bool QnlyGraphicsRegion::isSelected() const
{
  return selected;
}

void QnlyGraphicsRegion::setSelected(bool selected)
{
  this->selected = selected;

  if(this->selected)
    setFocus(Qt::MouseFocusReason);
}

bool QnlyGraphicsRegion::isValidated() const
{
  return validated;
}

void QnlyGraphicsRegion::setValidated(bool validated)
{
  this->validated = validated;
}

QString QnlyGraphicsRegion::getId() const
{
  return id;
}

void QnlyGraphicsRegion::setId(const QString &id)
{
  this->id = id;

  setToolTip(title+" "+"("+id+")");

  if (parentItem() != NULL)
  {
    QnlyGraphicsRegion* parent = (QnlyGraphicsRegion*) parentItem();
    parent->updateActionText(this);
  }
  else
  {
    QnlyGraphicsRegionBase* s = (QnlyGraphicsRegionBase*) scene();

    if(s != NULL)
      s->updateActionText(this);
  }
}

QString QnlyGraphicsRegion::getUid() const
{
  return uid;
}

void QnlyGraphicsRegion::setUid(const QString &uid)
{
  this->uid = uid;
}

QString QnlyGraphicsRegion::getTitle() const
{
  return title;
}

void QnlyGraphicsRegion::setTitle(const QString &title)
{
  this->title = title;

  setToolTip(title+" "+"("+id+")");
}

QString QnlyGraphicsRegion::getColor() const
{
  return color;
}

void QnlyGraphicsRegion::setColor(const QString &color)
{
  this->color = color;
}

bool QnlyGraphicsRegion::isPainted() const
{
  return painted;
}

void QnlyGraphicsRegion::setPainted(bool painted)
{
  this->painted = painted;
}

void QnlyGraphicsRegion::setzIndex(int zindex)
{
  this->zindex = zindex;

  setZValue(zindex);
}

int QnlyGraphicsRegion::getzIndex() const
{
  return zindex;
}

void QnlyGraphicsRegion::setzIndexTop(qreal zindexTop)
{
  this->zindexTop = zindexTop;
}

qreal QnlyGraphicsRegion::getzIndexTop() const
{
  return zindexTop;
}

qreal QnlyGraphicsRegion::getTop() const
{
  return top;
}

void QnlyGraphicsRegion::setTop(qreal top)
{
  this->top = top;

  setY(top-4);
}

qreal QnlyGraphicsRegion::getMoveTop() const
{
  return moveTop;
}

void QnlyGraphicsRegion::setMoveTop(qreal moveTop)
{
  this->moveTop = moveTop;
}

qreal QnlyGraphicsRegion::getPressTop() const
{
  return pressTop;
}

void QnlyGraphicsRegion::setPressTop(qreal pressTop)
{
  this->pressTop = pressTop;
}

qreal QnlyGraphicsRegion::getResizeTop() const
{
  return resizeTop;
}

void QnlyGraphicsRegion::setResizeTop(qreal resizeTop)
{
  this->resizeTop = resizeTop;
}

qreal QnlyGraphicsRegion::getRelativeTop() const
{
  return relativeTop;
}

void QnlyGraphicsRegion::setRelativeTop(qreal relativeTop)
{
  this->relativeTop = relativeTop;
}

qreal QnlyGraphicsRegion::getLeft() const
{
  return left;
}

void QnlyGraphicsRegion::setLeft(qreal left)
{
  this->left = left;

  setX(left-4);
}

qreal QnlyGraphicsRegion::getMoveLeft() const
{
  return moveLeft;
}

void QnlyGraphicsRegion::setMoveLeft(qreal moveLeft)
{
  this->moveLeft = moveLeft;
}

qreal QnlyGraphicsRegion::getPressLeft() const
{
  return pressLeft;
}

void QnlyGraphicsRegion::setPressLeft(qreal pressLeft)
{
  this->pressLeft = pressLeft;
}

qreal QnlyGraphicsRegion::getResizeLeft() const
{
  return resizeLeft;
}

void QnlyGraphicsRegion::setResizeLeft(qreal resizeLeft)
{
  this->resizeLeft = resizeLeft;
}

qreal QnlyGraphicsRegion::getRelativeLeft() const
{
  return relativeLeft;
}

void QnlyGraphicsRegion::setRelativeLeft(qreal relativeLeft)
{
  this->relativeLeft = relativeLeft;
}

qreal QnlyGraphicsRegion::getRight() const
{
  return right;
}

void QnlyGraphicsRegion::setRight(qreal right)
{
  this->right = right;
}

qreal QnlyGraphicsRegion::getRelativeRight() const
{
  return relativeRight;
}

void QnlyGraphicsRegion::setRelativeRight(qreal relativeRight)
{
  this->relativeRight = relativeRight;
}

qreal QnlyGraphicsRegion::getBottom() const
{
  return bottom;
}

void QnlyGraphicsRegion::setBottom(qreal bottom)
{
  this->bottom = bottom;
}

qreal QnlyGraphicsRegion::getRelativeBottom() const
{
  return relativeBottom;
}

void QnlyGraphicsRegion::setRelativeBottom(qreal relativeBottom)
{
  this->relativeBottom = relativeBottom;
}

qreal QnlyGraphicsRegion::getWidth() const
{
  return width;
}

void QnlyGraphicsRegion::setWidth(qreal width)
{
  this->width = width;
}

qreal QnlyGraphicsRegion::getPressWidth() const
{
  return pressWidth;
}

void QnlyGraphicsRegion::setPressWidth(qreal pressWidth)
{
  this->pressWidth = pressWidth;
}

qreal QnlyGraphicsRegion::getResizeWidth() const
{
  return resizeWidth;
}

bool QnlyGraphicsRegion::hasChanged() const
{
  return changed;
}

void QnlyGraphicsRegion::setChanged(bool changed)
{
  this->changed = changed;
}

void QnlyGraphicsRegion::setResizeWidth(qreal resizeWidth)
{
  this->resizeWidth = resizeWidth;
}

qreal QnlyGraphicsRegion::getRelativeWidth() const
{
  return relativeWidth;
}

void QnlyGraphicsRegion::setRelativeWidth(qreal relativeWidth)
{
  this->relativeWidth = relativeWidth;
}

qreal QnlyGraphicsRegion::getHeight() const
{
  return height;
}

void QnlyGraphicsRegion::setHeight(qreal height)
{
  this->height = height;
}

qreal QnlyGraphicsRegion::getPressHeight() const
{
  return pressHeight;
}

void QnlyGraphicsRegion::setPressHeight(qreal pressHeight)
{
  this->pressHeight = pressHeight;
}

qreal QnlyGraphicsRegion::getResizeHeight() const
{
  return resizeHeight;
}

void QnlyGraphicsRegion::setResizeHeight(qreal resizeHeight)
{
  this->resizeHeight = resizeHeight;
}

qreal QnlyGraphicsRegion::getRelativeHeight() const
{
  return relativeHeight;
}

void QnlyGraphicsRegion::setRelativeHeight(qreal relativeHeight)
{
  this->relativeHeight = relativeHeight;
}

QnlyGraphicsRegion::QnlyResizeType QnlyGraphicsRegion::getResizeType() const
{
  return resizeType;
}

void QnlyGraphicsRegion::setResizeType(const QnlyResizeType &resizeType)
{
  this->resizeType = resizeType;
}

void QnlyGraphicsRegion::QnlyGraphicsRegion::createActions()
{
  // help action
  helpAction = new QAction(this);
  helpAction->setText(tr("Help"));

  helpAction->setEnabled(false);
  helpAction->setShortcut(QKeySequence("F1"));

  // undo action
  undoAction = new QAction(this);
  undoAction->setText(tr("Undo"));

  undoAction->setEnabled(false);
  undoAction->setShortcut(QKeySequence("Ctrl+Z"));

  // redo action
  redoAction = new QAction(this);
  redoAction->setText(tr("Redo"));

  redoAction->setEnabled(false);
  redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

  // cut action
  cutAction = new QAction(this);
  cutAction->setText(tr("Cut"));

  cutAction->setEnabled(false);
  cutAction->setShortcut(QKeySequence("Ctrl+X"));

  // copy action
  copyAction = new QAction(this);
  copyAction->setText(tr("Copy"));

  copyAction->setEnabled(false);
  copyAction->setShortcut(QKeySequence("Ctrl+C"));

  // paste action
  pasteAction = new QAction(this);
  pasteAction->setText(tr("Paste"));

  pasteAction->setEnabled(false);
  pasteAction->setShortcut(QKeySequence("Ctrl+V"));

  // delete action
  deleteAction = new QAction(this);
  deleteAction->setText(tr("Delete"));

  deleteAction->setEnabled(true);
  deleteAction->setShortcut(QKeySequence("Del"));

  // zoomin action
  zoominAction = new QAction(this);
  zoominAction->setText(tr("Zoom In"));

  zoominAction->setEnabled(true);
  zoominAction->setShortcut(QKeySequence("Ctrl++"));

  // zoomout action
  zoomoutAction = new QAction(this);
  zoomoutAction->setText(tr("Zoom Out"));

  zoomoutAction->setEnabled(true);
  zoomoutAction->setShortcut(QKeySequence("Ctrl+-"));

  // reset action
  zoomresetAction = new QAction(this);
  zoomresetAction->setText(tr("Reset"));

  zoomresetAction->setEnabled(true);
  zoomresetAction->setShortcut(QKeySequence("Ctrl+0"));

  // fullscreen action
  fullscreenAction = new QAction(this);
  fullscreenAction->setText(tr("Full Screen"));

  fullscreenAction->setEnabled(true);
  fullscreenAction->setShortcut(QKeySequence("F11"));

  // export action
  exportAction = new QAction(this);
  exportAction->setText(tr("Export..."));

  exportAction->setEnabled(true);

  // region action
  regionAction = new QAction(this);
  regionAction->setText(tr("Region"));

  regionAction->setEnabled(true);

  // regionbase action
  regionbaseAction = new QAction(this);
  regionbaseAction->setText(tr("Regionbase"));

  regionbaseAction->setEnabled(true);

  // bring to front action
  bringfrontAction = new QAction(this);
  bringfrontAction->setText(tr("Bring to Front"));

  bringfrontAction->setEnabled(false);
  bringfrontAction->setShortcut(QKeySequence("Shift+Ctrl+]"));

  // bring forward action
  bringforwardAction = new QAction(this);
  bringforwardAction->setText(tr("Bring Forward"));

  bringforwardAction->setEnabled(false);
  bringforwardAction->setShortcut(QKeySequence("Ctrl+]"));

  // send backward action
  sendbackwardAction = new QAction(this);
  sendbackwardAction->setText(tr("Send Backward"));

  sendbackwardAction->setEnabled(false);
  sendbackwardAction->setShortcut(QKeySequence("Ctrl+["));

  // send to back action
  sendbackAction = new QAction(this);
  sendbackAction->setText(tr("Send to Back"));

  sendbackAction->setEnabled(false);
  sendbackAction->setShortcut(QKeySequence("Shift+Ctrl+["));

  // hide action
  hideAction = new QAction(this);
  hideAction->setText(tr("Hide"));

  hideAction->setEnabled(true);

  // properties action
  propertiesAction = new QAction(this);
  propertiesAction->setText(tr("Properties"));

  propertiesAction->setEnabled(true);

  regionActionGroup = new QActionGroup(this);
  regionActionGroup->setExclusive(false);

  setAcceptDrops(true);
}

void QnlyGraphicsRegion::setGridAction(QAction* action)
{
  gridAction = action;

  showMenu->addAction(gridAction);

}

void QnlyGraphicsRegion::createMenus()
{
  // view menu
  viewMenu = new QMenu();
  viewMenu->setTitle(tr("View"));

  viewMenu->setEnabled(true);

  viewMenu->addAction(zoominAction);
  viewMenu->addAction(zoomoutAction);
  viewMenu->addAction(zoomresetAction);
  viewMenu->addSeparator();
  viewMenu->addAction(fullscreenAction);

  // insert menu
  insertMenu = new QMenu();
  insertMenu->setTitle(tr("Insert"));

  insertMenu->setEnabled(true);

  insertMenu->addAction(regionAction);
  insertMenu->addAction(regionbaseAction);

  // show menu
  showMenu = new QMenu();
  showMenu->setTitle(tr("Show"));

  showMenu->setEnabled(true);

  // arrange menu
  arrangeMenu = new QMenu();
  arrangeMenu->setTitle(tr("Arrange"));

  arrangeMenu->setEnabled(false);

  arrangeMenu->addAction(bringfrontAction);
  arrangeMenu->addAction(bringforwardAction);
  arrangeMenu->addAction(sendbackwardAction);
  arrangeMenu->addAction(sendbackAction);

  // context menu
  contextMenu = new QMenu();
  contextMenu->addAction(helpAction);
  contextMenu->addSeparator();
  //    contextMenu->addAction(undoAction);
  //    contextMenu->addAction(redoAction);
  //    contextMenu->addSeparator();
  //    contextMenu->addAction(cutAction);
  //    contextMenu->addAction(copyAction);
  //    contextMenu->addAction(pasteAction);
  //    contextMenu->addSeparator();
  contextMenu->addAction(deleteAction);
  contextMenu->addSeparator();
  contextMenu->addAction(exportAction);
  contextMenu->addSeparator();
  // contextMenu->addMenu(viewMenu); // disabled for while
  contextMenu->addMenu(insertMenu);
  contextMenu->addMenu(showMenu);
  // contextMenu->addMenu(arrangeMenu); // disabled for while
  contextMenu->addSeparator();
  contextMenu->addAction(hideAction);
  contextMenu->addSeparator();
  contextMenu->addMenu(switchMenu);
  // contextMenu->addSeparator(); //disabled for now
  // contextMenu->addAction(propertiesAction); //disabled for now
}

void QnlyGraphicsRegion::createConnections()
{
  connect(regionActionGroup, SIGNAL(triggered(QAction*)),
          SLOT(performShow(QAction*)));

  connect(regionbaseAction, SIGNAL(triggered()),
          SIGNAL(regionbasePerformed()));

  connect(regionAction, SIGNAL(triggered()),
          SLOT(performRegion()));

  connect(hideAction, SIGNAL(triggered()),
          SLOT(performHide()));

  connect(deleteAction, SIGNAL(triggered()),
          SLOT(performDelete()));

  connect(exportAction, SIGNAL(triggered()), SLOT(performExport()));
}

void QnlyGraphicsRegion::updateActionText(QnlyGraphicsRegion *region)
{
  // Update Show Menu
  if(regionActions.contains(region->getUid()))
    regionActions[region->getUid()]->setText(region->getId());
}

void QnlyGraphicsRegion::hideRegion(QnlyGraphicsRegion* region)
{
  regionActions[region->getUid()]->trigger();
}

void QnlyGraphicsRegion::performDelete()
{
  emit regionDeletionRequested(this);
}

void QnlyGraphicsRegion::performExport()
{
  QString location = QFileDialog::getSaveFileName(NULL, "Export...", "", tr("Images (*.png)"));

  if (location != ""){

    qreal w = getWidth()+8;
    qreal h = getHeight()+8;

    QImage image(w, h, QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&image);

    QPointF p = mapToScene(0,0);

    scene()->render(&painter, QRect(), QRect(p.x(),p.y(),w,h));

    painter.end();

    image.save(location, "PNG");
  }
}

void QnlyGraphicsRegion::performHide()
{
  if (parentItem() != NULL)
  {
    QnlyGraphicsRegion* parent = (QnlyGraphicsRegion*) parentItem();

    parent->hideRegion(this);
  }
  else
  {
    QnlyGraphicsRegionBase* s = (QnlyGraphicsRegionBase*) scene();

    s->hideRegion(this);
  }
}

void QnlyGraphicsRegion::performRegion()
{
  emit regionAdditionRequested(this);
}

void QnlyGraphicsRegion::performShow(QAction* action)
{
  if (!action->isChecked())
  {
    regions[regionActions.key(action)]->setVisible(false);
  }
  else
  {
    regions[regionActions.key(action)]->setVisible(true);
  }
}

void QnlyGraphicsRegion::removeRegion(QnlyGraphicsRegion* region)
{
  if(regionActions.contains(region->getUid()))
  {
    QAction *action = regionActions[region->getUid()];

    showMenu->removeAction(action);

    regionActionGroup->removeAction(action);
  }

  regionActions.remove(region->getUid());
}

void QnlyGraphicsRegion::addRegion(QnlyGraphicsRegion* region)
{
  if (region != NULL)
  {
    region->setParent(this);

    region->setGridAction(gridAction);

    regions[region->getUid()] = region;

    QAction* action = new QAction(this);
    action->setText(region->getId());

    showMenu->addAction(action);

    showMenu->insertAction(showMenu->actions().front(), action);

    if (showMenu->actions().size() <= 2){
      showMenu->insertSeparator(showMenu->actions().back());
    }

    action->setCheckable(true);
    action->setChecked(true);
    action->setEnabled(true);

    regionActionGroup->addAction(action);

    regionActions[region->getUid()] = action;
  }
}

void QnlyGraphicsRegion::move(QGraphicsSceneMouseEvent* event)
{
  /* setting */
  qreal x = left;
  qreal y = top;

  /* setting minimal position */
  qreal minx;
  qreal miny;

  if (parentItem() != NULL)
  {
    minx = 4;
    miny = 4;
  }
  else
  {
    minx = 0;
    miny = 0;
  }

  /* setting maximal position */
  qreal maxx;
  qreal maxy;

  if (parentItem() != NULL)
  {
    maxx = parentItem()->boundingRect().width() - width - 4;
    maxy = parentItem()->boundingRect().height() - height - 4;
  }
  else
  {
    maxx = scene()->width() - width;
    maxy = scene()->height() - height;
  }

  /* setting delta */
  qreal dx = event->pos().x() - pressLeft; // (x1 - x0)
  qreal dy = event->pos().y() - pressTop;  // (y1 - y0)

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

void QnlyGraphicsRegion::resize(QGraphicsSceneMouseEvent* event)
{
  /* setting bounds */
  qreal x = left;
  qreal y = top;
  qreal w = width;
  qreal h = height;

  /* setting minimal bounds */
  qreal minx;
  qreal miny;
  qreal minw;
  qreal minh;

  if (parentItem() != NULL)
  {
    minx = 4;
    miny = 4;
    minw = -1; // not used
    minh = -1; // not used
  }
  else
  {
    minx = 0;
    miny = 0;
    minw = -1; // not used
    minh = -1; // not used
  }

  /* setting maximal bounds */
  qreal maxx;
  qreal maxy;
  qreal maxw;
  qreal maxh;

  if (parentItem() != NULL)
  {
    maxx = parentItem()->boundingRect().width() - width - 4;
    maxy = parentItem()->boundingRect().height() - height - 4;
    maxw = parentItem()->boundingRect().width() - 4;
    maxh = parentItem()->boundingRect().height() - 4;
  }
  else
  {
    maxx = scene()->width() - width;
    maxy = scene()->height() - height;
    maxw = scene()->width();
    maxh = scene()->height();
  }

  /* setting delta */
  qreal dx = event->pos().x() - pressLeft;    // (x1 - x0)
  qreal dy = event->pos().y() - pressTop;     // (y1 - y0)
  qreal dw = -dx;
  qreal dh = -dy;

  /* setting next bounds */
  qreal nextx = x + dx;
  qreal nexty = y + dy;
  qreal nextw = w + dw;
  qreal nexth = h + dh;

  /* adjusting */
  switch(resizeType)
  {
    /* adjusting TOPLEFT */
    case QnlyGraphicsRegion::TopLeft:
    {
      if (nextx < minx)
      {
        nextx = minx;
        nextw = x + w - minx;
      }

      if (nexty < miny)
      {
        nexty = miny;
        nexth = y + h - miny;
      }
      break;
    }
    /* adjusting TOP */
    case QnlyGraphicsRegion::Top:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width

      if (nexty < miny)
      {
        nexty = miny;
        nexth = y + h - miny;
      }

      break;
    }

    /* adjusting TOPRIGHT */
    case QnlyGraphicsRegion::TopRight:
    {
      nextx = x; // fixed x

      nextw = w - dw;
      if (x + nextw > maxw)
      {
        nextw = maxw - x;
      }

      if (nexty < miny)
      {
        nexty = miny;
        nexth = y + h - miny;
      }

      break;
    }

    /* adjusting RIGHT */
    case QnlyGraphicsRegion::Right:
    {
      nextx = x; // fixed x

      nextw = w - dw;
      if (x + nextw > maxw)
      {
        nextw = maxw - x;
      }

      nexty = y; // fixed y
      nexth = h; // fixed height

      break;
    }

    /* adjusting BOTTOMRIGHT */
    case QnlyGraphicsRegion::BottomRight:
    {
      nextx = x; // fixed x

      nextw = w - dw;
      if (x + nextw > maxw){
        nextw = maxw - x;
      }

      nexty = y; // fixed y

      nexth = h - dh;
      if (y + nexth > maxh)
      {
        nexth = maxh - y;
      }

      break;
    }

    /* adjusting BOTTOM */
    case QnlyGraphicsRegion::Bottom:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width

      nexty = y; // fixed y

      nexth = h - dh;
      if (y + nexth > maxh)
      {
        nexth = maxh - y;
      }

      break;
    }

    /* adjusting BOTTOMLEFT */
    case QnlyGraphicsRegion::BottomLeft:
    {
      if (nextx < minx)
      {
        nextx = minx;
        nextw = x + w - minx;
      }

      nexty = y; // fixed y

      nexth = h - dh;
      if (y + nexth > maxh)
      {
        nexth = maxh - y;
      }

      break;
    }

    /* adjusting LEFT */
    case QnlyGraphicsRegion::Left:
    {
      if (nextx < minx)
      {
        nextx = minx;
        nextw = x + w - minx;
      }

      nexty = y; // fixed y
      nexth = h; // fixed height

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

void QnlyGraphicsRegion::adjust(bool repaint)
{
  if (parentItem() != NULL)
  {
    QnlyGraphicsRegion* item = (QnlyGraphicsRegion*) parentItem();

    setTop(qRound(item->getHeight()*relativeTop + 4));
    setLeft(qRound(item->getWidth()*relativeLeft + 4));

    setWidth(qRound(item->getWidth()*relativeWidth));
    setHeight(qRound(item->getHeight()*relativeHeight));

    setRight(item->getWidth() - (left+width));      // not use to paint
    setBottom(item->getHeight() - (top+height));    // not use to paint

  }
  else
  {
    setTop(qRound(scene()->height()*relativeTop));
    setLeft(qRound(scene()->width()*relativeLeft));

    setWidth(qRound(scene()->width()*relativeWidth));
    setHeight(qRound(scene()->height()*relativeHeight));

    setRight(scene()->width() - (left+width));      // not use to paint
    setBottom(scene()->height() - (top+height));    // not use to paint
  }

  for (int i=0;i<childItems().size();++i)
  {
    QnlyGraphicsRegion* item = (QnlyGraphicsRegion*) childItems().at(i);

    item->adjust(false);
  }

  if (repaint)
  {
    scene()->update();
  }
}

QPainterPath QnlyGraphicsRegion::shape() const
{
  QPainterPath path;

  path.addRect(4, 4, width, height);

  if (selected)
  {
    path.setFillRule(Qt::WindingFill);

    path.addRect(0,0,8,8);                                  // topleft
    path.addRect((width+8)/2 - 4,0,8,8);                    // top
    path.addRect((width+8) - 8,0,8,8);                      // topright
    path.addRect((width+8) - 8,(height+8)/2 - 4,8,8);       // right
    path.addRect((width+8) - 8,(height+8) - 8,8,8);         // bottomright
    path.addRect((width+8)/2 - 4,(height+8) - 8,8,8);       // bottom
    path.addRect(0,(height+8) - 8,8,8);                     // bottomleft
    path.addRect(0,(height+8)/2 - 4,8,8);                   // left
  }

  return path;
}

QRectF QnlyGraphicsRegion::boundingRect() const
{
  QRectF bounds;

  bounds.setX(0);
  bounds.setY(0);
  bounds.setWidth(width+8);
  bounds.setHeight(height+8);

  return bounds;
}

void QnlyGraphicsRegion::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *option,
                               QWidget *widget)
{
  if (selected)
  {
    painter->setBrush(QColor(color));
    painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));  // 0px = cosmetic border
    painter->drawRect(4,4,width-1,height-1);
  }
  else
  {
    painter->setBrush(QColor(color));
    painter->setPen(QPen(QBrush(Qt::black), 0));                // 0px = cosmetic border
    painter->drawRect(4,4,width-1,height-1);
  }

  if (moving)
  {
    if (parentItem() != NULL)
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(QPen(QBrush(Qt::black), 0));            // 0px = cosmetic border
      painter->drawRect(moveLeft+4-left,moveTop+4-top,width-1,height-1);
    }
    else
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(QPen(QBrush(Qt::black), 0));            // 0px = cosmetic border
      painter->drawRect(moveLeft+4-left,moveTop+4-top,width-1,height-1);
    }
  }
  else if (resizing)
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0));                // 0px = cosmetic border
    painter->drawRect(resizeLeft+4-left,resizeTop+4-top,resizeWidth-1,resizeHeight-1);
  }
  else if (selected)
  {
    painter->setBrush(QBrush(Qt::white));
    painter->setPen(QPen(QBrush(QBrush(Qt::black)), 0));        // 0px = cosmetic border

    painter->drawRect(0,0,8,8);                                 // topleft
    painter->drawRect((width+8)/2-4-1,0,8,8);                   // top
    painter->drawRect((width+8)-8-1,0,8,8);                     // topright
    painter->drawRect((width+8)-8-1,(height+8)/2-4-1,8,8);      // right
    painter->drawRect((width+8)-8-1,(height+8)-8-1,8,8);        // bottomright
    painter->drawRect((width+8)/2-4-1,(height+8)-8-1,8,8);      // bottom
    painter->drawRect(0,(height+8)-8-1,8,8);                    // bottomleft
    painter->drawRect(0,(height+8)/2-4-1,8,8);                  // left
  }

  QString text = "";

  if (width >= 25 && height >= 25 && id != "")
  {
    text = title+" "+"("+id+")";
  }

  painter->drawText(4+6,4+6,width-1-4-6,height-1-4-6,Qt::AlignLeft, text);
}

void QnlyGraphicsRegion::updateCursor(QGraphicsSceneMouseEvent* event)
{
  Qt::CursorShape newShape;
  if(!resizing && !moving)
  {
    QPointF pos = mapFromScene(event->scenePos());

    // in the middle (UP or DOWN)
    if (QRectF((width+8)/2 - 4,0,8,8).contains(pos) ||
        QRectF((width+8)/2 - 4,(height+8) - 8,8,8).contains(pos))
    {
      newShape = Qt::SizeVerCursor;
    }

    // TOPLEFT or BOTTOM RIGHT
    else if (QRectF(0,0,8,8).contains(pos) ||
             QRectF((width+8) - 8,(height+8) - 8,8,8).contains(pos))

    {
      newShape = Qt::SizeFDiagCursor;
    }

    // TOPRIGHT or BOTTOMLEFT
    else if (QRectF((width+8) - 8,0,8,8).contains(pos) ||
             QRectF(0,(height+8) - 8,8,8).contains(pos))
    {
      newShape = Qt::SizeBDiagCursor;
    }

    // RIGHT OR LEFT
    else if (QRectF((width+8)-8-1,(height+8)/2-4-1,8,8).contains(pos) ||
             QRectF(0,(height+8)/2-4-1,8,8).contains(pos))
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

void QnlyGraphicsRegion::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  if (moving)
  {
    move(event);
  }
  else if (resizing)
  {
    resize(event);
  }
}

void QnlyGraphicsRegion::mousePressEvent(QGraphicsSceneMouseEvent* event)
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
    setPressWidth(width);
    setPressHeight(height);

    /* avoid flickering on first move */
    setMoveTop(top);
    setMoveLeft(left);

    /* avoid flickering on first resize */
    setResizeTop(top);
    setResizeLeft(left);
    setResizeWidth(width);
    setResizeHeight(height);

    /* if over TOPLEFT resize region */
    if (QRectF(0,0,8,8).contains(event->pos()))
    {
      setResizeType(QnlyGraphicsRegion::TopLeft);
      setResizing(true);
    }
    /* if over TOP resize region */
    else if (QRectF((width+8)/2 - 4,0,8,8).contains(event->pos()))
    {
      setResizeType(QnlyGraphicsRegion::Top);
      setResizing(true);
    }
    /* if over TOPRIGHT resize region */
    else if (QRectF((width+8) - 8,0,8,8).contains(event->pos()))
    {
      setResizeType(QnlyGraphicsRegion::TopRight);
      setResizing(true);
    }
    /* if over RIGHT resize region */
    else if (QRectF((width+8) - 8,(height+8)/2 - 4,8,8).contains(event->pos()))
    {
      setResizeType(QnlyGraphicsRegion::Right);
      setResizing(true);
    }
    /* if over BOTTOMRIGHT resize region */
    else if (QRectF((width+8) - 8,(height+8) - 8,8,8).contains(event->pos()))
    {
      setResizeType(QnlyGraphicsRegion::BottomRight);
      setResizing(true);
    }
    /* if over BOTTOM resize region */
    else if (QRectF((width+8)/2 - 4,(height+8) - 8,8,8).contains(event->pos()))
    {
      setResizeType(QnlyGraphicsRegion::Bottom);
      setResizing(true);
    }
    /* if over BOTTOMLEFT resize region */
    else if (QRectF(0,(height+8) - 8,8,8).contains(event->pos()))
    {
      setResizeType(QnlyGraphicsRegion::BottomLeft);
      setResizing(true);
    }
    /* if over LEFT resize region */
    else if (QRectF(0,(height+8)/2 - 4,8,8).contains(event->pos()))
    {
      setResizeType(QnlyGraphicsRegion::Left);
      setResizing(true);
    }
    /* if not over any resize region */
    else
    {
      setMoving(true);
    }
  }

  event->accept();

  QGraphicsItem::mousePressEvent(event);
}

void QnlyGraphicsRegion::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  if (moving)
  {
    setMoving(false);

    if ((top != moveTop || left != moveLeft))
    {
      /*
            setTop(moveTop);
            setLeft(moveLeft);

            if (parentItem() != NULL){
                QnlyGraphicsRegion* item = (QnlyGraphicsRegion*) parentItem();

                setRelativeTop((top-4)/item->getHeight());
                setRelativeLeft((left-4)/item->getWidth());

                setRelativeWidth(width/item->getWidth());
                setRelativeHeight(height/item->getHeight());

                setRelativeRight(1 - (relativeLeft+relativeWidth));
                setRelativeBottom(1 - (relativeTop+relativeHeight));

            }else{
                setRelativeTop(top/scene()->height());
                setRelativeLeft(left/scene()->width());

                setRelativeWidth(width/scene()->width());
                setRelativeHeight(height/scene()->height());

                setRelativeRight(1 - (relativeLeft+relativeWidth));
                setRelativeBottom(1 - (relativeTop+relativeHeight));
            }
*/
      QMap<QString, QString> attributes;

      double value = 0.0;
      if (parentItem() != NULL)
      {
        QnlyGraphicsRegion* item = (QnlyGraphicsRegion*) parentItem();

        value = ((moveTop-4)/item->getHeight()) * 100;
        ROUND_DOUBLE(value);
        attributes["top"] = QString::number(value, 'f', 2) + "%";

        value = ((moveLeft-4)/item->getWidth()) * 100;
        ROUND_DOUBLE(value);
        attributes["left"] = QString::number(value, 'f', 2) + "%";

        value = (1 - (((moveLeft-4)/item->getWidth())+(width/item->getWidth()))) * 100;
        ROUND_DOUBLE(value);
        attributes["right"] = QString::number(value, 'f', 2) + "%";

        value = (1 - (((moveTop-4)/item->getHeight())+(height/item->getHeight())))*100;
        ROUND_DOUBLE(value);
        attributes["bottom"] = QString::number(value, 'f', 2) + "%";

      }
      else
      {
        value = ((moveTop)/scene()->height())*100;
        ROUND_DOUBLE(value);
        attributes["top"] = QString::number(value, 'f', 2) + "%";

        value = ((moveLeft)/scene()->width())*100;
        ROUND_DOUBLE(value);
        attributes["left"] = QString::number(value, 'f', 2) + "%";

        value = (1 - (((moveLeft)/scene()->width())+(width/scene()->width())))*100;
        ROUND_DOUBLE(value);
        attributes["right"] = QString::number(value, 'f', 2) + "%";

        value = (1 - (((moveTop)/scene()->height())+(height/scene()->height())))*100;
        ROUND_DOUBLE(value);
        attributes["bottom"] = QString::number(value, 'f', 2) + "%";

      }

      attributes["zIndex"] = QString::number(getzIndex());

      setChanged(true);

      emit regionChangeRequested(this,attributes);
    }
  }

  if (resizing)
  {
    setResizing(false);

    if ((top != resizeTop || left != resizeLeft ||
         width != resizeWidth || height != resizeHeight))
    {
      if (resizeTop > top + height)
      {
        setResizeHeight(resizeTop - (top + height));
        setResizeTop(top + height);
      }

      if (resizeLeft > left + width)
      {
        setResizeWidth(resizeLeft - (left + width));
        setResizeLeft(left + width);
      }

      if (resizeWidth < 0)
      {
        setResizeLeft(resizeLeft + resizeWidth);
        setResizeWidth(-resizeWidth);
      }

      if (resizeHeight < 0)
      {
        setResizeTop(resizeTop + resizeHeight);
        setResizeHeight(-resizeHeight);
      }
      /*
        setTop(resizeTop);
        setLeft(resizeLeft);
        setWidth(resizeWidth);
        setHeight(resizeHeight);

        if (parentItem() != NULL){
            QnlyGraphicsRegion* item = (QnlyGraphicsRegion*) parentItem();

            setRelativeTop((top-4)/item->getHeight());
            setRelativeLeft((left-4)/item->getWidth());
            setRelativeWidth(width/item->getWidth());
            setRelativeHeight(height/item->getHeight());

            setRelativeRight(1 - (relativeLeft+relativeWidth));
            setRelativeBottom(1 - (relativeTop+relativeHeight));

        }else{
            setRelativeTop(top/scene()->height());
            setRelativeLeft(left/scene()->width());
            setRelativeWidth(width/scene()->width());
            setRelativeHeight(height/scene()->height());

            setRelativeRight(1 - (relativeLeft+relativeWidth));
            setRelativeBottom(1 - (relativeTop+relativeHeight));
        }
      */

      QMap<QString, QString> attributes;

      double value = 0.0;
      if (parentItem() != NULL)
      {
        QnlyGraphicsRegion* item = (QnlyGraphicsRegion*) parentItem();

        value = ((resizeTop-4)/item->getHeight())*100;
        ROUND_DOUBLE(value);
        attributes["top"] = QString::number(value, 'f', 2) + "%";

        value = ((resizeLeft-4)/item->getWidth())*100;
        ROUND_DOUBLE(value);
        attributes["left"] = QString::number(value, 'f', 2) + "%";

        value = (resizeHeight/item->getHeight())*100;
        ROUND_DOUBLE(value);
        attributes["height"] = QString::number(value, 'f', 2) + "%";

        value = (resizeWidth/item->getWidth())*100;
        ROUND_DOUBLE(value);
        attributes["width"] = QString::number(value, 'f', 2) + "%";

        value = (1 - (((resizeLeft-4)/item->getWidth())+(resizeWidth/item->getWidth())))*100;
        ROUND_DOUBLE(value);
        attributes["right"] = QString::number(value, 'f', 2) + "%";

        value = (1 - (((resizeTop-4)/item->getHeight())+(resizeHeight/item->getHeight())))*100;
        ROUND_DOUBLE(value);
        attributes["bottom"] = QString::number(value, 'f', 2) + "%";
      }
      else
      {
        value = ((resizeTop)/scene()->height())*100;
        ROUND_DOUBLE(value);
        attributes["top"] = QString::number(value, 'f', 2) + "%";

        value = ((resizeLeft)/scene()->width())*100;
        ROUND_DOUBLE(value);
        attributes["left"] = QString::number(value, 'f', 2) + "%";

        value = (resizeHeight/scene()->height())*100;
        ROUND_DOUBLE(value);
        attributes["height"] = QString::number(value, 'f', 2) + "%";

        value = (resizeWidth/scene()->width())*100;
        ROUND_DOUBLE(value);
        attributes["width"] = QString::number(value, 'f', 2) + "%";

        value = (1 - (((resizeLeft)/scene()->width())+(resizeWidth/scene()->width())))*100;
        ROUND_DOUBLE(value);
        attributes["right"] = QString::number(value, 'f', 2) + "%";

        value = (1 - (((resizeTop)/scene()->height())+(resizeHeight/scene()->height())))*100;
        ROUND_DOUBLE(value);
        attributes["bottom"] = QString::number(value, 'f', 2) + "%";
      }

      attributes["zIndex"] = QString::number(getzIndex());

      setChanged(true);

      emit regionChangeRequested(this, attributes);
    }
  }

  adjust();

  QGraphicsItem::mouseReleaseEvent(event);
}

void QnlyGraphicsRegion::keyPressEvent( QKeyEvent * event )
{
  if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
  {
    performDelete();

    event->accept();
  }

  QGraphicsItem::keyPressEvent(event);
}

void QnlyGraphicsRegion::keyReleaseEvent( QKeyEvent * event )
{
  QGraphicsItem::keyReleaseEvent(event);
}


void QnlyGraphicsRegion::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  QGraphicsItem::contextMenuEvent(event);

  if (!event->isAccepted())
  {
    emit regionSelectionRequested(this);

    contextMenu->exec(event->screenPos());

    event->accept();
  }
}

void QnlyGraphicsRegion::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
  if (event->mimeData()->hasFormat("nclcomposer/mediaid"))
    event->acceptProposedAction();
}

void QnlyGraphicsRegion::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  qDebug() << "dropEvent " << event->mimeData()->data("nclcomposer/mediaid")
           << event->mimeData()->data("nclcomposer/qnstuid");


  emit dragMediaOverRegion(event->mimeData()->data("nclcomposer/mediaid"),
                           this);
}
