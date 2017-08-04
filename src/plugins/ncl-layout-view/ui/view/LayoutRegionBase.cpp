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

#include "LayoutViewPlugin_global.h"

#include <util/Preferences.h>
using namespace cpr::core;

LayoutRegionBase::LayoutRegionBase( QObject* parent, QMenu* switchMenu )
  : QGraphicsScene(parent)
{
  this->_switchMenu = switchMenu;

  this->_switchMenu->setEnabled(true);

  createActions();
  createMenus();
  createConnections();

  _graphicsRegionBaseId = NULL;

  _selectedRegion = NULL;

  setSceneRect(0,0,854,480);

  int w = 854, h = 480;
  _bgrect = new QGraphicsRectItem(QRect(0, 0, w, h));

  this->addItem(_bgrect);

  // bgrect->setBrush(QBrush(QPixmap(":/bg/layout")));
  _bgrect->setBrush(QBrush(Qt::white));
  _bgrect->setPen(QPen(QColor("#BBBBBB")));

  _bgrect->setZValue(-1);

  createGrid();
  createSafeArea();
}

LayoutRegionBase::~LayoutRegionBase()
{

}

QString LayoutRegionBase::getId() const
{
  return _id;
}

void LayoutRegionBase::setId(const QString &id)
{
  this->_id = id;

  //if(graphicsRegionBaseId == NULL)
  //  graphicsRegionBaseId = addText("regionBaseId");
  //graphicsRegionBaseId->setPos(10,10);
  //graphicsRegionBaseId->setToolTip(id);
  //graphicsRegionBaseId->setPlainText(id);
}

QString LayoutRegionBase::getUid() const
{
  return _uid;
}

void LayoutRegionBase::setUid(const QString &uid)
{
  this->_uid = uid;
}

QString LayoutRegionBase::getRegion() const
{
  return _region;
}

void LayoutRegionBase::setRegion(const QString &region)
{
  this->_region = region;
}

QString LayoutRegionBase::getDevice() const
{
  return _device;
}

void LayoutRegionBase::setDevice(const QString &device)
{
  this->_device = device;
}

