#include "MainWindow.h"

QnstMainWindow::QnstMainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  createWidgets();
  createActions();
  createToolbar();
  createStatusbar();
  createConnections();
}

QnstMainWindow::~QnstMainWindow()
{

}

void QnstMainWindow::createActions()
{
  // new action
  newAction = new QAction(this);
  newAction->setEnabled(true);
  newAction->setText(tr("New"));
  newAction->setIcon(QIcon(":/images/icon/new"));
  newAction->setShortcut(QKeySequence("Ctrl+N"));

  // open action
  openAction = new QAction(this);
  openAction->setEnabled(true);
  openAction->setText(tr("Open..."));
  openAction->setIcon(QIcon(":/images/icon/open"));
  openAction->setShortcut(QKeySequence("Ctrl+O"));

  // save action
  saveAction = new QAction(this);
  saveAction->setEnabled(true);
  saveAction->setText(tr("Save"));
  saveAction->setIcon(QIcon(":/images/icon/save"));
  saveAction->setShortcut(QKeySequence("Ctrl+S"));

  // saveas action
  saveAsAction = new QAction(this);
  saveAsAction->setEnabled(true);
  saveAsAction->setText(tr("Save As..."));
  saveAsAction->setIcon(QIcon(":/images/icon/saveas"));
  saveAsAction->setShortcut(QKeySequence("Shift+Ctrl+S"));

  // close action
  closeAction = new QAction(this);
  closeAction->setEnabled(true);
  closeAction->setText(tr("Close"));
  closeAction->setShortcut(QKeySequence("Ctrl+W"));

  // quit action
  quitAction = new QAction(this);
  quitAction->setEnabled(true);
  quitAction->setText(tr("Quit"));
  quitAction->setShortcut(QKeySequence("Ctrl+Q"));

  // import action
  importAction = new QAction(this);
  importAction->setEnabled(true);
  importAction->setText(tr("Import..."));
  importAction->setShortcut(QKeySequence("Ctrl+R"));

  // export action
  exportAction = new QAction(this);
  exportAction->setEnabled(true);
  exportAction->setText(tr("Export..."));
  exportAction->setShortcut(QKeySequence("Ctrl+Shift+R"));

  // undo action
  undoAction = new QAction(this);
  undoAction->setEnabled(true);
  undoAction->setText(tr("Undo"));
  undoAction->setIcon(QIcon(":/images/icon/undo"));
  undoAction->setShortcut(QKeySequence("Ctrl+Z"));

  // redo action
  redoAction = new QAction(this);
  redoAction->setEnabled(true);
  redoAction->setText(tr("Redo"));
  redoAction->setIcon(QIcon(":/images/icon/redo"));
  redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

  // cut action
  cutAction = new QAction(this);
  cutAction->setEnabled(true);
  cutAction->setText(tr("Cut"));
  cutAction->setIcon(QIcon(":/images/icon/cut"));
  cutAction->setShortcut(QKeySequence("Ctrl+X"));

  // copy action
  copyAction = new QAction(this);
  copyAction->setEnabled(true);
  copyAction->setText(tr("Copy"));
  copyAction->setIcon(QIcon(":/images/icon/copy"));
  copyAction->setShortcut(QKeySequence("Ctrl+C"));

  // paste action
  pasteAction = new QAction(this);
  pasteAction->setEnabled(true);
  pasteAction->setText(tr("Paste"));
  pasteAction->setIcon(QIcon(":/images/icon/paste"));
  pasteAction->setShortcut(QKeySequence("Ctrl+V"));

  // pointer action
  pointerAction = new QAction(this);
  pointerAction->setEnabled(true);
  pointerAction->setCheckable(true);
  pointerAction->setChecked(true);
  pointerAction->setText(tr("Pointer"));
  pointerAction->setToolTip(tr("Pointer tool"));
  pointerAction->setIcon(QIcon(":/images/icon/pointer"));
  pointerAction->setShortcut(QKeySequence("1"));

  // link action
  linkAction = new QAction(this);
  linkAction->setEnabled(true);
  linkAction->setCheckable(true);
  linkAction->setChecked(false);
  linkAction->setText(tr("Link"));
  linkAction->setToolTip(tr("Link tool"));
  linkAction->setIcon(QIcon(":/images/icon/link"));
  linkAction->setShortcut(QKeySequence("2"));

  // media action
  mediaAction = new QAction(this);
  mediaAction->setEnabled(true);
  mediaAction->setText(tr("Media"));
  mediaAction->setToolTip(tr("Insert a &lt;media&gt; entity"));
  mediaAction->setIcon(QIcon(":/images/icon/media-plus"));
  mediaAction->setShortcut(QKeySequence("3"));

  // context action
  contextAction = new QAction(this);
  contextAction->setEnabled(true);
  contextAction->setText(tr("Context"));
  contextAction->setToolTip(tr("Insert a &lt;context&gt; entity"));
  contextAction->setIcon(QIcon(":/images/icon/context-plus"));
  contextAction->setShortcut(QKeySequence("4"));

  // switch action
  switchAction = new QAction(this);
  switchAction->setEnabled(true);
  switchAction->setText(tr("Switch"));
  switchAction->setToolTip(tr("Insert a &lt;switch&gt; entity"));
  switchAction->setIcon(QIcon(":/images/icon/switch-plus"));
  switchAction->setShortcut(QKeySequence("5"));

  // body action
  bodyAction = new QAction(this);
  bodyAction->setEnabled(true);
  bodyAction->setText(tr("Body"));
  bodyAction->setToolTip(tr("Insert a &lt;body&gt; entity"));
  bodyAction->setIcon(QIcon(":/images/icon/body-plus"));
  bodyAction->setShortcut(QKeySequence("6"));

  // area action
  areaAction = new QAction(this);
  areaAction->setEnabled(true);
  areaAction->setText(tr("Area"));
  areaAction->setToolTip(tr("Insert a &lt;area&gt; entity"));
  areaAction->setIcon(QIcon(":/images/icon/area-plus"));
  areaAction->setShortcut(QKeySequence("7"));

  // property action
  propertyAction = new QAction(this);
  propertyAction->setEnabled(true);
  propertyAction->setText(tr("Property"));
  propertyAction->setToolTip(tr("Insert a &lt;property&gt; entity"));
  propertyAction->setIcon(QIcon(":/images/icon/property-plus"));
  propertyAction->setShortcut(QKeySequence("8"));

  // port action
  portAction = new QAction(this);
  portAction->setEnabled(true);
  portAction->setText(tr("Port"));
  portAction->setToolTip(tr("Insert a &lt;port&gt; entity"));
  portAction->setIcon(QIcon(":/images/icon/port-plus"));
  portAction->setShortcut(QKeySequence("9"));

  // switchport action
  switchPortAction = new QAction(this);
  switchPortAction->setEnabled(true);
  switchPortAction->setText(tr("Switch Port"));
  switchPortAction->setToolTip(tr("Insert a &lt;switchport&gt; entity"));
  switchPortAction->setIcon(QIcon(":/images/icon/switchport-plus"));
  switchPortAction->setShortcut(QKeySequence("0"));

  // preferences action
  preferencesAction = new QAction(this);
  preferencesAction->setEnabled(true);
  preferencesAction->setText(tr("Preferences..."));
  preferencesAction->setIcon(QIcon(":/images/icon/preferences"));
  preferencesAction->setShortcut(QKeySequence("Ctrl+P"));

  // report action
  reportAction = new QAction(this);
  reportAction->setEnabled(true);
  reportAction->setText(tr("Report Bug..."));
  reportAction->setIcon(QIcon(":/images/icon/report"));

  // about action
  aboutAction = new QAction(this);
  aboutAction->setEnabled(true);
  aboutAction->setText(tr("About..."));

  // draw group
  drawActionGroup = new QActionGroup(this);
  drawActionGroup->setExclusive(true);

  drawActionGroup->addAction(pointerAction);
  drawActionGroup->addAction(linkAction);
}

