#include "StructuralWindow.h"

StructuralWindow::StructuralWindow (QWidget *parent)
    : QMainWindow (parent), _view (new StructuralView (this))
{
  createActions ();
  createToolbar ();
  createStatusbar ();
  createConnections ();

  setCentralWidget (_view);
}

StructuralWindow::~StructuralWindow () {}

StructuralView *
StructuralWindow::getView () const
{
  return _view;
}

void
StructuralWindow::setView (StructuralView *view)
{
  _view = view;
}

void
StructuralWindow::createActions ()
{
  // undo action
  _undoAction = new QAction (this);
  _undoAction->setEnabled (false);
  _undoAction->setText (tr ("Undo"));
  _undoAction->setIcon (QIcon (":/icon/undo"));
  _undoAction->setShortcut (QKeySequence ("Ctrl+Z"));
  _view->getMenu ()->addAction (_undoAction);

  // redo action
  _redoAction = new QAction (this);
  _redoAction->setEnabled (false);
  _redoAction->setText (tr ("Redo"));
  _redoAction->setIcon (QIcon (":/icon/redo"));
  _redoAction->setShortcut (QKeySequence ("Ctrl+Shift+Z"));
  _view->getMenu ()->addAction (_redoAction);
  _view->getMenu ()->addSeparator ();

  // cut action
  _cutAction = new QAction (this);
  _cutAction->setEnabled (false);
  _cutAction->setText (tr ("Cut"));
  _cutAction->setIcon (QIcon (":/icon/cut"));
  _cutAction->setShortcut (QKeySequence ("Ctrl+X"));
  _view->getMenu ()->addAction (_cutAction);

  // copy action
  _copyAction = new QAction (this);
  _copyAction->setEnabled (false);
  _copyAction->setText (tr ("Copy"));
  _copyAction->setIcon (QIcon (":/icon/copy"));
  _copyAction->setShortcut (QKeySequence ("Ctrl+C"));
  _view->getMenu ()->addAction (_copyAction);

  // paste action
  _pasteAction = new QAction (this);
  _pasteAction->setEnabled (false);
  _pasteAction->setText (tr ("Paste"));
  _pasteAction->setIcon (QIcon (":/icon/paste"));
  _pasteAction->setShortcut (QKeySequence ("Ctrl+V"));
  _view->getMenu ()->addAction (_pasteAction);
  _view->getMenu ()->addSeparator ();

  // delete action
  _deleteAction = new QAction (this);
  _deleteAction->setEnabled (false);
  _deleteAction->setText (tr ("Delete"));
  _deleteAction->setToolTip (tr ("Delete"));
  _deleteAction->setIcon (QIcon (":/icon/delete"));
  _view->getMenu ()->addAction (_deleteAction);
  _view->getMenu ()->addSeparator ();

  // snapshot action
  _snapshotAction = new QAction (this);
  _snapshotAction->setEnabled (true);
  _snapshotAction->setText (tr ("Snapshot"));
  _snapshotAction->setToolTip (tr ("Take a Snapshot..."));
  _snapshotAction->setIcon (QIcon (":/icon/snapshot"));
  // _view->getMenu ()->addAction (_snapshotAction);

  // zoomin action
  _zoominAction = new QAction (this);
  _zoominAction->setEnabled (false);
  _zoominAction->setText (tr ("Zoom In"));
  _zoominAction->setIcon (QIcon (":/icon/zoomin"));
  _zoominAction->setShortcut (QKeySequence ("Ctrl++"));

  // zoomout action
  _zoomoutAction = new QAction (this);
  _zoomoutAction->setEnabled (true);
  _zoomoutAction->setText (tr ("Zoom Out"));
  _zoomoutAction->setIcon (QIcon (":/icon/zoomout"));
  _zoomoutAction->setShortcut (QKeySequence ("Ctrl+-"));

  // zoomreset action
  _zoomresetAction = new QAction (this);
  _zoomresetAction->setEnabled (false);
  _zoomresetAction->setText (tr ("Reset Zoom"));
  _zoomresetAction->setIcon (QIcon (":/icon/zoomreset"));
  _zoomresetAction->setShortcut (QKeySequence ("Ctrl+0"));

  // pointer action
  _pointerAction = new QAction (this);
  _pointerAction->setEnabled (true);
  _pointerAction->setCheckable (true);
  _pointerAction->setChecked (true);
  _pointerAction->setText (tr ("Pointer tool"));
  _pointerAction->setToolTip (tr ("Pointer tool"));
  _pointerAction->setIcon (QIcon (":/icon/pointer"));
  _pointerAction->setShortcut (QKeySequence ("1"));

  // link action
  _linkAction = new QAction (this);
  _linkAction->setEnabled (true);
  _linkAction->setCheckable (true);
  _linkAction->setChecked (false);
  _linkAction->setText (tr ("Linking tool"));
  _linkAction->setToolTip (tr ("Linking tool"));
  _linkAction->setIcon (QIcon (":/icon/link"));
  _linkAction->setShortcut (QKeySequence ("2"));

  // autostart action
  _autostartAction = new QAction (this);
  _autostartAction->setEnabled (false);
  _autostartAction->setCheckable (true);
  _autostartAction->setChecked (false);
  _autostartAction->setText (tr ("Set as starting node"));
  _autostartAction->setIcon (QIcon (""));
  _view->getMenu ()->addAction (_autostartAction);

  QMenu *insertMenu = new QMenu (_view->getMenu ());
  insertMenu->setTitle (tr ("&Insert"));

  // media action
  _mediaAction = new QAction (this);
  _mediaAction->setEnabled (false);
  _mediaAction->setText (tr ("Media"));
  _mediaAction->setToolTip (tr ("Insert &lt;media&gt; entity"));
  _mediaAction->setIcon (QIcon (":/icon/media-insert"));
  _mediaAction->setShortcut (QKeySequence ("3"));
  insertMenu->addAction (_mediaAction);

  // context action
  _contextAction = new QAction (this);
  _contextAction->setEnabled (false);
  _contextAction->setText (tr ("Context"));
  _contextAction->setToolTip (tr ("Insert &lt;context&gt; entity"));
  _contextAction->setIcon (QIcon (":/icon/context-insert"));
  _contextAction->setShortcut (QKeySequence ("4"));
  insertMenu->addAction (_contextAction);

  // switch action
  _switchAction = new QAction (this);
  _switchAction->setEnabled (false);
  _switchAction->setText (tr ("Switch"));
  _switchAction->setToolTip (tr ("Insert &lt;switch&gt; entity"));
  _switchAction->setIcon (QIcon (":/icon/switch-insert"));
  _switchAction->setShortcut (QKeySequence ("5"));
  insertMenu->addAction (_switchAction);

  // body action
  _bodyAction = new QAction (this);
  _bodyAction->setEnabled (true);
  _bodyAction->setText (tr ("Body"));
  _bodyAction->setToolTip (tr ("Insert &lt;body&gt; entity"));
  _bodyAction->setIcon (QIcon (":/icon/body-insert"));
  _bodyAction->setShortcut (QKeySequence ("6"));
  insertMenu->addAction (_bodyAction);

  // area action
  _areaAction = new QAction (this);
  _areaAction->setEnabled (false);
  _areaAction->setText (tr ("Area"));
  _areaAction->setToolTip (tr ("Insert &lt;area&gt; entity"));
  _areaAction->setIcon (QIcon (":/icon/area-insert"));
  _areaAction->setShortcut (QKeySequence ("7"));
  insertMenu->addAction (_areaAction);

  // property action
  _propertyAction = new QAction (this);
  _propertyAction->setEnabled (false);
  _propertyAction->setText (tr ("Property"));
  _propertyAction->setToolTip (tr ("Insert &lt;property&gt; entity"));
  _propertyAction->setIcon (QIcon (":/icon/property-insert"));
  _propertyAction->setShortcut (QKeySequence ("8"));
  insertMenu->addAction (_propertyAction);

  // port action
  _portAction = new QAction (this);
  _portAction->setEnabled (false);
  _portAction->setText (tr ("Port"));
  _portAction->setToolTip (tr ("Insert &lt;port&gt; entity"));
  _portAction->setIcon (QIcon (":/icon/port-insert"));
  _portAction->setShortcut (QKeySequence ("9"));
  insertMenu->addAction (_portAction);

  // switchport action
  _switchportAction = new QAction (this);
  _switchportAction->setEnabled (false);
  _switchportAction->setText (tr ("Switch Port"));
  _switchportAction->setToolTip (tr ("Insert &lt;switchport&gt; entity"));
  _switchportAction->setIcon (QIcon (":/icon/switchport-insert"));
  _switchportAction->setShortcut (QKeySequence ("0"));
  insertMenu->addAction (_switchportAction);

  _view->getMenu ()->addMenu (insertMenu);

  // minimap action
  _minimapAction = new QAction (this);
  _minimapAction->setEnabled (true);
  _minimapAction->setCheckable (true);
  _minimapAction->setChecked (true);
  _minimapAction->setText (tr ("Minimap"));
  _minimapAction->setToolTip (tr ("Show/Hide minimap"));
  _minimapAction->setIcon (QIcon (":/icon/minimap"));
  _minimapAction->setShortcut (QKeySequence ("Ctrl+M"));

  // preferences action
  _preferencesAction = new QAction (this);
  _preferencesAction->setEnabled (true);
  _preferencesAction->setText (tr ("Preferences..."));
  _preferencesAction->setIcon (QIcon (":/icon/preferences"));
  _preferencesAction->setShortcut (QKeySequence ("Ctrl+P"));

  // properties action
  _propertiesAction = new QAction (this);
  _propertiesAction->setEnabled (false);
  _propertiesAction->setText (tr ("Properties"));
  _propertiesAction->setIcon (QIcon (":/icon/properties"));
  _view->getMenu ()->addAction (_propertiesAction);

  // insert group
  _insertActionGroup = new QActionGroup (this);
  _insertActionGroup->setExclusive (true);

  _insertActionGroup->addAction (_linkAction);
  _insertActionGroup->addAction (_pointerAction);

#ifdef WITH_GRAPHVIZ
  // autoadjust action
  _autoadjustAction = new QAction (this);
  _autoadjustAction->setEnabled (true);
  _autoadjustAction->setText (tr ("Auto adjust"));
  _view->getMenu ()->addAction (_autoadjustAction);
#endif
}

