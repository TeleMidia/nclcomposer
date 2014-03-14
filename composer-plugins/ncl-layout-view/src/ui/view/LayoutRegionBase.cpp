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
#include "LayoutRegionBase.h"

LayoutRegionBase::LayoutRegionBase( QObject* parent, QMenu* switchMenu )
  : QGraphicsScene(parent)
{
  this->switchMenu = switchMenu;

  this->switchMenu->setEnabled(true);

  createActions();
  createMenus();
  createConnections();

  graphicsRegionBaseId = NULL;

  selectedRegion = NULL;

  setSceneRect(0,0,854,480);

  int w = 854, h = 480;
  bgrect = new QGraphicsRectItem(QRect(0, 0, w, h));

  this->addItem(bgrect);

  // bgrect->setBrush(QBrush(QPixmap(":/bg/layout")));
  bgrect->setBrush(QBrush(Qt::white));
  bgrect->setPen(QPen(QColor("#BBBBBB")));

  bgrect->setZValue(-1);

  createGrid();
  createSafeArea();
}

LayoutRegionBase::~LayoutRegionBase()
{

}

QString LayoutRegionBase::getId() const
{
  return id;
}

void LayoutRegionBase::setId(const QString &id)
{
  this->id = id;

  //  if(graphicsRegionBaseId == NULL)
  //    graphicsRegionBaseId = addText("regionBaseId");
  //  graphicsRegionBaseId->setPos(10,10);
  //  graphicsRegionBaseId->setToolTip(id);
  //  graphicsRegionBaseId->setPlainText(id);
}

QString LayoutRegionBase::getUid() const
{
  return uid;
}

void LayoutRegionBase::setUid(const QString &uid)
{
  this->uid = uid;
}

QString LayoutRegionBase::getRegion() const
{
  return region;
}

void LayoutRegionBase::setRegion(const QString &region)
{
  this->region = region;
}

QString LayoutRegionBase::getDevice() const
{
  return device;
}

void LayoutRegionBase::setDevice(const QString &device)
{
  this->device = device;
}

void LayoutRegionBase::selectRegion(LayoutRegion* region)
{
  if (!region->isSelected())
  {
    if (selectedRegion != NULL)
    {
      selectedRegion->setSelected(false);
    }

    region->setSelected(true);

    selectedRegion = region;
  }
}

void LayoutRegionBase::changeRegion(LayoutRegion* region,
                                    const QMap<QString, QString> attributes)
{
  if (region != NULL)
  {
    //        if (!attributes["id"].isEmpty()){
    region->setId(attributes["id"]);
    //        }

    //        if (!attributes["title"].isEmpty()){
    region->setTitle(attributes["title"]);
    //        }

    if (!attributes["color"].isEmpty())
      region->setColor(attributes["color"]);

    if (!attributes["top"].isEmpty())
    {
      if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["top"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal top = attribute.toDouble();

        if (top >= 0 && top <= 100)
          region->setRelativeTop(top/100);
      }
      else if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["top"];

        qreal top = attribute.toDouble();

        if (top >= 0 && top <= 1)
          region->setRelativeTop(top);
      }
    }

    if (!attributes["left"].isEmpty())
    {
      if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["left"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal left = attribute.toDouble();

        if (left >= 0 && left <= 100)
          region->setRelativeLeft(left/100);
      }
      else if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["left"];

        qreal left = attribute.toDouble();

        if (left >= 0 && left <= 1)
          region->setRelativeLeft(left);
      }
    }

    if (!attributes["right"].isEmpty())
    {
      if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["right"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal right = attribute.toDouble();

        if (right >= 0 && right <= 100)
          region->setRelativeRight(right/100);
      }
      else if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["right"];

        qreal right = attribute.toDouble();

        if (right >= 0 && right <= 1)
          region->setRelativeRight(right);
      }
    }

    if (!attributes["bottom"].isEmpty())
    {
      if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["bottom"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal bottom = attribute.toDouble();

        if (bottom >= 0 && bottom <= 100)
          region->setRelativeBottom(bottom/100);

      }
      else if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["bottom"];

        qreal bottom = attribute.toDouble();

        if (bottom >= 0 && bottom <= 1)
          region->setRelativeBottom(bottom);
      }
    }

    if (!attributes["width"].isEmpty())
    {
      if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["width"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal width = attribute.toDouble();

        if (width >= 0 && width <= 100)
          region->setRelativeWidth(width/100);

      }
      else if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["width"];

        qreal width = attribute.toDouble();

        if (width >= 0 && width <= 1)
          region->setRelativeWidth(width);
      }
    }

    if (!attributes["height"].isEmpty())
    {
      if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["height"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal height = attribute.toDouble();

        if (height >= 0 && height <= 100)
          region->setRelativeHeight(height/100);

      }
      else if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["height"];

        qreal height = attribute.toDouble();

        if (height >= 0 && height <= 1)
          region->setRelativeHeight(height);

      }
    }

    if(attributes.contains("zIndex"))
      region->setzIndex(attributes["zIndex"].toInt());

    region->adjust();
  }
}

