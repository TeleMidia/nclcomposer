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
#include "LayoutView.h"

LayoutView::LayoutView(QWidget* parent) :
  QStackedWidget(parent),
  gridVisibility(false)
{
  setWindowTitle("Qnly");

  createActions();
  createMenus();
  createConnections();

  nregions = 0;
  nregionbases = 0;

  _selectedRegion = NULL;
  _selectedRegionBase = NULL;
}

LayoutView::~LayoutView()
{

}

void LayoutView::createActions()
{
  // help action
  _helpAction = new QAction(this);
  _helpAction->setText(tr("Help"));

  _helpAction->setEnabled(false);
  _helpAction->setShortcut(QKeySequence("F1"));

  // undo action
  // undoAction = new QAction(this);
  // undoAction->setText(tr("Undo"));

  // undoAction->setEnabled(false);
  // undoAction->setShortcut(QKeySequence("Ctrl+Z"));

  // redo action
  _redoAction = new QAction(this);
  _redoAction->setText(tr("Redo"));

  _redoAction->setEnabled(false);
  _redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

  // cut action
  _cutAction = new QAction(this);
  _cutAction->setText(tr("Cut"));

  _cutAction->setEnabled(false);
  _cutAction->setShortcut(QKeySequence("Ctrl+X"));

  // copy action
  _copyAction = new QAction(this);
  _copyAction->setText(tr("Copy"));

  _copyAction->setEnabled(false);
  // copyAction->setShortcut(QKeySequence("Ctrl+C"));

  // paste action
  _pasteAction = new QAction(this);
  _pasteAction->setText(tr("Paste"));

  _pasteAction->setEnabled(true);
  // pasteAction->setShortcut(QKeySequence("Ctrl+V"));

  // delete action
  _deleteAction = new QAction(this);
  _deleteAction->setText(tr("&Delete"));
  _deleteAction->setIcon(QIcon(":/icon/minus"));

  _deleteAction->setEnabled(false);
  _deleteAction->setShortcut(QKeySequence("Del"));

  // zoomin action
  _zoominAction = new QAction(this);
  _zoominAction->setText(tr("Zoom In"));

  _zoominAction->setEnabled(false);
  _zoominAction->setShortcut(QKeySequence("Ctrl++"));

  // zoomout action
  _zoomoutAction = new QAction(this);
  _zoomoutAction->setText(tr("Zoom Out"));

  _zoomoutAction->setEnabled(false);
  _zoomoutAction->setShortcut(QKeySequence("Ctrl+-"));

  // reset action
  _zoomresetAction = new QAction(this);
  _zoomresetAction->setText(tr("Reset"));

  _zoomresetAction->setEnabled(false);
  _zoomresetAction->setShortcut(QKeySequence("Ctrl+0"));

  // fullscreen action
  _fullscreenAction = new QAction(this);
  _fullscreenAction->setText(tr("Full Screen"));

  _fullscreenAction->setEnabled(true);
  _fullscreenAction->setShortcut(QKeySequence("F11"));

  // export action
  _exportAction = new QAction(this);
  _exportAction->setText(tr("Export..."));

  _exportAction->setEnabled(false);

  // region action
  _regionAction = new QAction(this);
  _regionAction->setText(tr("Region"));
  _regionAction->setIcon(QIcon(":/icon/plus"));

  _regionAction->setEnabled(false);

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

  _hideAction->setEnabled(false);

  // properties action
  _propertiesAction = new QAction(this);
  _propertiesAction->setText(tr("Properties"));

  _propertiesAction->setEnabled(false);

  _regionbaseActionGroup = new QActionGroup(this);
  _regionbaseActionGroup->setExclusive(true);

  setMouseTracking(true);
}

