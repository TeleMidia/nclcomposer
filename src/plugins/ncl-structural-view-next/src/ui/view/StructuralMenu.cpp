#include "StructuralMenu.h"

StructuralMenu::StructuralMenu(QWidget* parent)
  : QMenu(parent)
{
  createActions();
  createMenus();
  createConnections();
}

StructuralMenu::~StructuralMenu()
{

}

void StructuralMenu::updateInsertAction(StructuralType type, bool enableBody)
{
  switch (type) {
    case Structural::Media:
      mediaAction->setEnabled(false);
      contextAction->setEnabled(false);
      switchAction->setEnabled(false);
      bodyAction->setEnabled(false);
      areaAction->setEnabled(true);
      propertyAction->setEnabled(true);
      portAction->setEnabled(false);
      switchPortAction->setEnabled(false);
      break;

    case Structural::Context:
      mediaAction->setEnabled(true);
      contextAction->setEnabled(true);
      switchAction->setEnabled(true);
      bodyAction->setEnabled(false);
      areaAction->setEnabled(false);
      propertyAction->setEnabled(true);
      portAction->setEnabled(true);
      switchPortAction->setEnabled(false);
      break;

    case Structural::Switch:
      mediaAction->setEnabled(true);
      contextAction->setEnabled(true);
      switchAction->setEnabled(true);
      bodyAction->setEnabled(false);
      areaAction->setEnabled(false);
      propertyAction->setEnabled(true);
      portAction->setEnabled(false);
      switchPortAction->setEnabled(true);
      break;

    case Structural::Body:
      mediaAction->setEnabled(true);
      contextAction->setEnabled(true);
      switchAction->setEnabled(true);
      bodyAction->setEnabled(false);
      areaAction->setEnabled(false);
      propertyAction->setEnabled(true);
      portAction->setEnabled(true);
      switchPortAction->setEnabled(false);
      break;

    case Structural::Area:
    case Structural::Property:
    case Structural::Port:
    case Structural::SwitchPort:
      mediaAction->setEnabled(false);
      contextAction->setEnabled(false);
      switchAction->setEnabled(false);
      bodyAction->setEnabled(false);
      areaAction->setEnabled(false);
      propertyAction->setEnabled(false);
      portAction->setEnabled(false);
      switchPortAction->setEnabled(false);
      break;

    default:
      if (DEFAULT_BODY_ENABLE) {
        mediaAction->setEnabled(false);
        contextAction->setEnabled(false);
        switchAction->setEnabled(false);
        bodyAction->setEnabled(enableBody);
        areaAction->setEnabled(false);
        propertyAction->setEnabled(false);
        portAction->setEnabled(false);
        switchPortAction->setEnabled(false);
      }else{
        mediaAction->setEnabled(true);
        contextAction->setEnabled(true);
        switchAction->setEnabled(true);
        areaAction->setEnabled(false);
        propertyAction->setEnabled(false);
        portAction->setEnabled(false);
        switchPortAction->setEnabled(false);
      }
      break;
  }
}

void StructuralMenu::createActions()
{
  // help action
  helpAction = new QAction(this);
  helpAction->setEnabled(false);
  helpAction->setText(tr("Help"));
  helpAction->setShortcut(QKeySequence("F1"));
  helpAction->setIcon(QIcon(":/images/icon/help"));

  // undo action
  undoAction = new QAction(this);
  undoAction->setEnabled(false);
  undoAction->setText(tr("Undo"));
  undoAction->setShortcut(QKeySequence("Ctrl+Z"));
  undoAction->setIcon(QIcon(":/images/icon/undo"));

  // redo action
  redoAction = new QAction(this);
  redoAction->setEnabled(false);
  redoAction->setText(tr("Redo"));
  redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));
  redoAction->setIcon(QIcon(":/images/icon/redo"));

  // cut action
  cutAction = new QAction(this);
  cutAction->setEnabled(false);
  cutAction->setText(tr("Cut"));
  cutAction->setShortcut(QKeySequence("Ctrl+X"));
  cutAction->setIcon(QIcon(":/images/icon/cut"));

  // copy action
  copyAction = new QAction(this);
  copyAction->setEnabled(false);
  copyAction->setText(tr("Copy"));
  copyAction->setShortcut(QKeySequence("Ctrl+C"));
  copyAction->setIcon(QIcon(":/images/icon/copy"));

  // paste action
  pasteAction = new QAction(this);
  pasteAction->setEnabled(false);
  pasteAction->setText(tr("Paste"));
  pasteAction->setShortcut(QKeySequence("Ctrl+V"));
  pasteAction->setIcon(QIcon(":/images/icon/paste"));

  // delete action
  deleteAction = new QAction(this);
  deleteAction->setEnabled(false);
  deleteAction->setText(tr("Delete"));
  deleteAction->setShortcut(QKeySequence("Del"));
  deleteAction->setIcon(QIcon(":/images/icon/delete"));

  // snapshot action
  snapshotAction = new QAction(this);
  snapshotAction->setEnabled(false);
  snapshotAction->setText(tr("Snapshot..."));
  snapshotAction->setIcon(QIcon(":/images/icon/snapshot"));

  // media action
  mediaAction = new QAction(this);
  mediaAction->setEnabled(false);
  mediaAction->setText(tr("Media"));
  mediaAction->setIcon(QIcon(":/images/icon/media-insert"));

  // context action
  contextAction = new QAction(this);
  contextAction->setEnabled(false);
  contextAction->setText(tr("Context"));
  contextAction->setIcon(QIcon(":/images/icon/context-insert"));

  // switch action
  switchAction = new QAction(this);
  switchAction->setEnabled(false);
  switchAction->setText(tr("Switch"));
  switchAction->setIcon(QIcon(":/images/icon/switch-insert"));

  // body action
  bodyAction = new QAction(this);
  bodyAction->setEnabled(false);
  bodyAction->setText(tr("Body"));
  bodyAction->setIcon(QIcon(":/images/icon/body-insert"));

  // port action
  portAction = new QAction(this);
  portAction->setEnabled(false);
  portAction->setText(tr("Port"));
  portAction->setIcon(QIcon(":/images/icon/port-insert"));

  // switchport action
  switchPortAction = new QAction(this);
  switchPortAction->setEnabled(false);
  switchPortAction->setText(tr("Switch Port"));
  switchPortAction->setIcon(QIcon(":/images/icon/switchport-insert"));

  // area action
  areaAction = new QAction(this);
  areaAction->setEnabled(false);
  areaAction->setText(tr("Area"));
  areaAction->setIcon(QIcon(":/images/icon/area-insert"));

  // property action
  propertyAction = new QAction(this);
  propertyAction->setEnabled(false);
  propertyAction->setText(tr("Property"));
  propertyAction->setIcon(QIcon(":/images/icon/property-insert"));

  // properties action
  propertiesAction = new QAction(this);
  propertiesAction->setEnabled(false);
  propertiesAction->setText(tr("Properties"));
  propertiesAction->setIcon(QIcon(":/images/icon/properties"));
}