void LayoutRegionBase::requestRegionChange(LayoutRegion* region,
                                           QMap<QString, QString> attributes)
{
  // setting
  QMap<QString, QString> full = region->getAttributes();

  // Overwrite parameter from attributes
  foreach(QString key, attributes.keys())
  {
    full[key] = attributes[key];
  }

  emit regionChangeRequested( region->getUid(), uid, full );
}

void LayoutRegionBase::requestRegionSelection(LayoutRegion* region)
{
  emit regionSelectionRequested(region->getUid(),uid);
}

void LayoutRegionBase::LayoutRegionBase::createActions()
{
  // help action
  helpAction = new QAction(this);
  helpAction->setText(tr("Help"));

  helpAction->setEnabled(true);
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

  pasteAction->setEnabled(true);
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

  re640x480 = new QAction(this);
  re640x480->setText(tr("640x480 (4:3)"));
  re640x480->setCheckable(true);
  re640x480->setChecked(false);

  re800x600 = new QAction(this);
  re800x600->setText(tr("800x600 (4:3)"));
  re800x600->setCheckable(true);
  re800x600->setChecked(false);

  re1024x768 = new QAction(this);
  re1024x768->setText(tr("1024x768 (4:3)"));
  re1024x768->setCheckable(true);
  re1024x768->setChecked(false);

  re854x480 = new QAction(this);
  re854x480->setText(tr("854x480 (16:9)"));
  re854x480->setCheckable(true);
  re854x480->setChecked(true);

  re1280x720 = new QAction(this);
  re1280x720->setText(tr("1280x720 (16:9)"));
  re1280x720->setCheckable(true);
  re1280x720->setChecked(false);

  re1920x1080 = new QAction(this);
  re1920x1080->setText(tr("1920x1080 (16:9)"));
  re1920x1080->setCheckable(true);
  re1920x1080->setChecked(false);

  re320x400 = new QAction(this);
  re320x400->setText(tr("320x400 (4:5)"));
  re320x400->setCheckable(true);
  re320x400->setChecked(false);

  // hide action
  hideAction = new QAction(this);
  hideAction->setText(tr("Hide"));

  hideAction->setEnabled(false);

  // Grid action
  gridAction = new QAction(this);
  gridAction->setText(tr("Grid"));
  gridAction->setEnabled(true);
  gridAction->setCheckable(true);
  gridAction->setChecked(false);

  // Safe area action
  _safeAreaAction = new QAction(this);
  _safeAreaAction->setText(tr("Safe area"));
  _safeAreaAction->setEnabled(true);
  _safeAreaAction->setCheckable(true);
  _safeAreaAction->setChecked(false);

  // properties action
  propertiesAction = new QAction(this);
  propertiesAction->setText(tr("Properties"));

  propertiesAction->setEnabled(true);

  regionActionGroup = new QActionGroup(this);
  regionActionGroup->setExclusive(false);

  screensizeGroup  = new QActionGroup(this);
  screensizeGroup->setExclusive(true);

  screensizeGroup->addAction(re640x480);
  screensizeGroup->addAction(re800x600);
  screensizeGroup->addAction(re1024x768);
  screensizeGroup->addAction(re854x480);
  screensizeGroup->addAction(re1280x720);
  screensizeGroup->addAction(re1920x1080);
  screensizeGroup->addAction(re320x400);
}