void  QnstMainWindow::createMenus()
{
  // file menu
  fileMenu = menuBar()->addMenu(tr("File"));
  fileMenu->addAction(newAction);
  fileMenu->addAction(openAction);
  fileMenu->addSeparator();
  fileMenu->addAction(saveAction);
  fileMenu->addAction(saveAsAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exportAction);
  fileMenu->addAction(importAction);
  fileMenu->addSeparator();
  fileMenu->addAction(closeAction);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAction);

  // edit menu
  editMenu = menuBar()->addMenu(tr("Edit"));
  editMenu->addAction(undoAction);
  editMenu->addAction(redoAction);
  editMenu->addSeparator();
  editMenu->addAction(cutAction);
  editMenu->addAction(copyAction);
  editMenu->addAction(pasteAction);

  // window menu
  windowMenu = menuBar()->addMenu(tr("Window"));

  windowMenu->addAction(preferencesAction);

  // help menu
  helpMenu = menuBar()->addMenu(tr("Help"));
  helpMenu->addAction(reportAction);
  helpMenu->addSeparator();
  helpMenu->addAction(aboutAction);
}

void QnstMainWindow::createToolbar()
{
#ifdef APPLICATION
  // file toolbar
  fileToolbar = addToolBar(tr("File"));

  fileToolbar->setMovable(false);
  fileToolbar->setFloatable(false);
  fileToolbar->setIconSize(QSize(32,32));

  fileToolbar->addAction(newAction);
  fileToolbar->addAction(openAction);
  fileToolbar->addAction(saveAction);

#ifdef Q_WS_MACX
  fileToolbar->addSeparator();
#endif

  // edit toolbar
  editToolbar = addToolBar(tr("Edit"));

  editToolbar->setMovable(false);
  editToolbar->setFloatable(false);
  editToolbar->setIconSize(QSize(32,32));

  editToolbar->addAction(undoAction);
  editToolbar->addAction(redoAction);
  editToolbar->addSeparator();
  editToolbar->addAction(cutAction);
  editToolbar->addAction(copyAction);
  editToolbar->addAction(pasteAction);

  // draw toolbar
  drawToolbar = addToolBar(tr("Draw"));
  drawToolbar->setMovable(true);
  drawToolbar->setFloatable(true);
  drawToolbar->setIconSize(QSize(32,32));

  drawToolbar->addAction(pointerAction);
  drawToolbar->addAction(linkAction);

  addToolBar(Qt::LeftToolBarArea, drawToolbar);

  // insert toolbar
  insertToolbar = addToolBar(tr("Insert"));
  insertToolbar->setMovable(true);
  insertToolbar->setFloatable(true);
  insertToolbar->setIconSize(QSize(32,32));

  insertToolbar->addAction(mediaAction);
  insertToolbar->addSeparator();
  insertToolbar->addAction(contextAction);
  insertToolbar->addAction(switchAction);
  insertToolbar->addAction(bodyAction);
  insertToolbar->addSeparator();
  insertToolbar->addAction(areaAction);
  insertToolbar->addAction(propertyAction);
  insertToolbar->addAction(portAction);
  insertToolbar->addAction(switchPortAction);

  addToolBar(Qt::LeftToolBarArea, insertToolbar);

#ifdef Q_WS_MACX
  setUnifiedTitleAndToolBarOnMac(true);
#endif
#endif

#ifdef PLUGIN
  // file toolbar
  fileToolbar = addToolBar(tr("File"));

  fileToolbar->setMovable(false);
  fileToolbar->setFloatable(false);
  fileToolbar->setIconSize(QSize(32,32));

  fileToolbar->addAction(newAction);
  fileToolbar->addAction(openAction);
  fileToolbar->addAction(saveAction);

#ifdef Q_WS_MACX
  fileToolbar->addSeparator();
#endif

  // edit toolbar
  editToolbar = addToolBar(tr("Edit"));

  editToolbar->setMovable(false);
  editToolbar->setFloatable(false);
  editToolbar->setIconSize(QSize(32,32));

  editToolbar->addAction(undoAction);
  editToolbar->addAction(redoAction);
  editToolbar->addSeparator();
  editToolbar->addAction(cutAction);
  editToolbar->addAction(copyAction);
  editToolbar->addAction(pasteAction);

  // draw toolbar
  drawToolbar = addToolBar(tr("Draw"));
  drawToolbar->setMovable(true);
  drawToolbar->setFloatable(true);
  drawToolbar->setIconSize(QSize(32,32));

  drawToolbar->addAction(pointerAction);
  drawToolbar->addAction(linkAction);

  addToolBar(Qt::LeftToolBarArea, drawToolbar);

  // insert toolbar
  insertToolbar = addToolBar(tr("Insert"));
  insertToolbar->setMovable(true);
  insertToolbar->setFloatable(true);
  insertToolbar->setIconSize(QSize(32,32));

  insertToolbar->addAction(mediaAction);
  insertToolbar->addSeparator();
  insertToolbar->addAction(contextAction);
  insertToolbar->addAction(switchAction);
  insertToolbar->addAction(bodyAction);
  insertToolbar->addSeparator();
  insertToolbar->addAction(areaAction);
  insertToolbar->addAction(propertyAction);
  insertToolbar->addAction(portAction);
  insertToolbar->addAction(switchPortAction);

  addToolBar(Qt::LeftToolBarArea, insertToolbar);

#ifdef Q_WS_MACX
  setUnifiedTitleAndToolBarOnMac(true);
#endif
#endif
}