void StructuralMenu::createMenus()
{
  // insert menu
  insertMenu = new QMenu();
  insertMenu->setTitle(tr("Insert"));
  insertMenu->setIcon(QIcon(":/images/icon/insert"));

  insertMenu->addAction(mediaAction);
  insertMenu->addSeparator();
  insertMenu->addAction(contextAction);
  insertMenu->addAction(switchAction);
  if (DEFAULT_BODY_ENABLE)
    insertMenu->addAction(bodyAction);
  insertMenu->addSeparator();
  insertMenu->addAction(areaAction);
  insertMenu->addAction(propertyAction);
  insertMenu->addAction(portAction);
  insertMenu->addAction(switchPortAction);

  addAction(helpAction);
  addSeparator();
  addMenu(insertMenu);
  addSeparator();
  addAction(undoAction);
  addAction(redoAction);
  addSeparator();
  addAction(cutAction);
  addAction(copyAction);
  addAction(pasteAction);
  addSeparator();
  addAction(deleteAction);
  addSeparator();
  addAction(snapshotAction);
  addSeparator();
  addAction(propertiesAction  );

}

void StructuralMenu::changeUndoState(bool enable)
{
  undoAction->setEnabled(enable);
}

void StructuralMenu::changeRedoState(bool enable)
{
  redoAction->setEnabled(enable);
}

void StructuralMenu::changeCutState(bool enable)
{
  cutAction->setEnabled(enable);
}

void StructuralMenu::changeCopyState(bool enable)
{
  copyAction->setEnabled(enable);
}

void StructuralMenu::changePasteState(bool enable)
{
  pasteAction->setEnabled(enable);
}

void StructuralMenu::createConnections()
{
  connect(mediaAction,SIGNAL(triggered()),SLOT(performMedia()));
  connect(bodyAction,SIGNAL(triggered()),SLOT(performBody()));
  connect(contextAction,SIGNAL(triggered()),SLOT(performContext()));
  connect(switchAction,SIGNAL(triggered()),SLOT(performSwitch()));
  connect(portAction,SIGNAL(triggered()),SLOT(performPort()));
  connect(areaAction,SIGNAL(triggered()),SLOT(performArea()));
  connect(switchPortAction,SIGNAL(triggered()),SLOT(performSwitchPort()));
  connect(propertyAction,SIGNAL(triggered()),SLOT(performProperty()));
}

void StructuralMenu::performBody()
{
  emit insert(Structural::Body);
}

void StructuralMenu::performContext()
{
  emit insert(Structural::Context);
}

void StructuralMenu::performSwitch()
{
  emit insert(Structural::Switch);
}

void StructuralMenu::performMedia()
{
  emit insert(Structural::Media);
}

void StructuralMenu::performPort()
{
  emit insert(Structural::Port);
}

void StructuralMenu::performArea()
{
  emit insert(Structural::Area);
}

void StructuralMenu::performSwitchPort()
{
  emit insert(Structural::SwitchPort);
}

void StructuralMenu::performProperty()
{
  emit insert(Structural::Property);
}