void LayoutRegionBase::createMenus()
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
  showMenu->addAction(gridAction);
  showMenu->addAction(_safeAreaAction);

  showMenu->setEnabled(true);

  // arrange menu
  arrangeMenu = new QMenu();
  arrangeMenu->setTitle(tr("Arrange"));

  arrangeMenu->setEnabled(false);

  arrangeMenu->addAction(bringfrontAction);
  arrangeMenu->addAction(bringforwardAction);
  arrangeMenu->addAction(sendbackwardAction);
  arrangeMenu->addAction(sendbackAction);

  // screensize menu
  screensizeMenu = new QMenu();
  screensizeMenu->setTitle(tr("Screen Size"));

  screensizeMenu->setEnabled(true);

  screensizeMenu->addAction(re640x480);
  screensizeMenu->addAction(re800x600);
  screensizeMenu->addAction(re1024x768);
  screensizeMenu->addAction(re854x480);
  screensizeMenu->addAction(re1280x720);
  screensizeMenu->addAction(re1920x1080);
  screensizeMenu->addAction(re320x400);

  // context menu
  contextMenu = new QMenu();
  // contextMenu->addAction(helpAction);
  // contextMenu->addSeparator();
  // contextMenu->addAction(undoAction);
  // contextMenu->addAction(redoAction);
  // contextMenu->addSeparator();
  // contextMenu->addAction(cutAction);
  // contextMenu->addAction(copyAction);
  contextMenu->addAction(pasteAction);
  contextMenu->addSeparator();
  contextMenu->addAction(deleteAction);
  contextMenu->addSeparator();
  contextMenu->addAction(exportAction);
  contextMenu->addSeparator();
  // contextMenu->addMenu(viewMenu); //disabled for while
  contextMenu->addMenu(insertMenu);
  contextMenu->addMenu(showMenu);
  contextMenu->addMenu(screensizeMenu);
  // contextMenu->addMenu(arrangeMenu); //disabled for while
  contextMenu->addSeparator();
  contextMenu->addAction(hideAction);
  contextMenu->addSeparator();
  contextMenu->addMenu(switchMenu);
  // contextMenu->addSeparator(); // disabled for while
  // contextMenu->addAction(propertiesAction); //disabled for while
}

void LayoutRegionBase::createConnections()
{
  connect(regionAction, SIGNAL(triggered()),
          SLOT(performRegion()));

  connect(regionbaseAction, SIGNAL(triggered()),
          SIGNAL(regionbasePerformed()));

  connect(regionActionGroup, SIGNAL(triggered(QAction*)),
          SLOT(performShow(QAction*)));

  connect(deleteAction, SIGNAL(triggered()),
          SLOT(performDelete()));

  re640x480->setData(QSize(640, 480));
  connect(re640x480, SIGNAL(triggered()), SLOT(performChangeResolution()));

  re800x600->setData(QSize(800, 600));
  connect(re800x600, SIGNAL(triggered()), SLOT(performChangeResolution()));

  re1024x768->setData(QSize(1024, 768));
  connect(re1024x768, SIGNAL(triggered()), SLOT(performChangeResolution()));

  re854x480->setData(QSize(854, 480));
  connect(re854x480, SIGNAL(triggered()), SLOT(performChangeResolution()));

  re1280x720->setData(QSize(1280, 720));
  connect(re1280x720, SIGNAL(triggered()), SLOT(performChangeResolution()));

  re1920x1080->setData(QSize(1920, 1080));
  connect(re1920x1080, SIGNAL(triggered()), SLOT(performChangeResolution()));

  re320x400->setData(QSize(320, 400));
  connect(re320x400, SIGNAL(triggered()), SLOT(performChangeResolution()));

  connect(exportAction, SIGNAL(triggered()), SLOT(performExport()));

  connect(gridAction, SIGNAL(triggered()), SLOT(performGrid()));
  connect(_safeAreaAction, SIGNAL(triggered()), SLOT(performSafeArea()));

  connect(pasteAction, SIGNAL(triggered()), SIGNAL(pasteRequested()));
}