void LayoutView::createMenus()
{
  // view menu
  _viewMenu = new QMenu();
  _viewMenu->setTitle(tr("View"));

  _viewMenu->setEnabled(false);

  _viewMenu->addAction(_zoominAction);
  _viewMenu->addAction(_zoomoutAction);
  _viewMenu->addAction(_zoomresetAction);
  _viewMenu->addSeparator();
  _viewMenu->addAction(_fullscreenAction);

  _showMenu = new QMenu();
  _showMenu->setTitle(tr("Show"));

  _showMenu->setEnabled(false);

  // arrange menu
  _arrangeMenu = new QMenu();
  _arrangeMenu->setTitle(tr("Arrange"));

  _arrangeMenu->setEnabled(false);

  _arrangeMenu->addAction(_bringfrontAction);
  _arrangeMenu->addAction(_bringforwardAction);
  _arrangeMenu->addAction(_sendbackwardAction);
  _arrangeMenu->addAction(_sendbackAction);

  // switch menu
  _switchMenu = new QMenu();
  _switchMenu->setTitle(tr("Current RegionBase"));

  _switchMenu->setEnabled(false);

  // context menu
  _contextMenu = new QMenu();
  _contextMenu->addAction(_regionAction);
  _contextMenu->addAction(_regionbaseAction);
  _contextMenu->addSeparator();
  _contextMenu->addAction(_deleteAction);
  _contextMenu->addSeparator();

  //  contextMenu->addSeparator();
  //  contextMenu->addAction(undoAction);
  //  contextMenu->addAction(redoAction);
  //  contextMenu->addSeparator();
  //  contextMenu->addAction(cutAction);
  //  contextMenu->addAction(copyAction);

  _contextMenu->addAction(_pasteAction);
  _contextMenu->addSeparator();
  _contextMenu->addAction(_exportAction);
  _contextMenu->addSeparator();
  _contextMenu->addMenu(_viewMenu);
  _contextMenu->addMenu(_showMenu);
  _contextMenu->addMenu(_arrangeMenu);
  _contextMenu->addSeparator();
  _contextMenu->addAction(_hideAction);
  _contextMenu->addSeparator();
  _contextMenu->addMenu(_switchMenu);
  _contextMenu->addSeparator();
  _contextMenu->addAction(_propertiesAction);
  _contextMenu->addSeparator();
  _contextMenu->addAction(_helpAction);
}

void LayoutView::createConnections()
{
  connect(_regionbaseActionGroup, SIGNAL(triggered(QAction*)),
          SLOT(performSwitch(QAction*)));

  connect(_regionbaseAction, SIGNAL(triggered()),SLOT(performRegionBase()));

  connect(_pasteAction, SIGNAL(triggered()), SLOT(performPaste()));
}

void LayoutView::requestRegionAddition(const QString regionUID,
                                       const QString parentUID,
                                       const QString regionbaseUID,
                                       QMap<QString, QString> attributes)
{
  // attributes["id"] = "rg" + QString::number(nregions+1);
  // attributes["id"] =

  emit regionAdded(regionUID, parentUID, regionbaseUID, attributes);
}

void LayoutView::requestRegionChange(const QString regionUID,
                                     const QString regionbaseUID,
                                     QMap<QString, QString> attributes)
{

  emit regionChanged(regionUID,
                     regionbaseUID,
                     attributes);
}

void LayoutView::performSwitch(QAction* action)
{
  emit regionBaseSelected(_regionbaseActions.key(action));
}

void LayoutView::performRegionBase()
{
  QMap<QString, QString> attributes;

  attributes["id"] = "rgbase" + QString::number(++nregionbases);

  emit regionBaseAdded(QUuid::createUuid().toString(), attributes);
}

void LayoutView::addRegion(const QString regionUID,
                           const QString parentUID,
                           const QString regionbaseUID,
                           const QMap<QString, QString> attributes)
{
  if (!_regions.contains(regionUID) &&
      _regionbases.contains(regionbaseUID))
  {
    LayoutRegion* region = new LayoutRegion(_switchMenu);

    if (regionUID.isEmpty())
      region->setUid(QUuid::createUuid().toString());
    else
      region->setUid(regionUID);

    LayoutRegion* parent = NULL;

    if (_regions.contains(parentUID))
      parent = _regions[parentUID];

    LayoutRegionBase* regionbase = _regionbases[regionbaseUID];

    nregions++;

    addRegion(region, parent, regionbase, attributes);
  }
}

void LayoutView::performCopy()
{
  if (this->_selectedRegion)
    performCopy(this->_selectedRegion);
}

