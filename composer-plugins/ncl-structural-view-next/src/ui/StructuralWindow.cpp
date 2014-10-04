#include "StructuralWindow.h"

StructuralWindow::StructuralWindow(QWidget* parent)
  : QMainWindow(parent)
{
  createWidgets();
  createActions();
  createMenus();
  createToolbar();
  createStatusbar();
  createConnections();
}

StructuralWindow::~StructuralWindow()
{

}

StructuralView* StructuralWindow::getView()
{
  return _view;
}

void StructuralWindow::setView(StructuralView* view)
{
  _view = view;
}

void StructuralWindow::createActions()
{
  // new action
  _newAction = new QAction(this);
  _newAction->setEnabled(true);
  _newAction->setText(tr("New"));
  _newAction->setIcon(QIcon(":/images/icon/new"));
  _newAction->setShortcut(QKeySequence("Ctrl+N"));

  // open action
  _openAction = new QAction(this);
  _openAction->setEnabled(true);
  _openAction->setText(tr("Open..."));
  _openAction->setIcon(QIcon(":/images/icon/open"));
  _openAction->setShortcut(QKeySequence("Ctrl+O"));

  // save action
  _saveAction = new QAction(this);
  _saveAction->setEnabled(true);
  _saveAction->setText(tr("Save"));
  _saveAction->setIcon(QIcon(":/images/icon/save"));
  _saveAction->setShortcut(QKeySequence("Ctrl+S"));

  // saveas action
  _saveAsAction = new QAction(this);
  _saveAsAction->setEnabled(true);
  _saveAsAction->setText(tr("Save As..."));
  _saveAsAction->setIcon(QIcon(":/images/icon/saveas"));
  _saveAsAction->setShortcut(QKeySequence("Shift+Ctrl+S"));

  // close action
  _closeAction = new QAction(this);
  _closeAction->setEnabled(true);
  _closeAction->setText(tr("Close"));
  _closeAction->setShortcut(QKeySequence("Ctrl+W"));

  // quit action
  _quitAction = new QAction(this);
  _quitAction->setEnabled(true);
  _quitAction->setText(tr("Quit"));
  _quitAction->setShortcut(QKeySequence("Ctrl+Q"));

  // import action
  _importAction = new QAction(this);
  _importAction->setEnabled(true);
  _importAction->setText(tr("Import..."));
  _importAction->setShortcut(QKeySequence("Ctrl+R"));

  // export action
  _exportAction = new QAction(this);
  _exportAction->setEnabled(true);
  _exportAction->setText(tr("Export..."));
  _exportAction->setShortcut(QKeySequence("Ctrl+Shift+R"));

  // undo action
  _undoAction = new QAction(this);
  _undoAction->setEnabled(true);
  _undoAction->setText(tr("Undo"));
  _undoAction->setIcon(QIcon(":/images/icon/undo"));
  _undoAction->setShortcut(QKeySequence("Ctrl+Z"));

  // redo action
  _redoAction = new QAction(this);
  _redoAction->setEnabled(true);
  _redoAction->setText(tr("Redo"));
  _redoAction->setIcon(QIcon(":/images/icon/redo"));
  _redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

  // cut action
  _cutAction = new QAction(this);
  _cutAction->setEnabled(true);
  _cutAction->setText(tr("Cut"));
  _cutAction->setIcon(QIcon(":/images/icon/cut"));
  _cutAction->setShortcut(QKeySequence("Ctrl+X"));

  // copy action
  _copyAction = new QAction(this);
  _copyAction->setEnabled(true);
  _copyAction->setText(tr("Copy"));
  _copyAction->setIcon(QIcon(":/images/icon/copy"));
  _copyAction->setShortcut(QKeySequence("Ctrl+C"));

  // paste action
  _pasteAction = new QAction(this);
  _pasteAction->setEnabled(false);
  _pasteAction->setText(tr("Paste"));
  _pasteAction->setIcon(QIcon(":/images/icon/paste"));
  _pasteAction->setShortcut(QKeySequence("Ctrl+V"));

  // pointer action
  _pointerAction = new QAction(this);
  _pointerAction->setEnabled(true);
  _pointerAction->setCheckable(true);
  _pointerAction->setChecked(true);
  _pointerAction->setText(tr("Selection tool"));
  _pointerAction->setToolTip(tr("Selection tool"));
  _pointerAction->setIcon(QIcon(":/images/icon/pointer"));
  _pointerAction->setShortcut(QKeySequence("1"));

  // link action
  _linkAction = new QAction(this);
  _linkAction->setEnabled(true);
  _linkAction->setCheckable(true);
  _linkAction->setChecked(false);
  _linkAction->setText(tr("Link"));
  _linkAction->setToolTip(tr("Link tool"));
  _linkAction->setIcon(QIcon(":/images/icon/link"));
  _linkAction->setShortcut(QKeySequence("2"));

  // media action
  _mediaAction = new QAction(this);
  _mediaAction->setEnabled(false);
  _mediaAction->setText(tr("Media"));
  _mediaAction->setToolTip(tr("Insert &lt;media&gt; tool"));
  _mediaAction->setIcon(QIcon(":/images/icon/media-insert"));
  _mediaAction->setShortcut(QKeySequence("3"));

  // context action
  _contextAction = new QAction(this);
  _contextAction->setEnabled(false);
  _contextAction->setText(tr("Context"));
  _contextAction->setToolTip(tr("Insert &lt;context&gt; tool"));
  _contextAction->setIcon(QIcon(":/images/icon/context-insert"));
  _contextAction->setShortcut(QKeySequence("4"));

  // switch action
  _switchAction = new QAction(this);
  _switchAction->setEnabled(false);
  _switchAction->setText(tr("Switch"));
  _switchAction->setToolTip(tr("Insert &lt;switch&gt; tool"));
  _switchAction->setIcon(QIcon(":/images/icon/switch-insert"));
  _switchAction->setShortcut(QKeySequence("5"));

  // body action
  _bodyAction = new QAction(this);
  _bodyAction->setEnabled(true);
  _bodyAction->setText(tr("Body"));
  _bodyAction->setToolTip(tr("Insert &lt;body&gt; tool"));
  _bodyAction->setIcon(QIcon(":/images/icon/body-insert"));
  _bodyAction->setShortcut(QKeySequence("6"));

  // area action
  _areaAction = new QAction(this);
  _areaAction->setEnabled(false);
  _areaAction->setText(tr("Area"));
  _areaAction->setToolTip(tr("Insert &lt;area&gt; tool"));
  _areaAction->setIcon(QIcon(":/images/icon/area-insert"));
  _areaAction->setShortcut(QKeySequence("7"));

  // property action
  _propertyAction = new QAction(this);
  _propertyAction->setEnabled(false);
  _propertyAction->setText(tr("Property"));
  _propertyAction->setToolTip(tr("Insert &lt;property&gt; tool"));
  _propertyAction->setIcon(QIcon(":/images/icon/property-insert"));
  _propertyAction->setShortcut(QKeySequence("8"));

  // port action
  _portAction = new QAction(this);
  _portAction->setEnabled(false);
  _portAction->setText(tr("Port"));
  _portAction->setToolTip(tr("Insert &lt;port&gt; tool"));
  _portAction->setIcon(QIcon(":/images/icon/port-insert"));
  _portAction->setShortcut(QKeySequence("9"));

  // switchport action
  _switchportAction = new QAction(this);
  _switchportAction->setEnabled(false);
  _switchportAction->setText(tr("Switch Port"));
  _switchportAction->setToolTip(tr("Insert &lt;switchport&gt; tool"));
  _switchportAction->setIcon(QIcon(":/images/icon/switchport-insert"));
  _switchportAction->setShortcut(QKeySequence("0"));

  // preferences action
  _preferencesAction = new QAction(this);
  _preferencesAction->setEnabled(true);
  _preferencesAction->setText(tr("Preferences..."));
  _preferencesAction->setIcon(QIcon(":/images/icon/preferences"));
  _preferencesAction->setShortcut(QKeySequence("Ctrl+P"));

  // report action
  _reportAction = new QAction(this);
  _reportAction->setEnabled(true);
  _reportAction->setText(tr("Report Bug..."));
  _reportAction->setIcon(QIcon(":/images/icon/report"));

  // about action
  _aboutAction = new QAction(this);
  _aboutAction->setEnabled(true);
  _aboutAction->setText(tr("About..."));

  // draw group
  _insertActionGroup = new QActionGroup(this);
  _insertActionGroup->setExclusive(true);

  _insertActionGroup->addAction(_pointerAction);
  _insertActionGroup->addAction(_linkAction);
}