void
StructuralWindow::createToolbar ()
{
  // edit toolbar
  _editToolbar = addToolBar (tr ("Edit"));

  _editToolbar->setMovable (true);
  _editToolbar->setFloatable (true);

  _editToolbar->addAction (_undoAction);
  _editToolbar->addAction (_redoAction);
  _editToolbar->addSeparator ();
  _editToolbar->addAction (_cutAction);
  _editToolbar->addAction (_copyAction);
  _editToolbar->addAction (_pasteAction);
  _editToolbar->addSeparator ();
  _editToolbar->addAction (_deleteAction);

#ifdef Q_WS_MACX
  _editToolbar->addSeparator ();
#endif

  // insert toolbar
  _insertToolbar = addToolBar (tr ("Insert"));
  _insertToolbar->setMovable (true);
  _insertToolbar->setFloatable (true);

  _insertToolbar->addAction (_pointerAction);
  _insertToolbar->addAction (_linkAction);

  _insertToolbar->addSeparator ();
  _insertToolbar->addAction (_mediaAction);
  _insertToolbar->addSeparator ();
  _insertToolbar->addAction (_contextAction);
  _insertToolbar->addAction (_switchAction);

  if (ST_DEFAULT_WITH_BODY)
  {
    _insertToolbar->addAction (_bodyAction);
  }

  if (ST_DEFAULT_WITH_INTERFACES)
  {
    _insertToolbar->addSeparator ();
    _insertToolbar->addAction (_areaAction);
    _insertToolbar->addAction (_propertyAction);
    _insertToolbar->addAction (_portAction);
    _insertToolbar->addAction (_switchportAction);
  }

#ifdef Q_WS_MACX
  setUnifiedTitleAndToolBarOnMac (true);
#endif

  // window toolbar
  _windowToolbar = addToolBar (tr ("Window"));
  _windowToolbar->setMovable (true);
  _windowToolbar->setFloatable (true);

  _windowToolbar->addAction (_zoominAction);
  _windowToolbar->addAction (_zoomoutAction);
  _windowToolbar->addAction (_zoomresetAction);
  _windowToolbar->addSeparator ();
  _windowToolbar->addAction (_snapshotAction);
  _windowToolbar->addSeparator ();
  _windowToolbar->addAction (_minimapAction);

#ifdef Q_WS_MACX
  _editToolbar->addSeparator ();
#endif

  select ("", StructuralUtil::createSettings ());
}

