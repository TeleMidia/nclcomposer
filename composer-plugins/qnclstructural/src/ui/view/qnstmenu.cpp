#include "qnstmenu.h"

QnstMenu::QnstMenu(QWidget* parent)
  : QMenu(parent)
{
  createActions();
  createMenus();
  createConnections();

  addAction(actionHelp);
  addSeparator();
  addAction(actionUndo);
  addAction(actionRedo);
  addSeparator();
  addAction(actionCut);
  addAction(actionCopy);
  addAction(actionPaste);
  addSeparator();
  addAction(actionDelete);
  addSeparator();
  addAction(actionExport);
  addSeparator();
  addMenu(menuView);
  addMenu(menuInsert);
  addMenu(menuShow);
  addMenu(menuArrange);
  addMenu(menuAdjust);
  addSeparator();
  addAction(actionHide);
  addSeparator();
  addAction(actionProperties);
}

QnstMenu::~QnstMenu()
{

}

void QnstMenu::createActions()
{
  // help action
  actionHelp = new QAction(this);
  actionHelp->setText(tr("Help"));
  actionHelp->setIcon(QIcon(":/icon/help"));
  actionHelp->setShortcut(QKeySequence("F1"));
  actionHelp->setEnabled(false);

  // undo action
  actionUndo = new QAction(this);
  actionUndo->setText(tr("Undo"));
  actionUndo->setIcon(QIcon(":/icon/undo"));
  actionUndo->setShortcut(QKeySequence("Ctrl+Z"));
  actionUndo->setEnabled(false);

  // redo action
  actionRedo = new QAction(this);
  actionRedo->setText(tr("Redo"));
  actionRedo->setIcon(QIcon(":/icon/redo"));
  actionRedo->setShortcut(QKeySequence("Ctrl+Shift+Z"));
  actionRedo->setEnabled(false);

  // cut action
  actionCut = new QAction(this);
  actionCut->setText(tr("Cut"));
  actionCut->setIcon(QIcon(":/icon/cut"));
  actionCut->setShortcut(QKeySequence("Ctrl+X"));
  actionCut->setEnabled(false);

  // copy action
  actionCopy = new QAction(this);
  actionCopy->setText(tr("Copy"));
  actionCopy->setIcon(QIcon(":/icon/copy"));
  actionCopy->setShortcut(QKeySequence("Ctrl+C"));
  actionCopy->setEnabled(false);

  // paste action
  actionPaste = new QAction(this);
  actionPaste->setText(tr("Paste"));
  actionPaste->setIcon(QIcon(":/icon/paste"));
  actionPaste->setShortcut(QKeySequence("Ctrl+V"));
  actionPaste->setEnabled(false);

  // delete action
  actionDelete = new QAction(this);
  actionDelete->setText(tr("Delete"));
  actionDelete->setIcon(QIcon(":/icon/delete"));
  actionDelete->setShortcut(QKeySequence("Del"));
  actionDelete->setEnabled(false);

  // zoomin action
  actionZoomIn = new QAction(this);
  actionZoomIn->setText(tr("Zoom In"));
  actionZoomIn->setIcon(QIcon(":/icon/zoomin"));
  actionZoomIn->setShortcut(QKeySequence("Ctrl++"));
  actionZoomIn->setEnabled(false);

  // zoomout action
  actionZoomOut = new QAction(this);
  actionZoomOut->setText(tr("Zoom Out"));
  actionZoomOut->setIcon(QIcon(":/icon/zoomout"));
  actionZoomOut->setShortcut(QKeySequence("Ctrl+-"));
  actionZoomOut->setEnabled(false);

  // reset action
  actionZoomReset = new QAction(this);
  actionZoomReset->setText(tr("Reset"));
  actionZoomReset->setIcon(QIcon(":/icon/zoomreset"));
  actionZoomReset->setShortcut(QKeySequence("Ctrl+0"));
  actionZoomReset->setEnabled(false);

  // fullscreen action
  actionFullscreen = new QAction(this);
  actionFullscreen->setText(tr("Full Screen"));
  actionFullscreen->setIcon(QIcon(":/icon/fullscreen"));
  actionFullscreen->setShortcut(QKeySequence("F11"));
  actionFullscreen->setEnabled(false);

  // export action
  actionExport = new QAction(this);
  actionExport->setText(tr("Export..."));
  actionExport->setIcon(QIcon(":/icon/export"));
  actionExport->setEnabled(false);

  // add media action
  actionAddMedia = new QAction(this);
  actionAddMedia->setText(tr("Media"));
  actionAddMedia->setData(Qnst::Media);
  actionAddMedia->setIcon(QIcon(QnstUtil::iconFromMediaType(Qnst::Media)));
  actionAddMedia->setEnabled(false);

  // add context action
  actionAddContext = new QAction(this);
  actionAddContext->setText(tr("Context"));
  actionAddContext->setData(Qnst::Context);
  actionAddContext->setIcon(QIcon(QnstUtil::iconFromMediaType(Qnst::Context)));
  actionAddContext->setEnabled(false);

  // add switch action
  actionAddSwitch = new QAction(this);
  actionAddSwitch->setText(tr("Switch"));
  actionAddSwitch->setData(Qnst::Switch);
  actionAddSwitch->setIcon(QIcon(QnstUtil::iconFromMediaType(Qnst::Switch)));
  actionAddSwitch->setEnabled(false);

  // add body action
  actionAddBody = new QAction(this);
  actionAddBody->setText(tr("Body"));
  actionAddBody->setData(Qnst::Body);
  actionAddBody->setIcon(QIcon(QnstUtil::iconFromMediaType(Qnst::Body)));
  actionAddBody->setEnabled(false);

  // add port action
  actionAddPort = new QAction(this);
  actionAddPort->setText(tr("Port"));
  actionAddPort->setData(Qnst::Port);
  actionAddPort->setIcon(QIcon(QnstUtil::iconFromMediaType(Qnst::Port)));
  actionAddPort->setEnabled(false);

  // add mapping action
  actionAddSwitchPort = new QAction(this);
  actionAddSwitchPort->setText(tr("Switch Port"));
  actionAddSwitchPort->setData(Qnst::SwitchPort);
  actionAddSwitchPort->setIcon(QIcon(QnstUtil::iconFromMediaType(Qnst::SwitchPort)));
  actionAddSwitchPort->setEnabled(false);

  // area action
  actionAddArea = new QAction(this);
  actionAddArea->setText(tr("Area"));
  actionAddArea->setData(Qnst::Area);
  actionAddArea->setIcon(QIcon(QnstUtil::iconFromMediaType(Qnst::Area)));
  actionAddArea->setEnabled(false);

  // property action
  actionAddProperty = new QAction(this);
  actionAddProperty->setText(tr("Property"));
  actionAddProperty->setData(Qnst::Property);
  actionAddProperty->setIcon(QIcon(QnstUtil::iconFromMediaType(Qnst::Property)));
  actionAddProperty->setEnabled(false);

  // aggregator action
  actionAddAggregator = new QAction(this);
  actionAddAggregator->setText(tr("Aggregator"));
  actionAddAggregator->setData(Qnst::Link);
  actionAddAggregator->setIcon(QIcon(QnstUtil::iconFromMediaType(Qnst::Property)));
  actionAddAggregator->setEnabled(false);

  // bring to front action
  actionBringfront = new QAction(this);
  actionBringfront->setText(tr("Bring to Front"));
  actionBringfront->setIcon(QIcon(":/icon/bringfront"));
  actionBringfront->setShortcut(QKeySequence("Shift+Ctrl+]"));
  actionBringfront->setEnabled(false);

  // bring forward action
  actionBringforward = new QAction(this);
  actionBringforward->setText(tr("Bring Forward"));
  actionBringforward->setIcon(QIcon(":/icon/bringforward"));
  actionBringforward->setShortcut(QKeySequence("Ctrl+]"));
  actionBringforward->setEnabled(false);

  // send backward action
  actionSendbackward = new QAction(this);
  actionSendbackward->setText(tr("Send Backward"));
  actionSendbackward->setIcon(QIcon(":/icon/sendbackward"));
  actionSendbackward->setShortcut(QKeySequence("Ctrl+["));
  actionSendbackward->setEnabled(false);

  // send to back action
  actionSendback = new QAction(this);
  actionSendback->setText(tr("Send to Back"));
  actionSendback->setIcon(QIcon(":/icon/sendback"));
  actionSendback->setShortcut(QKeySequence("Shift+Ctrl+["));
  actionSendback->setEnabled(false);

  // hide action
  actionHide = new QAction(this);
  actionHide->setText(tr("Hide"));
  actionHide->setShortcut(QKeySequence("Ctrl+H"));
  actionHide->setEnabled(false);

  // properties action
  actionProperties = new QAction(this);
  actionProperties->setText(tr("Properties"));
  actionProperties->setEnabled(false);

  // auto action
  actionAuto = new QAction(this);
  actionAuto->setText(tr("Auto (Unstable)"));
  actionAuto->setEnabled(false);
}