void LayoutView::performCopy(LayoutRegion *region)
{
  // setting
  QMap<QString, QString> attr = region->getAttributes();

  copiedRegionAttrs.clear();
  foreach(QString key, attr.keys())
  {
    if(key != "id")
      copiedRegionAttrs[key] = attr[key];
  }

  qWarning () << copiedRegionAttrs;
}

void LayoutView::performCut()
{
  if (this->_selectedRegion)
    performCut(this->_selectedRegion);
}

void LayoutView::performCut(LayoutRegion *region)
{
  performCopy(region);
  if(region != NULL)
  {
    region->performDelete();
  }
}

void LayoutView::performPaste()
{
  QString selectedRegionUId = "";
  if(_selectedRegion != NULL)
  {
    selectedRegionUId = _selectedRegion->getUid();

    if(_selectedRegion->isCloning()) //being called by ctrl+mousepressmove,which clones region
    {
      //selectedRegion->getCloneAttributes();
      copiedRegionAttrs["top"] = _selectedRegion->getCloneAttributes()["top"];
      copiedRegionAttrs["left"] = _selectedRegion->getCloneAttributes()["left"];
      copiedRegionAttrs["right"] = _selectedRegion->getCloneAttributes()["right"];
      copiedRegionAttrs["bottom"] = _selectedRegion->getCloneAttributes()["bottom"];
      copiedRegionAttrs["zIndex"] = _selectedRegion->getCloneAttributes()["zIndex"];

      LayoutRegion* parent = (LayoutRegion*) _selectedRegion->parentItem();

      if(parent != NULL)
        selectedRegionUId = parent->getUid();
      else
        selectedRegionUId = _selectedRegionBase->getUid();
    }
  }
  qDebug() << "Perform paste inside: " << selectedRegionUId;

  QString selectedRegionBaseUId = "";
  if(_selectedRegionBase != NULL)
    selectedRegionBaseUId = _selectedRegionBase->getUid();

  requestRegionAddition("",
                        selectedRegionUId,
                        selectedRegionBaseUId,
                        copiedRegionAttrs);
}

void LayoutView::removeRegion(const QString regionUID,
                              const QString regionbaseUID)
{
  if (_regions.contains(regionUID) && _regionbases.contains(regionbaseUID))
  {
    removeRegion(_regions[regionUID],
                 _regionbases[regionbaseUID]);
  }
}

void LayoutView::changeRegion(const QString regionUID,
                              const QString regionbaseUID,
                              const QMap<QString, QString> attributes)
{
  if (_regions.contains(regionUID) && _regionbases.contains(regionbaseUID))
  {
    changeRegion(_regions[regionUID],
                 _regionbases[regionbaseUID],
                 attributes);
  }
}

void LayoutView::selectRegion(const QString regionUID,
                              const QString regionbaseUID)
{
  if (_regions.contains(regionUID) && _regionbases.contains(regionbaseUID))
  {
    selectRegion(_regions[regionUID],
                 _regionbases[regionbaseUID]);
  }
}

void LayoutView::addRegionBase(const QString regionbaseUID,
                               const QMap<QString, QString> attributes)
{
  if (!_regionbases.contains(regionbaseUID))
  {
    LayoutCanvas* canvas = new LayoutCanvas(this);

    LayoutRegionBase* regionbase =
        new LayoutRegionBase(canvas, _switchMenu);

    regionbase->setParent(canvas);

    canvas->setScene(regionbase);

    if (regionbaseUID.isEmpty())
    {
      regionbase->setUid(QUuid::createUuid().toString());
    }
    else{
      regionbase->setUid(regionbaseUID);
    }

    addRegionBase(regionbase, attributes);
  }
}

void LayoutView::removeRegionBase(const QString regionbaseUID)
{
  if (_regionbases.contains(regionbaseUID))
  {
    removeRegionBase(_regionbases[regionbaseUID]);
  }
}

void LayoutView::changeRegionBase(const QString regionbaseUID,
                                  const QMap<QString, QString> attributes)
{
  if (_regionbases.contains(regionbaseUID))
  {
    changeRegionBase(_regionbases[regionbaseUID], attributes);
  }
}