void LayoutRegionBase::performShow(QAction* action)
{
  if (!action->isChecked())
    regions[regionActions.key(action)]->setVisible(false);
  else
    regions[regionActions.key(action)]->setVisible(true);
}

void LayoutRegionBase::requestAdditionRegion(LayoutRegion* parent)
{
  QMap<QString, QString> attributes;

  attributes["top"] = "10%";
  attributes["left"] = "10%";
  //    attributes["right"] = "10%";
  //    attributes["bottom"] = "10%";
  attributes["width"] = "80%";
  attributes["height"] = "80%";

  //Make the zIndex to be the greater one in the regionBase
  int zIndex = 0;
  foreach(LayoutRegion *region, regions.values())
  {
    zIndex = zIndex > region->getzIndex() ? zIndex : region->getzIndex() + 1;
  }
  attributes["zIndex"] = QString::number(zIndex);

  emit regionAdditionRequested("", parent->getUid(), uid, attributes);
}

void LayoutRegionBase::requestRegionDeletion(LayoutRegion* region)
{
  emit regionDeletionRequested(region->getUid(), uid);
}

void LayoutRegionBase::updateActionText(LayoutRegion *region)
{
  // Update Show Menu
  if(regionActions.contains(region->getUid()))
    regionActions[region->getUid()]->setText(region->getId());
}

void LayoutRegionBase::hideRegion(LayoutRegion* region)
{
  regionActions[region->getUid()]->trigger();
}

void LayoutRegionBase::removeRegion(LayoutRegion* region)
{
  if (region != NULL)
  {
    if (region->parentItem() != NULL)
    {
      LayoutRegion* parent =
          (LayoutRegion*) region->parentItem();

      foreach(QGraphicsItem* item, region->childItems())
      {
        LayoutRegion* child = dynamic_cast<LayoutRegion*> (item);

        if(child != NULL)
          regions.remove(child->getUid());
        else
          qWarning() << "Trying to remove an element that is not of\
                        the type QnlyGraphicsRegion " << __FILE__ << __LINE__;
      }

      parent->removeRegion(region);
    }
    else
    {
      foreach(QGraphicsItem* item, region->childItems())
      {
        LayoutRegion* child = dynamic_cast<LayoutRegion*> (item);

        if(child != NULL)
          regions.remove(child->getUid());
        else
          qWarning() << "Trying to remove an element that is not of the\
                        type QnlyGraphicsRegion " << __FILE__ << __LINE__;
      }

      if(regionActions.contains(region->getUid()))
      {
        QAction *action = regionActions[region->getUid()];

        showMenu->removeAction(action);

        regionActionGroup->removeAction(action);

        regionActions.remove(region->getUid());
      }
    }

    removeItem(region);

    regions.remove(region->getUid());

    //  delete(region);
    selectedRegion = NULL;

    emit regionBaseSelectionRequested(uid);
  }
}

QGraphicsItem* LayoutRegionBase::getBackgroundItem()
{
  return bgrect;
}

void LayoutRegionBase::performDelete()
{
  emit regionBaseDeletionRequested(uid);
}

void LayoutRegionBase::performRegion()
{
  QMap<QString, QString> attributes;

  attributes["top"] = "10%";
  attributes["left"] = "10%";
  attributes["right"] = "10%";
  attributes["bottom"] = "10%";
  attributes["width"] = "80%";
  attributes["height"] = "80%";

  //Make the zIndex to be the greater one in the regionBase
  int zIndex = 0;
  foreach(LayoutRegion *region, regions.values())
  {
    zIndex = zIndex > region->getzIndex() ? zIndex : region->getzIndex() + 1;
  }
  attributes["zIndex"] = QString::number(zIndex);

  emit regionAdditionRequested("", "", uid, attributes);
}