void QnstMenu::createMenus()
{
  // view menu
  menuView = new QMenu();
  menuView->setTitle(tr("View"));

  menuView->addAction(actionZoomIn);
  menuView->addAction(actionZoomOut);
  menuView->addAction(actionZoomReset);
  menuView->addSeparator();
  menuView->addAction(actionFullscreen);

  menuView->setEnabled(false);

  // insert menu
  menuInsert = new QMenu();
  menuInsert->setTitle(tr("Insert"));

  menuInsert->addAction(actionAddMedia);
  menuInsert->addSeparator();
  menuInsert->addAction(actionAddContext);
  menuInsert->addAction(actionAddSwitch);
  menuInsert->addSeparator();
  menuInsert->addAction(actionAddBody);
  menuInsert->addSeparator();
  menuInsert->addAction(actionAddPort);
  menuInsert->addAction(actionAddSwitchPort);
  menuInsert->addAction(actionAddArea);
  menuInsert->addAction(actionAddProperty);
  menuInsert->addSeparator();
  menuInsert->addAction(actionAddAggregator);

  menuInsert->setEnabled(false);

  // show menu
  menuShow = new QMenu();
  menuShow->setTitle(tr("Show"));

  menuShow->setEnabled(false);

  // arrange menu
  menuArrange = new QMenu();
  menuArrange->setTitle(tr("Arrange"));

  menuArrange->addAction(actionBringfront);
  menuArrange->addAction(actionBringforward);
  menuArrange->addAction(actionSendbackward);
  menuArrange->addAction(actionSendback);

  menuArrange->setEnabled(false);

  // adjust menu
  menuAdjust = new QMenu();
  menuAdjust->setTitle(tr("Adjust"));

  menuAdjust->addAction(actionAuto);

  menuAdjust->setEnabled(false);
}