void StructuralWindow::createMenus()
{
#ifndef PLUGIN
  // file menu
  _fileMenu = menuBar()->addMenu(tr("File"));
  _fileMenu->addAction(_newAction);
  _fileMenu->addAction(_openAction);
  _fileMenu->addSeparator();
  _fileMenu->addAction(_saveAction);
  _fileMenu->addAction(_saveAsAction);
  _fileMenu->addSeparator();
  _fileMenu->addAction(_exportAction);
  _fileMenu->addAction(_importAction);
  _fileMenu->addSeparator();
  _fileMenu->addAction(_closeAction);
  _fileMenu->addSeparator();
  _fileMenu->addAction(_quitAction);

  // edit menu
  _editMenu = menuBar()->addMenu(tr("Edit"));
  _editMenu->addAction(_undoAction);
  _editMenu->addAction(_redoAction);
  _editMenu->addSeparator();
  _editMenu->addAction(_cutAction);
  _editMenu->addAction(_copyAction);
  _editMenu->addAction(_pasteAction);

  // insert menu
  _insertMenu = menuBar()->addMenu(tr("Insert"));

  // window menu
  _windowMenu = menuBar()->addMenu(tr("Window"));
  _windowMenu->addAction(_preferencesAction);

  // help menu
  _helpMenu = menuBar()->addMenu(tr("Help"));
  _helpMenu->addAction(_reportAction);
  _helpMenu->addSeparator();
  _helpMenu->addAction(_aboutAction);
#endif
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
}

