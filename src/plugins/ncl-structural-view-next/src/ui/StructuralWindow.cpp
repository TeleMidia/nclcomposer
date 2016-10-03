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
  _undoAction->setIcon(QIcon(":/images/icon/undo"));
  _undoAction->setShortcut(QKeySequence("Ctrl+Z"));

  // redo action
  _redoAction = new QAction((QObject*) this);
  _redoAction->setEnabled(false);
  _redoAction->setText(tr("Redo"));
  _redoAction->setIcon(QIcon(":/images/icon/redo"));
  _redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

  // cut action
  _cutAction = new QAction((QObject*) this);
  _cutAction->setEnabled(false);
  _cutAction->setText(tr("Cut"));
  _cutAction->setIcon(QIcon(":/images/icon/cut"));
  _cutAction->setShortcut(QKeySequence("Ctrl+X"));

  // copy action
  _copyAction = new QAction((QObject*) this);
  _copyAction->setEnabled(false);
  _copyAction->setText(tr("Copy"));
  _copyAction->setIcon(QIcon(":/images/icon/copy"));
  _copyAction->setShortcut(QKeySequence("Ctrl+C"));

  // paste action
  _pasteAction = new QAction((QObject*) this);
  _pasteAction->setEnabled(false);
  _pasteAction->setText(tr("Paste"));
  _pasteAction->setIcon(QIcon(":/images/icon/paste"));
  _pasteAction->setShortcut(QKeySequence("Ctrl+V"));

  // zoomin action
  _zoominAction = new QAction((QObject*) this);
  _zoominAction->setEnabled(false);
  _zoominAction->setText(tr("Zoom In"));
  _zoominAction->setIcon(QIcon(":/images/icon/zoomin"));
  _zoominAction->setShortcut(QKeySequence("Ctrl++"));

  // zoomout action
  _zoomoutAction = new QAction((QObject*) this);
  _zoomoutAction->setEnabled(true);
  _zoomoutAction->setText(tr("Zoom Out"));
  _zoomoutAction->setIcon(QIcon(":/images/icon/zoomout"));
  _zoomoutAction->setShortcut(QKeySequence("Ctrl+-"));

  // pointer action
  _pointerAction = new QAction((QObject*) this);
  _pointerAction->setEnabled(true);
  _pointerAction->setCheckable(true);
  _pointerAction->setChecked(true);
  _pointerAction->setText(tr("Pointer tool"));
  _pointerAction->setToolTip(tr("Pointer tool"));
  _pointerAction->setIcon(QIcon(":/images/icon/pointer"));
  _pointerAction->setShortcut(QKeySequence("1"));

  // link action
  _linkAction = new QAction((QObject*) this);
  _linkAction->setEnabled(true);
  _linkAction->setCheckable(true);
  _linkAction->setChecked(false);
  _linkAction->setText(tr("Linking tool"));
  _linkAction->setToolTip(tr("Linking tool"));
  _linkAction->setIcon(QIcon(":/images/icon/link"));
  _linkAction->setShortcut(QKeySequence("2"));

  // media action
  _mediaAction = new QAction((QObject*) this);
  _mediaAction->setEnabled(false);
  _mediaAction->setText(tr("Media"));
  _mediaAction->setToolTip(tr("Insert &lt;media&gt; entity"));
  _mediaAction->setIcon(QIcon(":/images/icon/media-insert"));
  _mediaAction->setShortcut(QKeySequence("3"));

  // context action
  _contextAction = new QAction((QObject*) this);
  _contextAction->setEnabled(false);
  _contextAction->setText(tr("Context"));
  _contextAction->setToolTip(tr("Insert &lt;context&gt; entity"));
  _contextAction->setIcon(QIcon(":/images/icon/context-insert"));
  _contextAction->setShortcut(QKeySequence("4"));

  // switch action
  _switchAction = new QAction((QObject*) this);
  _switchAction->setEnabled(false);
  _switchAction->setText(tr("Switch"));
  _switchAction->setToolTip(tr("Insert &lt;switch&gt; entity"));
  _switchAction->setIcon(QIcon(":/images/icon/switch-insert"));
  _switchAction->setShortcut(QKeySequence("5"));

  // body action
  _bodyAction = new QAction((QObject*) this);
  _bodyAction->setEnabled(true);
  _bodyAction->setText(tr("Body"));
  _bodyAction->setToolTip(tr("Insert &lt;body&gt; entity"));
  _bodyAction->setIcon(QIcon(":/images/icon/body-insert"));
  _bodyAction->setShortcut(QKeySequence("6"));

  // area action
  _areaAction = new QAction((QObject*) this);
  _areaAction->setEnabled(false);
  _areaAction->setText(tr("Area"));
  _areaAction->setToolTip(tr("Insert &lt;area&gt; entity"));
  _areaAction->setIcon(QIcon(":/images/icon/area-insert"));
  _areaAction->setShortcut(QKeySequence("7"));

  // property action
  _propertyAction = new QAction((QObject*) this);
  _propertyAction->setEnabled(false);
  _propertyAction->setText(tr("Property"));
  _propertyAction->setToolTip(tr("Insert &lt;property&gt; entity"));
  _propertyAction->setIcon(QIcon(":/images/icon/property-insert"));
  _propertyAction->setShortcut(QKeySequence("8"));

  // port action
  _portAction = new QAction((QObject*) this);
  _portAction->setEnabled(false);
  _portAction->setText(tr("Port"));
  _portAction->setToolTip(tr("Insert &lt;port&gt; entity"));
  _portAction->setIcon(QIcon(":/images/icon/port-insert"));
  _portAction->setShortcut(QKeySequence("9"));

  // switchport action
  _switchportAction = new QAction((QObject*) this);
  _switchportAction->setEnabled(false);
  _switchportAction->setText(tr("Switch Port"));
  _switchportAction->setToolTip(tr("Insert &lt;switchport&gt; entity"));
  _switchportAction->setIcon(QIcon(":/images/icon/switchport-insert"));
  _switchportAction->setShortcut(QKeySequence("0"));

  // minimap action
  _minimapAction = new QAction((QObject*) this);
  _minimapAction->setEnabled(true);
  _minimapAction->setCheckable(true);
  _minimapAction->setChecked(false);
  _minimapAction->setText(tr("Minimap"));
  _minimapAction->setToolTip(tr("Show/Hide minimap"));
  _minimapAction->setIcon(QIcon(":/images/icon/minimap"));
  _minimapAction->setShortcut(QKeySequence("Ctrl+M"));

  // action
  _snapshotAction = new QAction((QObject*) this);
  _snapshotAction->setEnabled(true);
  _snapshotAction->setText(tr("Snapshot"));
  _snapshotAction->setToolTip(tr("Take a snapshot of the structural view"));
  _snapshotAction->setIcon(QIcon(":/images/icon/snapshot"));

  // preferences action
  _preferencesAction = new QAction((QObject*) this);
  _preferencesAction->setEnabled(true);
  _preferencesAction->setText(tr("Preferences..."));
  _preferencesAction->setIcon(QIcon(":/images/icon/preferences"));
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
  _editToolbar->setIconSize(QSize(32,32));

  _editToolbar->addAction(_undoAction);
  _editToolbar->addAction(_redoAction);
  _editToolbar->addSeparator();
  _editToolbar->addAction(_cutAction);
  _editToolbar->addAction(_copyAction);
  _editToolbar->addAction(_pasteAction);
  _editToolbar->addSeparator();
  _editToolbar->addAction(_zoominAction);
  _editToolbar->addAction(_zoomoutAction);