void QnstMenu::createConnections()
{
  connect(actionHelp, SIGNAL(triggered()), SIGNAL(helpRequested()));

  connect(actionUndo, SIGNAL(triggered()), SIGNAL(undoRequested()));
  connect(actionRedo, SIGNAL(triggered()), SIGNAL(redoRequested()));

  connect(actionCut, SIGNAL(triggered()), SIGNAL(cutRequested()));
  connect(actionCopy, SIGNAL(triggered()), SIGNAL(copyRequested()));
  connect(actionPaste, SIGNAL(triggered()), SIGNAL(pasteRequested()));

  connect(actionDelete, SIGNAL(triggered()), SIGNAL(deleteRequested()));

  connect(actionExport, SIGNAL(triggered()), SIGNAL(exportRequested()));

  connect(actionZoomIn, SIGNAL(triggered()), SIGNAL(zoominRequested()));
  connect(actionZoomOut, SIGNAL(triggered()), SIGNAL(zoomoutRequested()));
  connect(actionZoomReset, SIGNAL(triggered()), SIGNAL(zoomresetRequested()));
  connect(actionFullscreen, SIGNAL(triggered()), SIGNAL(fullscreenRequested()));

  connect(actionAddMedia, SIGNAL(triggered()), SLOT(dispatchAddAction()));

  // Add actions
  connect(actionAddContext, SIGNAL(triggered()), SLOT(dispatchAddAction()));
  connect(actionAddSwitch, SIGNAL(triggered()), SLOT(dispatchAddAction()));
  connect(actionAddBody, SIGNAL(triggered()), SLOT(dispatchAddAction()));
  connect(actionAddPort, SIGNAL(triggered()), SLOT(dispatchAddAction()));
  connect(actionAddSwitchPort, SIGNAL(triggered()), SLOT(dispatchAddAction()));
  connect(actionAddArea, SIGNAL(triggered()), SLOT(dispatchAddAction()));
  connect(actionAddProperty, SIGNAL(triggered()), SLOT(dispatchAddAction()));
  connect(actionAddAggregator, SIGNAL(triggered()), SLOT(dispatchAddAction()));

  connect(actionBringfront, SIGNAL(triggered()), SIGNAL(bringfrontRequested()));
  connect(actionBringforward, SIGNAL(triggered()), SIGNAL(bringforwardRequested()));
  connect(actionSendbackward, SIGNAL(triggered()), SIGNAL(sendbackwardRequested()));
  connect(actionSendback, SIGNAL(triggered()), SIGNAL(sendbackRequested()));

  connect(actionHide, SIGNAL(triggered()), SIGNAL(hideRequested()));

  connect(actionProperties, SIGNAL(triggered()), SIGNAL(propertiesRequested()));

  connect(actionAuto, SIGNAL(triggered()), SIGNAL(autoRequested()));
}

void QnstMenu::dispatchAddAction()
{
  QAction *action = dynamic_cast <QAction*> (QObject::sender());
  if(action != NULL)
  {
    qWarning() << "[QNST] QnstMenu::dispatchAddAction" << action->data().toInt();
    emit menuAddEntityTriggered( (Qnst::EntityType) action->data().toInt() );
  }
}