void LayoutRegionBase::addRegion(LayoutRegion* region,
                                 LayoutRegion* parent,
                                 const QMap<QString, QString> attributes)
{
  if (region != NULL)
  {
    /* changing */
    if (!attributes["id"].isEmpty())
      region->setId(attributes["id"]);

    if (!attributes["title"].isEmpty())
      region->setTitle(attributes["title"]);

    if (!attributes["color"].isEmpty())
      region->setColor(attributes["color"]);

    if (!attributes["top"].isEmpty())
    {
      if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["top"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal top = attribute.toDouble();

        if (top >= 0 && top <= 100)
          region->setRelativeTop(top/100);
      }
      else if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["top"];

        qreal top = attribute.toDouble();

        if (top >= 0 && top <= 1)
          region->setRelativeTop(top);
      }
    }

    if (!attributes["left"].isEmpty())
    {
      if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["left"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal left = attribute.toDouble();

        if (left >= 0 && left <= 100)
          region->setRelativeLeft(left/100);
      }
      else if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["left"];

        qreal left = attribute.toDouble();

        if (left >= 0 && left <= 1)
          region->setRelativeLeft(left);
      }
    }

    if (!attributes["right"].isEmpty())
    {
      if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["right"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal right = attribute.toDouble();

        if (right >= 0 && right <= 100)
          region->setRelativeRight(right/100);
      }
      else if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["right"];

        qreal right = attribute.toDouble();

        if (right >= 0 && right <= 1)
          region->setRelativeRight(right);
      }
    }

    if (!attributes["bottom"].isEmpty())
    {
      if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["bottom"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal bottom = attribute.toDouble();

        if (bottom >= 0 && bottom <= 100)
          region->setRelativeBottom(bottom/100);
      }
      else if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["bottom"];

        qreal bottom = attribute.toDouble();

        if (bottom >= 0 && bottom <= 1)
          region->setRelativeBottom(bottom);
      }
    }

    if (!attributes["width"].isEmpty())
    {
      if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["width"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal width = attribute.toDouble();

        if (width >= 0.0 &&  width <= 100.0)
          region->setRelativeWidth(width/100);
      }
      else if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["width"];

        qreal width = attribute.toDouble();

        if (width >= 0 && width <= 1)
          region->setRelativeWidth(width);
      }
    }

    if (!attributes["height"].isEmpty()){
      if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?%")))
      {
        QString attribute = attributes["height"];
        attribute.remove(attribute.length()-1,1); // removing '%'

        qreal height = attribute.toDouble();

        if (height >= 0 && height <= 100)
          region->setRelativeHeight(height/100);

      }
      else if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?")))
      {
        QString attribute = attributes["height"];

        qreal height = attribute.toDouble();

        if (height >= 0 && height <= 1)
          region->setRelativeHeight(height);
      }
    }

    if(attributes.contains("zIndex"))
      region->setzIndex(attributes["zIndex"].toInt());

    region->setGridAction(gridAction);

    if (parent != NULL)
    {
      parent->addRegion(region);
    }
    else
    {
      addItem(region);

      action = new QAction(this);
      action->setText(region->getId());

      showMenu->insertAction(showMenu->actions().front(), action);

      if (showMenu->actions().size() <= 2)
      {
        showMenu->insertSeparator(showMenu->actions().back());
      }

      action->setCheckable(true);
      action->setChecked(true);
      action->setEnabled(true);

      regionActionGroup->addAction(action);

      regionActions[region->getUid()] = action;
    }

    region->adjust();

    regions[region->getUid()] = region;

    connect(region,
            SIGNAL(regionSelectionRequested(LayoutRegion*)),
            SLOT(requestRegionSelection(LayoutRegion*)));

    connect(region,
            SIGNAL(regionChangeRequested(LayoutRegion*,QMap<QString,QString>)),
            SLOT(requestRegionChange(LayoutRegion*,QMap<QString,QString>)));

    connect(region,
            SIGNAL(regionbasePerformed()),
            SIGNAL(regionbasePerformed()));


    connect(region,
            SIGNAL(regionAdditionRequested(LayoutRegion*)),
            SLOT(requestAdditionRegion(LayoutRegion*)));

    connect(region,
            SIGNAL(regionDeletionRequested(LayoutRegion*)),
            SLOT(requestRegionDeletion(LayoutRegion*)));

    connect(region,
            SIGNAL(dragMediaOverRegion(QString,LayoutRegion*)),
            this,
            SLOT(requestMediaOverRegionAction(QString,LayoutRegion*))
            );

    connect(region,
            SIGNAL(copyRequested(LayoutRegion*)),
            SIGNAL(copyRequested(LayoutRegion*)));

    connect(region,
            SIGNAL(pasteRequested()),
            SIGNAL(pasteRequested()));

    // \fixme seg fault on Outline
    QMap<QString, QString> noChangeAtts;
    requestRegionChange(region, noChangeAtts);

    emit requestRegionSelection(region);
  }
}