void LayoutRegionBase::selectRegion(LayoutRegion* region)
{
  if (!region->isSelected())
  {
    if (_selectedRegion != NULL)
    {
      _selectedRegion->setSelected(false);
    }

    region->setSelected(true);

    _selectedRegion = region;
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

  emit regionChangeRequested( region->getUid(), _uid, full );
}

void LayoutRegionBase::requestRegionSelection(LayoutRegion* region)
{
  emit regionSelectionRequested(region->getUid(),_uid);
}

void LayoutRegionBase::LayoutRegionBase::createActions()
{
  // help action
  _helpAction = new QAction(this);
  _helpAction->setText(tr("Help"));

  _helpAction->setEnabled(true);
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

  //resolutions
  Preferences::getInstance ()->restore ();
  Preference *resolutions
      = Preferences::getInstance ()->getValue ("cpr.layout.resolutions");

  QStringList reslist = resolutions->value().toStringList();
  while(!reslist.isEmpty())
  {
    QStringList wh = reslist.first().split("x");
    QAction* res = new QAction(this);
    res->setText(reslist.takeFirst());
    res->setCheckable(true);
    res->setChecked(false);
    res->setData(QSize(QVariant(wh[0]).toInt(), QVariant(wh[1]).toInt()));
    connect(res, SIGNAL(triggered()), SLOT(performChangeResolution()));
    //should it initialize with the other connectors?

    _resolutions.append(res);
  }
  _resolutions.first()->setChecked(true);


  // hide action
  _hideAction = new QAction(this);
  _hideAction->setText(tr("Hide"));

  _hideAction->setEnabled(false);

  // Grid action
  _gridAction = new QAction(this);
  _gridAction->setText(tr("Grid"));
  _gridAction->setEnabled(true);
  _gridAction->setCheckable(true);
  _gridAction->setChecked(false);

  // Safe area action
  _safeAreaAction = new QAction(this);
  _safeAreaAction->setText(tr("Safe area"));
  _safeAreaAction->setEnabled(true);
  _safeAreaAction->setCheckable(true);
  _safeAreaAction->setChecked(false);

  // properties action
  _propertiesAction = new QAction(this);
  _propertiesAction->setText(tr("Properties"));

  _propertiesAction->setEnabled(true);

  _regionActionGroup = new QActionGroup(this);
  _regionActionGroup->setExclusive(false);

  _screensizeGroup  = new QActionGroup(this);
  _screensizeGroup->setExclusive(true);

  for(int i=0; i<_resolutions.size(); i++)
  {
    _screensizeGroup->addAction(_resolutions.at(i));
  }
}

void LayoutRegionBase::createMenus()
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
  _showMenu->addAction(_gridAction);
  _showMenu->addAction(_safeAreaAction);

  _showMenu->setEnabled(true);

  // arrange menu
  _arrangeMenu = new QMenu();
  _arrangeMenu->setTitle(tr("Arrange"));

  _arrangeMenu->setEnabled(false);

  _arrangeMenu->addAction(_bringfrontAction);
  _arrangeMenu->addAction(_bringforwardAction);
  _arrangeMenu->addAction(_sendbackwardAction);
  _arrangeMenu->addAction(_sendbackAction);

  // screensize menu
  _screensizeMenu = new QMenu();
  _screensizeMenu->setTitle(tr("Screen Size"));

  _screensizeMenu->setEnabled(true);

  for(int i=0; i<_resolutions.size(); i++)
  {
    _screensizeMenu->addAction(_resolutions.at(i));
  }

  // context menu
  _contextMenu = new QMenu();
  _contextMenu->addAction(_regionAction);
  // contextMenu->addAction(regionbaseAction);
  _contextMenu->addSeparator();
  _contextMenu->addAction(_deleteAction);
  _contextMenu->addSeparator();
  // contextMenu->addAction(helpAction);
  // contextMenu->addSeparator();
  // contextMenu->addAction(undoAction);
  // contextMenu->addAction(redoAction);
  // contextMenu->addSeparator();
  // contextMenu->addAction(cutAction);
  // contextMenu->addAction(copyAction);
  _contextMenu->addAction(_pasteAction);
  _contextMenu->addSeparator();
  _contextMenu->addAction(_exportAction);
  _contextMenu->addSeparator();
  // contextMenu->addMenu(viewMenu); //disabled for while
  _contextMenu->addMenu(_showMenu);
  _contextMenu->addMenu(_screensizeMenu);
  // contextMenu->addMenu(arrangeMenu); //disabled for while
  _contextMenu->addSeparator();
  _contextMenu->addAction(_hideAction);
  _contextMenu->addSeparator();
  _contextMenu->addMenu(_switchMenu);
  // contextMenu->addSeparator(); // disabled for while
  // contextMenu->addAction(propertiesAction); //disabled for while
}

void LayoutRegionBase::createConnections()
{
  connect(_regionAction, SIGNAL(triggered()),
          SLOT(performRegion()));

  connect(_regionbaseAction, SIGNAL(triggered()),
          SIGNAL(regionbasePerformed()));

  connect(_regionActionGroup, SIGNAL(triggered(QAction*)),
          SLOT(performShow(QAction*)));

  connect(_deleteAction, SIGNAL(triggered()),
          SLOT(performDelete()));

  connect(_exportAction, SIGNAL(triggered()), SLOT(performExport()));

  connect(_gridAction, SIGNAL(triggered()), SLOT(performGrid()));
  connect(_safeAreaAction, SIGNAL(triggered()), SLOT(performSafeArea()));

  connect(_pasteAction, SIGNAL(triggered()), SIGNAL(pasteRequested()));
}

void LayoutRegionBase::performShow(QAction* action)
{
  if (!action->isChecked())
    _regions[_regionActions.key(action)]->setVisible(false);
  else
    _regions[_regionActions.key(action)]->setVisible(true);
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
  foreach(LayoutRegion *region, _regions.values())
  {
    zIndex = zIndex > region->getzIndex() ? zIndex : region->getzIndex() + 1;
  }
  attributes["zIndex"] = QString::number(zIndex);

  emit regionAdditionRequested("", parent->getUid(), _uid, attributes);
}