void QnstMainWindow::createWidgets()
{
  view = new QnstView(this); setCentralWidget(view);
}

void  QnstMainWindow::createStatusbar()
{
  // TODO
}

void  QnstMainWindow::createConnections()
{
  connect(newAction, SIGNAL(triggered()), SLOT(performNew()));
  connect(openAction, SIGNAL(triggered()), SLOT(performOpen()));
  connect(saveAction, SIGNAL(triggered()), SLOT(performSave()));
  connect(saveAsAction, SIGNAL(triggered()), SLOT(performSaveAs()));
  connect(exportAction, SIGNAL(triggered()), SLOT(performExport()));
  connect(importAction, SIGNAL(triggered()), SLOT(performImport()));
  connect(closeAction, SIGNAL(triggered()), SLOT(performClose()));
  connect(quitAction, SIGNAL(triggered()), SLOT(performQuit()));

  connect(undoAction, SIGNAL(triggered()), SLOT(performUndo()));
  connect(redoAction, SIGNAL(triggered()), SLOT(performRedo()));
  connect(cutAction, SIGNAL(triggered()), SLOT(performCut()));
  connect(copyAction, SIGNAL(triggered()), SLOT(performCopy()));
  connect(pasteAction, SIGNAL(triggered()), SLOT(performPaste()));

  connect(preferencesAction, SIGNAL(triggered()), SLOT(performPreferences()));

  connect(reportAction, SIGNAL(triggered()), SLOT(performReport()));
  connect(aboutAction, SIGNAL(triggered()), SLOT(performAbout()));

  connect(pointerAction, SIGNAL(triggered()), SLOT(performPointer()));
  connect(linkAction, SIGNAL(triggered()), SLOT(performLink()));

  connect(mediaAction, SIGNAL(triggered()), SLOT(performMedia()));

  connect(bodyAction, SIGNAL(triggered()), SLOT(performBody()));
  connect(contextAction, SIGNAL(triggered()), SLOT(performContext()));
  connect(switchAction, SIGNAL(triggered()), SLOT(performSwitch()));

}