void LayoutRegionBase::performChangeResolution()
{
  QAction* action = dynamic_cast<QAction*> (QObject::sender());
  if(action != NULL)
  {
    QSize size = action->data().toSize();
    changeResolution(size.width(), size.height());
  }
}

void LayoutRegionBase::changeResolution(int w, int h)
{
  setSceneRect(0, 0, w, h);
  bgrect->setRect(0, 0, w, h);

  updateSafeAreaPos();

  _grid->setRect(0, 0, w, h);

  foreach(LayoutRegion* r, regions.values())
    r->adjust();

  // TODO: This should be based on an array or a map!
  QSize size(w, h);
  if(re640x480->data().toSize() == size)
    re640x480->setChecked(true);
  else if(re800x600->data().toSize() == size)
    re800x600->setChecked(true);
  else if(re1024x768->data().toSize() == size)
    re1024x768->setChecked(true);
  else if(re854x480->data().toSize() == size)
    re854x480->setChecked(true);
  else if(re1280x720->data().toSize() == size)
    re1280x720->setChecked(true);
  else if(re1920x1080->data().toSize() == size)
    re1920x1080->setChecked(true);
  else if(re320x400->data().toSize() == size)
    re320x400->setChecked(true);
}

void LayoutRegionBase::performExport()
{
  QString location =
      QFileDialog::getSaveFileName(NULL, "Export...", "", tr("Images (*.png)"));

  if (location != "")
  {
    QImage image(width(), height(), QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&image);

    render(&painter, QRect(), QRect(0,0,width(),height()));

    painter.end();

    image.save(location, "PNG");
  }
}

void LayoutRegionBase::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  QGraphicsScene::mousePressEvent(event);

  if (!event->isAccepted())
  {
    if (event->button() == Qt::RightButton)
    {
      event->ignore();

    }
    else if (event->button() == Qt::LeftButton){
      emit regionBaseSelectionRequested(uid);
    }

    event->accept();
  }
}

void LayoutRegionBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if(selectedRegion != NULL)
  {
    selectedRegion->updateCursor(event);
  }

  QGraphicsScene::mouseMoveEvent(event);
}

void LayoutRegionBase::contextMenuEvent(
      QGraphicsSceneContextMenuEvent* event)
{
  QGraphicsScene::contextMenuEvent(event);

  if (!event->isAccepted())
  {
    emit regionBaseSelectionRequested(uid);

    contextMenu->exec(event->screenPos());

    event->accept();
  }
}

void LayoutRegionBase::requestMediaOverRegionAction(QString mediaId,
                                                    LayoutRegion* region)
{
  emit mediaOverRegion(mediaId, region->getUid());
}

void LayoutRegionBase::keyPressEvent( QKeyEvent * event )
{
  // CTRL+C - Copy
  /* if (event->modifiers() == Qt::ControlModifier &&
     event->key() == Qt::Key_C)
  {
    performCopy();
    event->accept();
  }
  // CTRL+V - Paste
  else */
  if (event->modifiers() == Qt::ControlModifier &&
      event->key() == Qt::Key_V)
  {
    emit pasteRequested();
    event->accept();
  }
  else
  {
    QGraphicsScene::keyPressEvent(event);
  }
}

void LayoutRegionBase::createGrid()
{
  _grid = new LayoutGrid(0, this);
  _grid->setStep(25);
  _grid->setPen(QPen(QBrush(Qt::lightGray), 1.5,Qt::DotLine));
  _grid->setRect(0,0,854,480);
  _grid->setZValue(1000);
  _grid->setVisible(false);

  this->addItem(_grid);
}

void LayoutRegionBase::performGrid()
{
  setGridVisible(!isGridVisible());
  emit gridVisibilityChanged(isGridVisible());
}

