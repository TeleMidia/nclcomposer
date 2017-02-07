#include "StructuralWindow.h"

StructuralWindow::StructuralWindow(QWidget* parent)
  : QMainWindow(parent)
{
  createWidgets();
  createActions();
  createToolbar();
  createStatusbar();
  createConnections();
}

StructuralWindow::~StructuralWindow()
{

}

StructuralView* StructuralWindow::getView() const
{
  return _view;
}

void StructuralWindow::setView(StructuralView* view)
{
  _view = view;
}

void StructuralWindow::createActions()
{
  // undo action
  _undoAction = new QAction(this);
  _undoAction->setEnabled(false);
  _undoAction->setText(tr("Undo"));
  _undoAction->setIcon(QIcon(":/icon/undo"));
  _undoAction->setShortcut(QKeySequence("Ctrl+Z"));

  // redo action
  _redoAction = new QAction((QObject*) this);
  _redoAction->setEnabled(false);
  _redoAction->setText(tr("Redo"));
  _redoAction->setIcon(QIcon(":/icon/redo"));
  _redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

  // cut action
  _cutAction = new QAction((QObject*) this);
  _cutAction->setEnabled(false);
  _cutAction->setText(tr("Cut"));
  _cutAction->setIcon(QIcon(":/icon/cut"));
  _cutAction->setShortcut(QKeySequence("Ctrl+X"));

  // copy action
  _copyAction = new QAction((QObject*) this);
  _copyAction->setEnabled(false);
  _copyAction->setText(tr("Copy"));
  _copyAction->setIcon(QIcon(":/icon/copy"));
  _copyAction->setShortcut(QKeySequence("Ctrl+C"));

  // paste action
  _pasteAction = new QAction((QObject*) this);
  _pasteAction->setEnabled(false);
  _pasteAction->setText(tr("Paste"));
  _pasteAction->setIcon(QIcon(":/icon/paste"));
  _pasteAction->setShortcut(QKeySequence("Ctrl+V"));

  // delete action
  _deleteAction = new QAction((QObject*) this);
  _deleteAction->setEnabled(false);
  _deleteAction->setText(tr("Delete"));
  _deleteAction->setToolTip(tr("Delete"));
  _deleteAction->setIcon(QIcon(":/icon/delete"));

  // snapshot action
  _snapshotAction = new QAction((QObject*) this);
  _snapshotAction->setEnabled(true);
  _snapshotAction->setText(tr("Snapshot"));
  _snapshotAction->setToolTip(tr("Take a Snapshot..."));
  _snapshotAction->setIcon(QIcon(":/icon/snapshot"));

  // zoomin action
  _zoominAction = new QAction((QObject*) this);
  _zoominAction->setEnabled(false);
  _zoominAction->setText(tr("Zoom In"));
  _zoominAction->setIcon(QIcon(":/icon/zoomin"));
  _zoominAction->setShortcut(QKeySequence("Ctrl++"));

  // zoomout action
  _zoomoutAction = new QAction((QObject*) this);
  _zoomoutAction->setEnabled(true);
  _zoomoutAction->setText(tr("Zoom Out"));
  _zoomoutAction->setIcon(QIcon(":/icon/zoomout"));
  _zoomoutAction->setShortcut(QKeySequence("Ctrl+-"));

  // zoomreset action
  _zoomresetAction = new QAction((QObject*) this);
  _zoomresetAction->setEnabled(false);
  _zoomresetAction->setText(tr("Reset Zoom"));
  _zoomresetAction->setIcon(QIcon(":/icon/zoomreset"));
  _zoomresetAction->setShortcut(QKeySequence("Ctrl+0"));

  // pointer action
  _pointerAction = new QAction((QObject*) this);
  _pointerAction->setEnabled(true);
  _pointerAction->setCheckable(true);
  _pointerAction->setChecked(true);
  _pointerAction->setText(tr("Pointer tool"));
  _pointerAction->setToolTip(tr("Pointer tool"));
  _pointerAction->setIcon(QIcon(":/icon/pointer"));
  _pointerAction->setShortcut(QKeySequence("1"));

  // link action
  _linkAction = new QAction((QObject*) this);
  _linkAction->setEnabled(true);
  _linkAction->setCheckable(true);
  _linkAction->setChecked(false);
  _linkAction->setText(tr("Linking tool"));
  _linkAction->setToolTip(tr("Linking tool"));
  _linkAction->setIcon(QIcon(":/icon/link"));
  _linkAction->setShortcut(QKeySequence("2"));

  // media action
  _mediaAction = new QAction((QObject*) this);
  _mediaAction->setEnabled(false);
  _mediaAction->setText(tr("Media"));
  _mediaAction->setToolTip(tr("Insert &lt;media&gt; entity"));
  _mediaAction->setIcon(QIcon(":/icon/media-insert"));
  _mediaAction->setShortcut(QKeySequence("3"));

  // context action
  _contextAction = new QAction((QObject*) this);
  _contextAction->setEnabled(false);
  _contextAction->setText(tr("Context"));
  _contextAction->setToolTip(tr("Insert &lt;context&gt; entity"));
  _contextAction->setIcon(QIcon(":/icon/context-insert"));
  _contextAction->setShortcut(QKeySequence("4"));

  // switch action
  _switchAction = new QAction((QObject*) this);
  _switchAction->setEnabled(false);
  _switchAction->setText(tr("Switch"));
  _switchAction->setToolTip(tr("Insert &lt;switch&gt; entity"));
  _switchAction->setIcon(QIcon(":/icon/switch-insert"));
  _switchAction->setShortcut(QKeySequence("5"));

  // body action
  _bodyAction = new QAction((QObject*) this);
  _bodyAction->setEnabled(true);
  _bodyAction->setText(tr("Body"));
  _bodyAction->setToolTip(tr("Insert &lt;body&gt; entity"));
  _bodyAction->setIcon(QIcon(":/icon/body-insert"));
  _bodyAction->setShortcut(QKeySequence("6"));

  // area action
  _areaAction = new QAction((QObject*) this);
  _areaAction->setEnabled(false);
  _areaAction->setText(tr("Area"));
  _areaAction->setToolTip(tr("Insert &lt;area&gt; entity"));
  _areaAction->setIcon(QIcon(":/icon/area-insert"));
  _areaAction->setShortcut(QKeySequence("7"));

  // property action
  _propertyAction = new QAction((QObject*) this);
  _propertyAction->setEnabled(false);
  _propertyAction->setText(tr("Property"));
  _propertyAction->setToolTip(tr("Insert &lt;property&gt; entity"));
  _propertyAction->setIcon(QIcon(":/icon/property-insert"));
  _propertyAction->setShortcut(QKeySequence("8"));

  // port action
  _portAction = new QAction((QObject*) this);
  _portAction->setEnabled(false);
  _portAction->setText(tr("Port"));
  _portAction->setToolTip(tr("Insert &lt;port&gt; entity"));
  _portAction->setIcon(QIcon(":/icon/port-insert"));
  _portAction->setShortcut(QKeySequence("9"));

  // switchport action
  _switchportAction = new QAction((QObject*) this);
  _switchportAction->setEnabled(false);
  _switchportAction->setText(tr("Switch Port"));
  _switchportAction->setToolTip(tr("Insert &lt;switchport&gt; entity"));
  _switchportAction->setIcon(QIcon(":/icon/switchport-insert"));
  _switchportAction->setShortcut(QKeySequence("0"));

  // minimap action
  _minimapAction = new QAction((QObject*) this);
  _minimapAction->setEnabled(true);
  _minimapAction->setCheckable(true);
  _minimapAction->setChecked(true);
  _minimapAction->setText(tr("Minimap"));
  _minimapAction->setToolTip(tr("Show/Hide minimap"));
  _minimapAction->setIcon(QIcon(":/icon/minimap"));
  _minimapAction->setShortcut(QKeySequence("Ctrl+M"));

  // preferences action
  _preferencesAction = new QAction((QObject*) this);
  _preferencesAction->setEnabled(true);
  _preferencesAction->setText(tr("Preferences..."));
  _preferencesAction->setIcon(QIcon(":/icon/preferences"));
  _preferencesAction->setShortcut(QKeySequence("Ctrl+P"));

  // insert group
  _insertActionGroup = new QActionGroup((QObject*) this);
  _insertActionGroup->setExclusive(true);

  _insertActionGroup->addAction(_linkAction);
  _insertActionGroup->addAction(_pointerAction);
}