void QnstMainWindow::performNew()
{
  qDebug() << "[QNST]" << ":" << "Performing 'New' action";

  // TODO
}

void QnstMainWindow::performOpen()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Open' action";

  // TODO
}

void QnstMainWindow::performSave()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Save' action";

  // TODO
}

void QnstMainWindow::performSaveAs()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Save As' action";

  // TODO
}


void QnstMainWindow::performExport()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Export' action";

  // TODO
}

void QnstMainWindow::performImport()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Import' action";

  // TODO
}

void QnstMainWindow::performClose()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Close' action";

  // TODO
}

void QnstMainWindow::performQuit()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Quit' action";

  // TODO
}

void QnstMainWindow::performUndo()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Undo' action";

  view->performUndo();
}

void QnstMainWindow::performRedo()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Redo' action";

  view->performRedo();
}

void QnstMainWindow::performCut()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Cut' action";

  view->performCut();
}

void QnstMainWindow::performCopy()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Copy' action";

  view->performCopy();
}

void QnstMainWindow::performPaste()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Paste' action";

  view->performPaste();
}

void QnstMainWindow::performPointer()
{
  view->setEnableLink(false);
}

void QnstMainWindow::performLink()
{
  view->setEnableLink(true);
}

void QnstMainWindow::performBody()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Body' action";

  view->create(Qnst::Body);
}

void QnstMainWindow::performContext()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Context' action";

  view->create(Qnst::Context);
}

void QnstMainWindow::performSwitch()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Switch' action";

  view->create(Qnst::Switch);
}

void QnstMainWindow::performMedia()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Media' action";

  view->create(Qnst::Media);
}

void QnstMainWindow::performPreferences()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Preferences' action";

  // TODO
}

void QnstMainWindow::performReport()
{
  qDebug() << "[QNST]" << ":" << "Performing 'Report' action";

  // TODO
}

void QnstMainWindow::performAbout()
{
  qDebug() << "[QNST]" << ":" << "Performing 'About' action";

  // TODO
}