void StructuralWindow::createWidgets()
{
  _view = new StructuralView(this); setCentralWidget(_view);
}

void  StructuralWindow::createStatusbar()
{
  // TODO
}

void  StructuralWindow::createConnections()
{
  connect(_newAction, SIGNAL(triggered()), SLOT(performNew()));
  connect(_openAction, SIGNAL(triggered()), SLOT(performOpen()));
  connect(_saveAction, SIGNAL(triggered()), SLOT(performSave()));
  connect(_saveAsAction, SIGNAL(triggered()), SLOT(performSaveAs()));
  connect(_exportAction, SIGNAL(triggered()), SLOT(performExport()));
  connect(_importAction, SIGNAL(triggered()), SLOT(performImport()));
  connect(_closeAction, SIGNAL(triggered()), SLOT(performClose()));
  connect(_quitAction, SIGNAL(triggered()), SLOT(performQuit()));

  connect(_undoAction, SIGNAL(triggered()), SLOT(performUndo()));
  connect(_redoAction, SIGNAL(triggered()), SLOT(performRedo()));

  connect(_cutAction, SIGNAL(triggered()), SLOT(performCut()));
  connect(_copyAction, SIGNAL(triggered()), SLOT(performCopy()));
  connect(_pasteAction, SIGNAL(triggered()), SLOT(performPaste()));

  connect(_preferencesAction, SIGNAL(triggered()), SLOT(performPreferences()));

  connect(_reportAction, SIGNAL(triggered()), SLOT(performReport()));
  connect(_aboutAction, SIGNAL(triggered()), SLOT(performAbout()));

  connect(_pointerAction, SIGNAL(triggered()), SLOT(performPointer()));
  connect(_linkAction, SIGNAL(triggered()), SLOT(performLink()));

  connect(_bodyAction, SIGNAL(triggered()), SLOT(performBody()));
  connect(_contextAction, SIGNAL(triggered()), SLOT(performContext()));
  connect(_switchAction, SIGNAL(triggered()), SLOT(performSwitch()));

  connect(_mediaAction, SIGNAL(triggered()), SLOT(performMedia()));
  connect(_portAction, SIGNAL(triggered()), SLOT(performPort()));
  connect(_switchportAction, SIGNAL(triggered()), SLOT(performSwitchPort()));
  connect(_areaAction, SIGNAL(triggered()), SLOT(performArea()));
  connect(_propertyAction, SIGNAL(triggered()), SLOT(performProperty()));

  connect(_view, SIGNAL(inserted(QString,QString,QMap<QString,QString>,QMap<QString,QString>)),SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
  connect(_view, SIGNAL(removed(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
  connect(_view, SIGNAL(changed(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
  connect(_view, SIGNAL(selected(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));
}

void StructuralWindow::performNew()
{
  qDebug() << "[QNST]" << ":" << "Performing 'New' action";

  // TODO
}

void StructuralWindow::performOpen()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Open' action";

  // TODO
}

void StructuralWindow::performSave()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Save' action";

  // TODO
}

void StructuralWindow::performSaveAs()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Save As' action";

  // TODO
}


void StructuralWindow::performExport()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Export' action";

  // TODO
}

void StructuralWindow::performImport()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Import' action";

  // TODO
}

void StructuralWindow::performClose()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Close' action";

  // TODO
}

void StructuralWindow::performQuit()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Quit' action";

  // TODO
}

void StructuralWindow::performUndo()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Undo' action";

  _view->performUndo();

}

void StructuralWindow::performRedo()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Redo' action";

  _view->performRedo();
}

void StructuralWindow::performCut()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Cut' action";

  if (_view->performCut())
    _pasteAction->setEnabled(true);
}