void LayoutView::selectRegionBase(const QString regionbaseUID)
{
  if (_regionbases.contains(regionbaseUID))
  {
    selectRegionBase(_regionbases[regionbaseUID]);
  }
  else
    qWarning() << tr("It is not possible to select regionBase ")
               << regionbaseUID;
}

void LayoutView::addRegion(LayoutRegion* region,
                           LayoutRegion* parent,
                           LayoutRegionBase* regionBase,
                           const QMap<QString, QString> attributes)
{
  if (region != NULL && regionBase != NULL)
  {
    qDebug() << "Adding Region" << attributes;

    _regions[region->getUid()] = region;

    regionBase->addRegion(region,
                          parent,
                          attributes);
  }
}

void LayoutView::removeRegion( LayoutRegion* region,
                               LayoutRegionBase* regionBase )
{
  if (region != NULL && regionBase != NULL)
  {
    qDebug() << "Region Removed!";

    foreach(QGraphicsItem* item, region->childItems())
    {
      LayoutRegion* child = dynamic_cast<LayoutRegion*> (item);

      if(item != NULL)
        removeRegion(child, regionBase);
      else
        qWarning() << "Trying to remove an element that is not\
                      of the type QnlyGraphicsRegion " << __FILE__ << __LINE__;
    }

    _regions.remove(region->getUid());
    regionBase->removeRegion(region);
    //delete(region);
  }
}

void LayoutView::changeRegion( LayoutRegion* region,
                               LayoutRegionBase* regionBase,
                               const QMap<QString, QString> attributes )
{
  if (region != NULL && regionBase != NULL)
  {
    regionBase->changeRegion(region, attributes);
  }
}

void LayoutView::selectRegion( LayoutRegion* region,
                               LayoutRegionBase* regionBase )
{
  if (region != nullptr && regionBase != nullptr)
  {
    if (_selectedRegion != nullptr)
    {
      _selectedRegion->setSelected(false);
    }

    regionBase->selectRegion(region);
    _selectedRegion = region;

    QWidget* parent = (QWidget*) regionBase->parent();
    setCurrentWidget(parent);
  }
}

void LayoutView::addRegionBase( LayoutRegionBase* regionBase,
                                const QMap<QString, QString> attributes )
{
  if (regionBase != NULL)
  {
    qDebug() << "LayoutRegionBase Added!";

    if (attributes.contains("id"))
      regionBase->setId(attributes["id"]);

    if (attributes.contains("region"))
      regionBase->setId(attributes["region"]);

    if (attributes.contains("device"))
      regionBase->setId(attributes["device"]);

    QWidget* parent = (QWidget*) regionBase->parent();
    addWidget(parent);
    setCurrentWidget(parent);

    _regionbases[regionBase->getUid()] = regionBase;

    QAction* action = new QAction(this);
    action->setText(regionBase->getId());
    action->setCheckable(true);
    action->setEnabled(true);

    _switchMenu->addAction(action);
    _regionbaseActionGroup->addAction(action);

    action->trigger();
    _regionbaseActions[regionBase->getUid()] = action;

    connect(regionBase, SIGNAL(regionbasePerformed()),
            SLOT(performRegionBase()));
    connect(regionBase,
            SIGNAL(regionAdditionRequested(QString,QString,QString,
                                           QMap<QString,QString>)),
            SLOT(requestRegionAddition(QString,QString,QString,
                                       QMap<QString,QString>)) );

    connect(regionBase,
            SIGNAL(regionChangeRequested(QString,QString,QMap<QString,QString>)),
            SLOT(requestRegionChange(QString,QString,QMap<QString,QString>)));

    connect(regionBase,
            SIGNAL(regionSelectionRequested(QString,QString)),
            SIGNAL(regionSelected(QString,QString)));

    connect(regionBase,
            SIGNAL(regionBaseSelectionRequested(QString)),
            SIGNAL(regionBaseSelected(QString)));

    connect(regionBase,
            SIGNAL(regionBaseDeletionRequested(QString)),
            SIGNAL(regionBaseRemoved(QString)));

     connect(regionBase,
             SIGNAL(regionDeletionRequested(QString,QString)),
             SIGNAL(regionRemoved(QString,QString)));

    connect(regionBase,
            SIGNAL(mediaOverRegion(QString,QString)),
            SIGNAL(mediaOverRegionAction(QString,QString)));

    regionBase->setGridVisible(gridVisibility);
    connect(regionBase,
            SIGNAL(gridVisibilityChanged(bool)),
            SLOT(setGridVisible(bool)));

    connect(regionBase,
            SIGNAL(copyRequested(LayoutRegion*)),
            SLOT(performCopy(LayoutRegion *)));

    connect(regionBase,
            SIGNAL(cutRequested(LayoutRegion*)),
            SLOT(performCut(LayoutRegion *)));

    connect(regionBase,
            SIGNAL(pasteRequested()),
            SLOT(performPaste()));

    emit regionBaseSelected(regionBase->getUid());
  }
}