void
StructuralWindow::createStatusbar ()
{
  // TODO
}

void
StructuralWindow::createConnections ()
{
  connect (_snapshotAction, &QAction::triggered, _view,
           &StructuralView::snapshot);
  connect (_zoominAction, &QAction::triggered, _view, &StructuralView::zoomIn);
  connect (_zoomoutAction, &QAction::triggered, _view,
           &StructuralView::zoomOut);
  connect (_zoomresetAction, &QAction::triggered, _view,
           &StructuralView::zoomOriginal);

  connect (_minimapAction, &QAction::triggered, _view,
           &StructuralView::toggleMinimapVisibility);

  connect (_undoAction, &QAction::triggered, _view,
           &StructuralView::performUndo);
  connect (_redoAction, &QAction::triggered, _view,
           &StructuralView::performRedo);
  connect (_cutAction, &QAction::triggered, _view,
           &StructuralView::performCut);
  connect (_copyAction, &QAction::triggered, _view,
           &StructuralView::performCopy);
  connect (_pasteAction, &QAction::triggered, _view,
           &StructuralView::performPaste);
  connect (_deleteAction, &QAction::triggered, _view,
           &StructuralView::performDelete);

  connect (_pointerAction, &QAction::triggered, _view,
           &StructuralView::performPointer);
  connect (_linkAction, &QAction::triggered, _view,
           &StructuralView::performLink);

  connect (_autostartAction, &QAction::triggered, _view,
           &StructuralView::performAutostart);

  connect (_mediaAction, &QAction::triggered,
           [&]() { _view->createEntity (StructuralType::Media); });

  connect (_contextAction, &QAction::triggered,
           [&]() { _view->createEntity (StructuralType::Context); });

  connect (_switchAction, &QAction::triggered,
           [&]() { _view->createEntity (StructuralType::Switch); });

  connect (_bodyAction, &QAction::triggered,
           [&]() { _view->createEntity (StructuralType::Body); });

  connect (_areaAction, &QAction::triggered,
           [&]() { _view->createEntity (StructuralType::Area); });

  connect (_propertyAction, &QAction::triggered,
           [&]() { _view->createEntity (StructuralType::Property); });

  connect (_portAction, &QAction::triggered,
           [&]() { _view->createEntity (StructuralType::Port); });

  connect (_switchportAction, &QAction::triggered,
           [&]() { _view->createEntity (StructuralType::SwitchPort); });

#ifdef WITH_GRAPHVIZ
  connect (_autostartAction, &QAction::triggered, this,
           &StructuralView::performAutoAdjust);
#endif

  connect (_view, &StructuralView::canUndoChanged, _undoAction,
           &QAction::setEnabled);
  connect (_view, &StructuralView::canRedoChanged, _redoAction,
           &QAction::setEnabled);
  connect (_view, &StructuralView::switchedCut, _cutAction,
           &QAction::setEnabled);
  connect (_view, &StructuralView::switchedCopy, _copyAction,
           &QAction::setEnabled);
  connect (_view, &StructuralView::switchedPaste, _pasteAction,
           &QAction::setEnabled);
  connect (_view, &StructuralView::switchedDelete, _deleteAction,
           &QAction::setEnabled);
  connect (_view, &StructuralView::switchedSnapshot, _snapshotAction,
           &QAction::setEnabled);

  connect (_view, &StructuralView::zoomChanged, this,
           &StructuralWindow::handleZoomChanged);

  connect (_view, &StructuralView::switchedPointer, this,
           &StructuralWindow::switchPointer);
  connect (_view, &StructuralView::switchedLink, this,
           &StructuralWindow::switchLink);
  connect (_view, &StructuralView::switchedBody, this,
           &StructuralWindow::switchBody);

  connect (_view, &StructuralView::selected, this, &StructuralWindow::select);
}