bool LayoutRegionBase::isGridVisible()
{
  return _grid->isVisible();
}

void LayoutRegionBase::setGridVisible(bool visible)
{
  gridAction->setChecked(visible);
  _grid->setVisible(visible);
}

QRect LayoutRegionBase::getSafeAreaRect(int w, int h,
                                        double w_perc, double h_perc)
{
  double safeArea_x1 = ((1-w_perc)*(double)w)/2;
  double safeArea_y1 = ((1-h_perc)*(double)h)/2;
  return QRect (safeArea_x1, safeArea_y1,
                w_perc * (double)w,
                h_perc * (double)h);
}

void LayoutRegionBase::createSafeArea()
{
  // Draw safe area
  _safeActionAreaRect = new QGraphicsRectItem( getSafeAreaRect( this->width(),
                                                                this->height(),
                                                                0.93, 0.93) );
  _safeActionAreaRect->setPen(QPen(QBrush(QColor(0, 0, 255)),
                             1.5, Qt::DashDotDotLine));
  _safeActionAreaRect->setBrush(QColor(203, 203, 255, 50));
  _safeActionAreaRect->setZValue(1000);
  _safeActionAreaRect->setVisible(false);

  _safeActionAreaText = new QGraphicsTextItem(tr("Safe Action Area"),
                                              _safeActionAreaRect);
  _safeActionAreaText->setZValue(1001);
  _safeActionAreaText->setParentItem(_safeActionAreaRect);
  _safeActionAreaText->setDefaultTextColor(QColor(100, 100, 255));
  _safeActionAreaText->setPos(_safeActionAreaRect->rect().x(),
                   _safeActionAreaRect->rect().height()+12);
  _safeActionAreaText->setFont(QFont("", 8));

  _safeTitleAreaRect = new QGraphicsRectItem( getSafeAreaRect( this->width(),
                                                               this->height(),
                                                               0.9, 0.9) );
  _safeTitleAreaRect->setPen(QPen(QBrush(QColor(255, 0, 0)),
                             1.5, Qt::DashDotDotLine));
  _safeTitleAreaRect->setBrush(QColor(255, 203, 203, 50));
  _safeTitleAreaRect->setZValue(1000);
  _safeTitleAreaRect->setVisible(false);

  _safeTitleAreaText = new QGraphicsTextItem(tr("Safe Title Area"),
                                             _safeTitleAreaRect);
  _safeTitleAreaText->setZValue(1001);
  _safeTitleAreaText->setParentItem(_safeTitleAreaRect);
  _safeTitleAreaText->setDefaultTextColor(QColor(255, 100, 100));
  _safeTitleAreaText->setPos(_safeTitleAreaRect->rect().x(),
                   _safeTitleAreaRect->rect().height()+5);
  _safeTitleAreaText->setFont(QFont("", 8));

  this->addItem(_safeTitleAreaRect);
  this->addItem(_safeActionAreaRect);
}

void LayoutRegionBase::updateSafeAreaPos()
{
  QRect newSafeActionRect = getSafeAreaRect( this->width(), this->height(),
                                            0.93, 0.93);
  _safeActionAreaRect->setRect( newSafeActionRect );
  _safeActionAreaText->setPos( newSafeActionRect.x(),
                               newSafeActionRect.y() +
                               newSafeActionRect.height() - 4);


  QRect newSafeTitleRect = getSafeAreaRect( this->width(), this->height(),
                                            0.90, 0.90);
  _safeTitleAreaRect->setRect(newSafeTitleRect);
  _safeTitleAreaText->setPos(newSafeTitleRect.x(),
                             newSafeTitleRect.y() +
                             newSafeTitleRect.height() - 15);
}

void LayoutRegionBase::performSafeArea()
{
  setSafeAreaVisible(!isSafeAreaVisible());
  emit safeAreaVisibilityChanged(isSafeAreaVisible());
}

bool LayoutRegionBase::isSafeAreaVisible()
{
  return _safeActionAreaRect->isVisible();
}

void LayoutRegionBase::setSafeAreaVisible(bool active)
{
  _safeActionAreaRect->setVisible(active);
  _safeTitleAreaRect->setVisible(active);
}