void LayoutRegionBase::requestRegionDeletion(LayoutRegion* region)
{
  emit regionDeletionRequested(region->getUid(), _uid);
}

void LayoutRegionBase::updateActionText(LayoutRegion *region)
{
  // Update Show Menu
  if(_regionActions.contains(region->getUid()))
    _regionActions[region->getUid()]->setText(region->getId());
}

void LayoutRegionBase::hideRegion(LayoutRegion* region)
{
  _regionActions[region->getUid()]->trigger();
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
          _regions.remove(child->getUid());
        else
          qCWarning (CPR_PLUGIN_LAYOUT) << "Trying to remove an element that is not of\
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
          _regions.remove(child->getUid());
        else
          qCWarning (CPR_PLUGIN_LAYOUT) << "Trying to remove an element that is not of the\
                        type QnlyGraphicsRegion " << __FILE__ << __LINE__;
      }

      if(_regionActions.contains(region->getUid()))
      {
        QAction *action = _regionActions[region->getUid()];

        _showMenu->removeAction(action);

        _regionActionGroup->removeAction(action);

        _regionActions.remove(region->getUid());
      }
    }

    removeItem(region);

    _regions.remove(region->getUid());

    //  delete(region);
    _selectedRegion = NULL;

    emit regionBaseSelectionRequested(_uid);
  }
}

QGraphicsItem* LayoutRegionBase::getBackgroundItem()
{
  return _bgrect;
}

void LayoutRegionBase::performDelete()
{
  emit regionBaseDeletionRequested(_uid);
}

void LayoutRegionBase::performRegion()
{
  QMap<QString, QString> attributes = {
      {"top", "10%" },
      {"left", "10%"},
      {"right", "10%"},
      {"bottom", "10%"},
      {"width", "80%"},
      {"height", "80%"}
  };

  //Make the zIndex to be the greater one in the regionBase
  int zIndex = 0;
  foreach(LayoutRegion *region, _regions.values())
  {
    zIndex = zIndex > region->getzIndex() ? zIndex : region->getzIndex() + 1;
  }
  attributes["zIndex"] = QString::number(zIndex);

  emit regionAdditionRequested("", "", _uid, attributes);
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

    region->setGridAction(_gridAction);

    if (parent != NULL)
    {
      parent->addRegion(region);
    }
    else
    {
      addItem(region);

      _action = new QAction(this);
      _action->setText(region->getId());

      _showMenu->insertAction(_showMenu->actions().front(), _action);

      if (_showMenu->actions().size() <= 2)
      {
        _showMenu->insertSeparator(_showMenu->actions().back());
      }

      _action->setCheckable(true);
      _action->setChecked(true);
      _action->setEnabled(true);

      _regionActionGroup->addAction(_action);

      _regionActions[region->getUid()] = _action;
    }

    region->adjust();

    _regions[region->getUid()] = region;

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
            SIGNAL(cutRequested(LayoutRegion*)),
            SIGNAL(cutRequested(LayoutRegion*)));

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
  _bgrect->setRect(0, 0, w, h);

  updateSafeAreaPos();

  _grid->setRect(0, 0, w, h);

  foreach(LayoutRegion* r, _regions.values())
    r->adjust();

  QSize size(w, h);
  for (int i=0;i<_resolutions.size();i++)
  {
    if(_resolutions.at(i)->data().toSize() == size)
      _resolutions.at(i)->setChecked(true);
  }

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
      emit regionBaseSelectionRequested(_uid);
    }

    event->accept();
  }
}

void LayoutRegionBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if(_selectedRegion != NULL)
  {
    _selectedRegion->updateCursor(event);
  }

  QGraphicsScene::mouseMoveEvent(event);
}

void LayoutRegionBase::contextMenuEvent(
      QGraphicsSceneContextMenuEvent* event)
{
  QGraphicsScene::contextMenuEvent(event);

  if (!event->isAccepted())
  {
    emit regionBaseSelectionRequested(_uid);

    _contextMenu->exec(event->screenPos());

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
  _grid->setPen(QPen(QBrush(Qt::lightGray), 1,Qt::DotLine));
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
  _gridAction->setChecked(visible);
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