#ifdef Q_WS_MACX
  _editToolbar->addSeparator();
#endif

  // insert toolbar
  _insertToolbar = addToolBar(tr("Insert"));
  _insertToolbar->setMovable(true);
  _insertToolbar->setFloatable(true);
  _insertToolbar->setIconSize(QSize(32,32));

  _insertToolbar->addAction(_pointerAction);
  _insertToolbar->addAction(_linkAction);

  _insertToolbar->addSeparator();
  _insertToolbar->addAction(_mediaAction);
  _insertToolbar->addSeparator();
  _insertToolbar->addAction(_contextAction);
  _insertToolbar->addAction(_switchAction);
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
  _windowToolbar->setIconSize(QSize(32,32));

  _windowToolbar->addAction(_minimapAction);
  _windowToolbar->addAction(_snapshotAction);

#ifdef Q_WS_MACX
  _editToolbar->addSeparator();
#endif

  changeSelect("NULL");
}

void StructuralWindow::createWidgets()
{
  _view = new StructuralView(this);
  _view->setMiniMapVisible(false);

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
  connect(_zoominAction, SIGNAL(triggered()), _view, SLOT(performZoomIn()));
  connect(_zoomoutAction, SIGNAL(triggered()), _view, SLOT(performZoomOut()));
  connect(_pointerAction, SIGNAL(triggered()), _view, SLOT(performPointer()));
  connect(_linkAction, SIGNAL(triggered()), _view, SLOT(performLink()));
  connect(_snapshotAction, SIGNAL(triggered()), _view, SLOT(performSnapshot()));
  connect(_minimapAction, SIGNAL(triggered()), _view, SLOT(performMinimap()));
  connect(_mediaAction, SIGNAL(triggered()), _view, SLOT(performMedia()));
  connect(_contextAction, SIGNAL(triggered()), _view, SLOT(performContext()));
  connect(_switchAction, SIGNAL(triggered()), _view, SLOT(performSwitch()));
  connect(_bodyAction, SIGNAL(triggered()), _view, SLOT(performBody()));
  connect(_areaAction, SIGNAL(triggered()), _view, SLOT(performArea()));
  connect(_propertyAction, SIGNAL(triggered()), _view, SLOT(performProperty()));
  connect(_portAction, SIGNAL(triggered()), _view, SLOT(performPort()));
  connect(_switchportAction, SIGNAL(triggered()), _view, SLOT(performSwitchPort()));

  /* TODO: Refactoring */
  connect(_view, SIGNAL(inserted(QString,QString,QMap<QString,QString>,QMap<QString,QString>)),SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
  connect(_view, SIGNAL(removed(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
  connect(_view, SIGNAL(changed(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
  connect(_view, SIGNAL(selected(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

  connect(_view, SIGNAL(switchedUndo(bool)), SLOT(changeUndoState(bool)));
  connect(_view, SIGNAL(switchedRedo(bool)), SLOT(changeRedoState(bool)));
  connect(_view, SIGNAL(switchedCut(bool)), SLOT(changeCutState(bool)));
  connect(_view, SIGNAL(switchedCopy(bool)), SLOT(changeCopyState(bool)));
  connect(_view, SIGNAL(switchedPaste(bool)), SLOT(changePasteState(bool)));
  connect(_view, SIGNAL(switchedZoomIn(bool)), SLOT(changeZoomInState(bool)));
  connect(_view, SIGNAL(switchedZoomOut(bool)), SLOT(switchZoomOut(bool)));

  connect(_view, SIGNAL(switchedBody(bool)), SLOT(changeBodyState(bool)));

  connect(_view, SIGNAL(selectChange(QString)), SLOT(changeSelect(QString)));

  connect(_view, SIGNAL(switchedPointer(bool)), SLOT(switchPointer(bool)));
  connect(_view, SIGNAL(switchedLink(bool)), SLOT(changeLinkState(bool)));
  /* TODO: Refactoring */
}

void StructuralWindow::changeBodyState(bool enable)
{
  _bodyAction->setEnabled(enable);

//  _view->getScene()->_menu->_bodyAction->setEnabled(enable);
}

void StructuralWindow::changeZoomInState(bool enable)
{
  _zoominAction->setEnabled(enable);
}

void StructuralWindow::switchZoomOut(bool enable)
{
  _zoomoutAction->setEnabled(enable);
}

void StructuralWindow::changeUndoState(bool enable)
{
  _undoAction->setEnabled(enable);

//  _view->getScene()->_menu->_undoAction->setEnabled(enable);

//  foreach (StructuralEntity* e, _view->getEntities()) {
//    e->_menu->_undoAction->setEnabled(enable);
//  }

}

void StructuralWindow::changeRedoState(bool enable)
{
  _redoAction->setEnabled(enable);

//  _view->getScene()->_menu->_redoAction->setEnabled(enable);

//  foreach (StructuralEntity* e, _view->getEntities()) {
//    e->_menu->_redoAction->setEnabled(enable);
//  }

}

void StructuralWindow::changeCutState(bool enable)
{
  _cutAction->setEnabled(enable);

//  _view->getScene()->_menu->_cutAction->setEnabled(enable);

//  foreach (StructuralEntity* e, _view->getEntities()) {
//    e->_menu->_cutAction->setEnabled(enable);
//  }
}

void StructuralWindow::switchPointer(bool enable)
{
  if (enable)
    _pointerAction->setChecked(true);
  else
    _linkAction->setChecked(true);
}

void StructuralWindow::changeLinkState(bool enable)
{
  switchPointer(!enable);
}

void StructuralWindow::changeCopyState(bool enable)
{
  _copyAction->setEnabled(enable);

//  _view->getScene()->_menu->_copyAction->setEnabled(enable);

//  foreach (StructuralEntity* e, _view->getEntities()) {
//    e->_menu->_copyAction->setEnabled(enable);
//  }
}

void StructuralWindow::changePasteState(bool enable)
{
  _pasteAction->setEnabled(enable);

//  _view->getScene()->_menu->_pasteAction->setEnabled(enable);

//  foreach (StructuralEntity* e, _view->getEntities()) {
//    e->_menu->_pasteAction->setEnabled(enable);
//  }
}


void StructuralWindow::insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings)
{
//  StructuralEntity* entity = _view->getEntity(uid);

//  if (entity)
//  {
//    StructuralType NAME = _view->getEntity(uid)->getStructuralType();

//    switch (NAME) {
//      case Structural::Body:
//        _bodyAction->setEnabled(false);
//        _view->getScene()->_menu->_bodyAction->setEnabled(false);
//        break;

//      default:
//        break;
//    }
//  }
}

void StructuralWindow::remove(QString uid, QMap<QString, QString> settings)
{
//  StructuralEntity* entity = _view->getEntity(uid);

//  if (entity)
//  {
//    StructuralType NAME = _view->getEntity(uid)->getStructuralType();

//    switch (NAME) {
//      case Structural::Body:
//        _bodyAction->setEnabled(true);
//        break;

//      default:
//        break;
//    }
//  }
}

void StructuralWindow::change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings)
{
//  StructuralEntity* entity = _view->getEntity(uid);

//  if (entity)
//  {
//    StructuralType NAME = _view->getEntity(uid)->getStructuralType();

//    switch (NAME) {
//      case Structural::Body:
//        _bodyAction->setEnabled(false);
//        break;

//      default:
//        break;
//    }
//  }
}

void StructuralWindow::changeSelect(QString uid)
{
  StructuralEntity* entity = _view->getEntity(uid);

  if (entity != NULL)
  {

    StructuralType NAME = _view->getEntity(uid)->getStructuralType();

//    entity->_menu->adjust(NAME);

    switch (NAME) {
      case Structural::Media:
        _mediaAction->setEnabled(false);
        _contextAction->setEnabled(false);
        _switchAction->setEnabled(false);
        _bodyAction->setEnabled(false);
        _areaAction->setEnabled(true);
        _propertyAction->setEnabled(true);
        _portAction->setEnabled(false);
        _switchportAction->setEnabled(false);


        break;

      case Structural::Context:
        _mediaAction->setEnabled(true);
        _contextAction->setEnabled(true);
        _switchAction->setEnabled(true);
        _bodyAction->setEnabled(false);
        _areaAction->setEnabled(false);
        _propertyAction->setEnabled(true);
        _portAction->setEnabled(true);
        _switchportAction->setEnabled(false);
        break;

      case Structural::Switch:
        _mediaAction->setEnabled(true);
        _contextAction->setEnabled(true);
        _switchAction->setEnabled(true);
        _bodyAction->setEnabled(false);
        _areaAction->setEnabled(false);
        _propertyAction->setEnabled(true);
        _portAction->setEnabled(false);
        _switchportAction->setEnabled(true);
        break;

      case Structural::Body:
        _mediaAction->setEnabled(true);
        _contextAction->setEnabled(true);
        _switchAction->setEnabled(true);
        _bodyAction->setEnabled(false);
        _areaAction->setEnabled(false);
        _propertyAction->setEnabled(true);
        _portAction->setEnabled(true);
        _switchportAction->setEnabled(false);
        break;

      case Structural::Area:
      case Structural::Property:
      case Structural::Port:
      case Structural::SwitchPort:
        _mediaAction->setEnabled(false);
        _contextAction->setEnabled(false);
        _switchAction->setEnabled(false);
        _bodyAction->setEnabled(false);
        _areaAction->setEnabled(false);
        _propertyAction->setEnabled(false);
        _portAction->setEnabled(false);
        _switchportAction->setEnabled(false);
        break;

      default:
        break;
    }

    changeCutState(true);
    changeCopyState(true);
  }
  else
  {
    _mediaAction->setEnabled(false);
    _contextAction->setEnabled(false);
    _switchAction->setEnabled(false);
    _bodyAction->setEnabled((_view->getBody() == NULL));
    _areaAction->setEnabled(false);
    _propertyAction->setEnabled(false);
    _portAction->setEnabled(false);

    changeCutState(false);
    changeCopyState(false);
  }
}

void StructuralWindow::select(QString uid, QMap<QString, QString> settings)
{
  changeSelect(uid);
}