void StructuralWindow::createToolbar()
{
  // edit toolbar
  _editToolbar = addToolBar(tr("Edit"));

  _editToolbar->setMovable(true);
  _editToolbar->setFloatable(true);
  _editToolbar->setIconSize(QSize(24,24));

  _editToolbar->addAction(_undoAction);
  _editToolbar->addAction(_redoAction);
  _editToolbar->addSeparator();
  _editToolbar->addAction(_cutAction);
  _editToolbar->addAction(_copyAction);
  _editToolbar->addAction(_pasteAction);
  _editToolbar->addSeparator();
  _editToolbar->addAction(_deleteAction);

#ifdef Q_WS_MACX
  _editToolbar->addSeparator();
#endif

  // insert toolbar
  _insertToolbar = addToolBar(tr("Insert"));
  _insertToolbar->setMovable(true);
  _insertToolbar->setFloatable(true);
  _insertToolbar->setIconSize(QSize(24,24));

  _insertToolbar->addAction(_pointerAction);
  _insertToolbar->addAction(_linkAction);

  _insertToolbar->addSeparator();
  _insertToolbar->addAction(_mediaAction);
  _insertToolbar->addSeparator();
  _insertToolbar->addAction(_contextAction);
  _insertToolbar->addAction(_switchAction);

  if (STR_DEFAULT_WITH_BODY)
    _insertToolbar->addAction(_bodyAction);

  _insertToolbar->addSeparator();
  _insertToolbar->addAction(_areaAction);
  _insertToolbar->addAction(_propertyAction);
  _insertToolbar->addAction(_portAction);
  _insertToolbar->addAction(_switchportAction);

#ifdef Q_WS_MACX
  setUnifiedTitleAndToolBarOnMac(true);
#endif

  // window toolbar
  _windowToolbar = addToolBar(tr("Window"));
  _windowToolbar->setMovable(true);
  _windowToolbar->setFloatable(true);
  _windowToolbar->setIconSize(QSize(24,24));

  _windowToolbar->addAction(_zoominAction);
  _windowToolbar->addAction(_zoomoutAction);
  _windowToolbar->addAction(_zoomresetAction);
  _windowToolbar->addSeparator();
  _windowToolbar->addAction(_snapshotAction);
  _windowToolbar->addSeparator();
  _windowToolbar->addAction(_minimapAction);

#ifdef Q_WS_MACX
  _editToolbar->addSeparator();
#endif

  select("", StructuralUtil::createSettings());
}