void LayoutView::removeRegionBase(LayoutRegionBase* regionBase)
{
  if (regionBase != NULL)
  {
    QAction* action = _regionbaseActions[regionBase->getUid()];
    _switchMenu->removeAction(action);

    _regionbaseActionGroup->removeAction(action);

    foreach(QGraphicsItem* item, regionBase->items())
    {
      if (item != regionBase->getBackgroundItem())
      {
        LayoutRegion* child = dynamic_cast<LayoutRegion *> (item);

        if (child != NULL)
        {
          _regions.remove(child->getUid());
        }
        else
          qWarning() << "Trying to remove an element that is not of the\
                        type QnlyGraphicsRegion " << __FILE__ << __LINE__;
      }
    }

    QWidget* parent = (QWidget*) regionBase->parent();
    removeWidget(parent);
    _regionbases.remove(regionBase->getUid());

    if (currentWidget() != NULL)
    {
      LayoutRegionBase* current =
              (LayoutRegionBase*) ((QGraphicsView*)currentWidget())->scene();

      emit regionBaseSelected(current->getUid());
    }
  }
}

void LayoutView::changeRegionBase( LayoutRegionBase* regionBase,
                                   const QMap<QString, QString> attributes )
{
  if (regionBase != NULL)
  {
    if (attributes.contains("id"))
    {
      regionBase->setId(attributes["id"]);

      QAction* action = _regionbaseActions[regionBase->getUid()];
      action->setText(attributes["id"]);
    }

    if (attributes.contains("region"))
      regionBase->setId(attributes["region"]);

    if (attributes.contains("device"))
      regionBase->setId(attributes["device"]);
  }
}

void LayoutView::selectRegionBase(LayoutRegionBase* regionBase)
{
  if (regionBase != NULL)
  {
    QWidget* parent = (QWidget*) regionBase->parent();
    setCurrentWidget(parent);

    QAction* action = _regionbaseActions[regionBase->getUid()];
    action->setChecked(true);

    if (_selectedRegion != NULL)
    {
      _selectedRegion->setSelected(false);
      _selectedRegion = NULL;
    }

    _selectedRegionBase = regionBase;

    emit regionBaseSelected(_selectedRegionBase->getUid());
  }
}

void LayoutView::setGridVisible(bool visible)
{
  if(this->gridVisibility != visible)
  {
    this->gridVisibility = visible;
    foreach(LayoutRegionBase *regionBase, _regionbases.values())
    {
      regionBase->setGridVisible(visible);
    }

    emit gridVisibilityChanged(visible);
  }
}

LayoutRegionBase* LayoutView::getSelectedRegionBase()
{
  return _selectedRegionBase;
}

LayoutRegion* LayoutView::getSelectedRegion()
{
  return _selectedRegion;
}

void LayoutView::contextMenuEvent(QContextMenuEvent *event)
{
  QStackedWidget::contextMenuEvent(event);

  if (!event->isAccepted())
  {
    _contextMenu->exec(event->globalPos());
    event->accept();
  }
}

void LayoutView::doSnapshot()
{
  if(getSelectedRegionBase() != NULL)
    getSelectedRegionBase()->performExport();
}