void StructuralWindow::performCopy()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Copy' action";

  if (_view->performCopy())
    _pasteAction->setEnabled(true);
}

void StructuralWindow::performPaste()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Paste' action";

  _view->performPaste();
}

void StructuralWindow::performPointer()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Pointer' action";

  // TODO
}

void StructuralWindow::performLink()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Link' action";

  // TODO
}

void StructuralWindow::performBody()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Body' action";

  QMap<QString,QString> properties;
  properties["LOCAL:NAME"] = "body";

  QMap<QString,QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = StructuralUtil::createUid();

  _view->create(Structural::Body, properties, settings);
}

void StructuralWindow::performContext()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Context' action";

  QMap<QString,QString> properties;
  properties["LOCAL:NAME"] = "context";

  QMap<QString,QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = StructuralUtil::createUid();

  _view->create(Structural::Context, properties, settings);
}

void StructuralWindow::performSwitch()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Switch' action";

  QMap<QString,QString> properties;
  properties["LOCAL:NAME"] = "switch";

  QMap<QString,QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = StructuralUtil::createUid();

  _view->create(Structural::Switch, properties, settings);
}

void StructuralWindow::performMedia()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Media' action";

  QMap<QString,QString> properties;
  properties["LOCAL:NAME"] = "media";

  QMap<QString,QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = StructuralUtil::createUid();

  _view->create(Structural::Media, properties, settings);
}

void StructuralWindow::performPort()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Port' action";

  QMap<QString,QString> properties;
  properties["LOCAL:NAME"] = "port";

  QMap<QString,QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = StructuralUtil::createUid();

  _view->create(Structural::Port, properties, settings);
}

void StructuralWindow::performArea()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Area' action";

  QMap<QString,QString> properties;
  properties["LOCAL:NAME"] = "area";

  QMap<QString,QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = StructuralUtil::createUid();

  _view->create(Structural::Area, properties, settings);
}

void StructuralWindow::performSwitchPort()
{
  qDebug() << "[QNST]" << ":" << "Performing 'SwitchPort' action";

  QMap<QString,QString> properties;
  properties["LOCAL:NAME"] = "switchPort";

  QMap<QString,QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = StructuralUtil::createUid();

  _view->create(Structural::SwitchPort, properties, settings);
}

void StructuralWindow::performProperty()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Property' action";

  QMap<QString,QString> properties;
  properties["LOCAL:NAME"] = "property";

  QMap<QString,QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = StructuralUtil::createUid();

  _view->create(Structural::Property, properties, settings);
}

void StructuralWindow::performPreferences()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Preferences' action";

  // TODO
}

void StructuralWindow::performReport()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Report' action";

  // TODO
}

void StructuralWindow::performAbout()
{
  qDebug() << "[QNST]" << ":" << "Performing 'About' action";

  // TODO
}

void StructuralWindow::insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings)
{
  StructuralEntity* entity = _view->getEntity(uid);

  if (entity)
  {
    QnstName NAME = _view->getEntity(uid)->getnstSubtype();

    switch (NAME) {
      case Structural::Body:
        _bodyAction->setEnabled(false);
        break;

      default:
        break;
    }
  }
}

void StructuralWindow::remove(QString uid, QMap<QString, QString> settings)
{
  StructuralEntity* entity = _view->getEntity(uid);

  if (entity)
  {
    QnstName NAME = _view->getEntity(uid)->getnstSubtype();

    switch (NAME) {
      case Structural::Body:
        _bodyAction->setEnabled(true);
        break;

      default:
        break;
    }
  }
}

void StructuralWindow::change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings)
{
  StructuralEntity* entity = _view->getEntity(uid);

  if (entity)
  {
    QnstName NAME = _view->getEntity(uid)->getnstSubtype();

    switch (NAME) {
      case Structural::Body:
        _bodyAction->setEnabled(false);
        break;

      default:
        break;
    }
  }
}

void StructuralWindow::select(QString uid, QMap<QString, QString> settings)
{
  StructuralEntity* entity = _view->getEntity(uid);

  if (entity)
  {
    QnstName NAME = _view->getEntity(uid)->getnstSubtype();

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
  }
  else
  {
    _mediaAction->setEnabled(false);
    _contextAction->setEnabled(false);
    _switchAction->setEnabled(false);
    _bodyAction->setEnabled(_bodyAction->isEnabled());
    _areaAction->setEnabled(false);
    _propertyAction->setEnabled(false);
    _portAction->setEnabled(false);
    _switchAction->setEnabled(false);
  }
}