void StructuralWindow::createWidgets()
{
  _view = new StructuralView(this);

  setCentralWidget(_view);
}

void  StructuralWindow::createStatusbar()
{
  // TODO
}

void  StructuralWindow::createConnections()
{

  connect(_undoAction, SIGNAL(triggered()), _view, SLOT(performUndo()));
  connect(_redoAction, SIGNAL(triggered()), _view, SLOT(performRedo()));
  connect(_cutAction, SIGNAL(triggered()), _view, SLOT(performCut()));
  connect(_copyAction, SIGNAL(triggered()), _view, SLOT(performCopy()));
  connect(_pasteAction, SIGNAL(triggered()), _view, SLOT(performPaste()));
  connect(_deleteAction, SIGNAL(triggered()), _view, SLOT(performDelete()));
  connect(_snapshotAction, SIGNAL(triggered()), _view, SLOT(performSnapshot()));
  connect(_zoominAction, SIGNAL(triggered()), _view, SLOT(performZoomIn()));
  connect(_zoomoutAction, SIGNAL(triggered()), _view, SLOT(performZoomOut()));
  connect(_zoomresetAction, SIGNAL(triggered()), _view, SLOT(performZoomOriginal()));
  connect(_pointerAction, SIGNAL(triggered()), _view, SLOT(performPointer()));
  connect(_linkAction, SIGNAL(triggered()), _view, SLOT(performLink()));
  connect(_minimapAction, SIGNAL(triggered()), _view, SLOT(performMinimap()));
  connect(_mediaAction, SIGNAL(triggered()), _view, SLOT(performMedia()));
  connect(_contextAction, SIGNAL(triggered()), _view, SLOT(performContext()));
  connect(_switchAction, SIGNAL(triggered()), _view, SLOT(performSwitch()));
  connect(_bodyAction, SIGNAL(triggered()), _view, SLOT(performBody()));
  connect(_areaAction, SIGNAL(triggered()), _view, SLOT(performArea()));
  connect(_propertyAction, SIGNAL(triggered()), _view, SLOT(performProperty()));
  connect(_portAction, SIGNAL(triggered()), _view, SLOT(performPort()));
  connect(_switchportAction, SIGNAL(triggered()), _view, SLOT(performSwitchPort()));

  connect(_view, SIGNAL(switchedUndo(bool)), SLOT(switchUndo(bool)));
  connect(_view, SIGNAL(switchedRedo(bool)), SLOT(switchRedo(bool)));
  connect(_view, SIGNAL(switchedCut(bool)), SLOT(switchCut(bool)));
  connect(_view, SIGNAL(switchedCopy(bool)), SLOT(switchCopy(bool)));
  connect(_view, SIGNAL(switchedPaste(bool)), SLOT(switchPaste(bool)));
  connect(_view, SIGNAL(switchedDelete(bool)), SLOT(switchDelete(bool)));
  connect(_view, SIGNAL(switchedSnapshot(bool)), SLOT(switchSnapshot(bool)));
  connect(_view, SIGNAL(switchedZoomIn(bool)), SLOT(switchZoomIn(bool)));
  connect(_view, SIGNAL(switchedZoomOut(bool)), SLOT(switchZoomOut(bool)));
  connect(_view, SIGNAL(switchedPointer(bool)), SLOT(switchPointer(bool)));
  connect(_view, SIGNAL(switchedLink(bool)), SLOT(switchLink(bool)));
  connect(_view, SIGNAL(switchedBody(bool)), SLOT(switchBody(bool)));

  connect(_view, SIGNAL(selected(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));
}

void StructuralWindow::switchUndo(bool state)
{
  _undoAction->setEnabled(state);
}

void StructuralWindow::switchRedo(bool state)
{
  _redoAction->setEnabled(state);
}

void StructuralWindow::switchCut(bool state)
{
  _cutAction->setEnabled(state);
}

void StructuralWindow::switchCopy(bool state)
{
  _copyAction->setEnabled(state);
}

void StructuralWindow::switchPaste(bool state)
{
  _pasteAction->setEnabled(state);
}

void StructuralWindow::switchDelete(bool state)
{
  _deleteAction->setEnabled(state);
}

void StructuralWindow::switchSnapshot(bool state)
{
  _snapshotAction->setEnabled(state);
}


void StructuralWindow::switchZoomIn(bool state)
{
  _zoominAction->setEnabled(state);
  _zoomresetAction->setEnabled(state);
}

void StructuralWindow::switchZoomOut(bool state)
{
  _zoomoutAction->setEnabled(state);
}

void StructuralWindow::switchPointer(bool state)
{
  if (state)
    _pointerAction->setChecked(true);
  else
    _linkAction->setChecked(true);
}

void StructuralWindow::switchLink(bool state)
{
  switchPointer(!state);
}

void StructuralWindow::switchBody(bool state)
{
  _bodyAction->setEnabled(state);
}

void StructuralWindow::select(QString uid, QMap<QString, QString> settings)
{
  Q_UNUSED(settings);

  StructuralEntity* entity = _view->getEntity(uid);

  if (entity != NULL) {
    StructuralType type = _view->getEntity(uid)->getStructuralType();

    switch (type) {
      case Structural::Media:
      {
        _mediaAction->setEnabled(false);
        _contextAction->setEnabled(false);
        _switchAction->setEnabled(false);
        _bodyAction->setEnabled(false);
        _areaAction->setEnabled(true);
        _propertyAction->setEnabled(true);
        _portAction->setEnabled(false);
        _switchportAction->setEnabled(false);

        break;
      }

      case Structural::Context:
      {
        _mediaAction->setEnabled(true);
        _contextAction->setEnabled(true);
        _switchAction->setEnabled(true);
        _bodyAction->setEnabled(false);
        _areaAction->setEnabled(false);
        _propertyAction->setEnabled(true);
        _portAction->setEnabled(true);
        _switchportAction->setEnabled(false);

        break;
      }

      case Structural::Switch:
      {
        _mediaAction->setEnabled(true);
        _contextAction->setEnabled(true);
        _switchAction->setEnabled(true);
        _bodyAction->setEnabled(false);
        _areaAction->setEnabled(false);
        _propertyAction->setEnabled(true);
        _portAction->setEnabled(false);
        _switchportAction->setEnabled(true);

        break;
      }

      case Structural::Body:
      {
        _mediaAction->setEnabled(true);
        _contextAction->setEnabled(true);
        _switchAction->setEnabled(true);
        _bodyAction->setEnabled(false);
        _areaAction->setEnabled(false);
        _propertyAction->setEnabled(true);
        _portAction->setEnabled(true);
        _switchportAction->setEnabled(false);

        break;
      }

      case Structural::Area:
      case Structural::Property:
      case Structural::Port:
      case Structural::SwitchPort:
      {
        _mediaAction->setEnabled(false);
        _contextAction->setEnabled(false);
        _switchAction->setEnabled(false);
        _bodyAction->setEnabled(false);
        _areaAction->setEnabled(false);
        _propertyAction->setEnabled(false);
        _portAction->setEnabled(false);
        _switchportAction->setEnabled(false);

        break;
      }

      default:
      {
        break;
      }
    }

    switchCut(true);
    switchCopy(true);
    switchDelete(true);

  } else  {

    _switchportAction->setEnabled(false);

    if (STR_DEFAULT_WITH_BODY)
    {
      _mediaAction->setEnabled(false);
      _contextAction->setEnabled(false);
      _switchAction->setEnabled(false);
      _bodyAction->setEnabled((_view->getBody() == NULL));
      _areaAction->setEnabled(false);
      _propertyAction->setEnabled(false);
      _portAction->setEnabled(false);
    }
    else
    {
      _mediaAction->setEnabled(true);
      _contextAction->setEnabled(true);
      _switchAction->setEnabled(true);
      _bodyAction->setEnabled(false);
      _areaAction->setEnabled(false);
      _propertyAction->setEnabled(true);
      _portAction->setEnabled(true);

    }

    switchCut(false);
    switchCopy(false);
    switchDelete(false);
  }
}