void
StructuralWindow::handleZoomChanged (int zoom)
{
  if (zoom >= StructuralView::ZOOM_MAX)
  {
    _zoominAction->setEnabled (false);
  }
  else
    _zoominAction->setEnabled (true);

  if (zoom <= StructuralView::ZOOM_MIN)
    _zoomoutAction->setEnabled (false);
  else
    _zoomoutAction->setEnabled (true);

  if (zoom == StructuralView::ZOOM_ORIGINAL)
    _zoomresetAction->setEnabled (false);
  else
    _zoomresetAction->setEnabled (true);
}

void
StructuralWindow::switchPointer (bool state)
{
  if (state)
    _pointerAction->setChecked (true);
  else
    _linkAction->setChecked (true);
}

void
StructuralWindow::switchLink (bool state)
{
  switchPointer (!state);
}

void
StructuralWindow::switchBody (bool state)
{
  _bodyAction->setEnabled (state);
}

void
StructuralWindow::select (QString uid, QMap<QString, QString> settings)
{
  Q_UNUSED (settings);

  StructuralEntity *entity = _view->getEntity (uid);

  if (entity != NULL)
  {
    StructuralType type = _view->getEntity (uid)->getType ();

    switch (type)
    {
      case Structural::Media:
      {
        _mediaAction->setEnabled (false);
        _contextAction->setEnabled (false);
        _switchAction->setEnabled (false);
        _bodyAction->setEnabled (false);
        _areaAction->setEnabled (true);
        _propertyAction->setEnabled (true);
        _portAction->setEnabled (false);
        _switchportAction->setEnabled (false);

        break;
      }

      case Structural::Context:
      {
        _mediaAction->setEnabled (true);
        _contextAction->setEnabled (true);
        _switchAction->setEnabled (true);
        _bodyAction->setEnabled (false);
        _areaAction->setEnabled (false);
        _propertyAction->setEnabled (true);
        _portAction->setEnabled (true);
        _switchportAction->setEnabled (false);

        break;
      }

      case Structural::Switch:
      {
        _mediaAction->setEnabled (true);
        _contextAction->setEnabled (true);
        _switchAction->setEnabled (true);
        _bodyAction->setEnabled (false);
        _areaAction->setEnabled (false);
        _propertyAction->setEnabled (true);
        _portAction->setEnabled (false);
        _switchportAction->setEnabled (true);

        break;
      }

      case Structural::Body:
      {
        _mediaAction->setEnabled (true);
        _contextAction->setEnabled (true);
        _switchAction->setEnabled (true);
        _bodyAction->setEnabled (false);
        _areaAction->setEnabled (false);
        _propertyAction->setEnabled (true);
        _portAction->setEnabled (true);
        _switchportAction->setEnabled (false);

        break;
      }

      case Structural::Area:
      case Structural::Property:
      case Structural::Port:
      case Structural::SwitchPort:
      {
        _mediaAction->setEnabled (false);
        _contextAction->setEnabled (false);
        _switchAction->setEnabled (false);
        _bodyAction->setEnabled (false);
        _areaAction->setEnabled (false);
        _propertyAction->setEnabled (false);
        _portAction->setEnabled (false);
        _switchportAction->setEnabled (false);

        break;
      }

      default:
      {
        break;
      }
    }

    _cutAction->setEnabled (true);
    _copyAction->setEnabled (true);
    _deleteAction->setEnabled (true);
  }
  else
  {
    _switchportAction->setEnabled (false);

    if (ST_DEFAULT_WITH_BODY)
    {
      _mediaAction->setEnabled (false);
      _contextAction->setEnabled (false);
      _switchAction->setEnabled (false);
      _bodyAction->setEnabled ((_view->getBody () == NULL));
      _areaAction->setEnabled (false);
      _propertyAction->setEnabled (false);
      _portAction->setEnabled (false);
    }
    else
    {
      _mediaAction->setEnabled (true);
      _contextAction->setEnabled (true);
      _switchAction->setEnabled (true);
      _bodyAction->setEnabled (false);
      _areaAction->setEnabled (false);

      if (ST_DEFAULT_WITH_FLOATING_INTERFACES)
      {
        _propertyAction->setEnabled (true);
        _portAction->setEnabled (true);
      }
      else
      {
        _propertyAction->setEnabled (false);
        _portAction->setEnabled (false);
      }
    }

    _cutAction->setEnabled (false);
    _copyAction->setEnabled (false);
    _deleteAction->setEnabled (false);
  }
}
