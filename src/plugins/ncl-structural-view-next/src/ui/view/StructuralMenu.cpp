#include "StructuralMenu.h"

StructuralMenu::StructuralMenu(QWidget* parent)
  : QMenu(parent)
{
  createActions();
  createMenus();
  createConnections();

  setInsertTop(0);
  setInsertLeft(0);
}

StructuralMenu::~StructuralMenu()
{

}

qreal StructuralMenu::getInsertTop() const
{
  return _insertTop;
}

void StructuralMenu::setInsertTop(qreal insertTop)
{
  _insertTop = insertTop;
}

qreal StructuralMenu::getInsertLeft() const
{
  return _insertLeft;
}

void StructuralMenu::setInsertLeft(qreal insertLeft)
{
  _insertLeft = insertLeft;
}

void StructuralMenu::adjust(StructuralType type)
{
  switch (type) {
    case Structural::Media:
    {
      switchDelete(true);
      switchMedia(false);
      switchContext(false);
      switchSwitch(false);
      switchArea(true);
      switchProperty(true);
      switchPort(false);
      switchSwitchPort(false);

      break;
    }

    case Structural::Body:
    case Structural::Context:
    {
      switchDelete(true);
      switchMedia(true);
      switchContext(true);
      switchSwitch(true);
      switchArea(false);
      switchProperty(true);
      switchPort(true);
      switchSwitchPort(false);

      break;
    }

    case Structural::Switch:
    {
      switchDelete(true);
      switchMedia(true);
      switchContext(true);
      switchSwitch(true);
      switchArea(false);
      switchProperty(true);
      switchPort(false);
      switchSwitchPort(true);

      break;
    }

    case Structural::Area:
    case Structural::Property:
    case Structural::Port:
    case Structural::SwitchPort:
    {
      switchDelete(true);
      switchMedia(false);
      switchContext(false);
      switchSwitch(false);
      switchArea(false);
      switchProperty(false);
      switchPort(false);
      switchSwitchPort(false);

      break;
    }

    default:
    {
      switchDelete(false);
      switchMedia(false);
      switchContext(false);
      switchSwitch(false);
      switchArea(false);
      switchProperty(false);
      switchPort(false);
      switchSwitchPort(false);

      break;
    }
  }
}

void StructuralMenu::createActions()
{
  // help action
  _helpAction = new QAction(this);
  _helpAction->setEnabled(false);
  _helpAction->setText(tr("Help"));
  _helpAction->setShortcut(QKeySequence("F1"));
  _helpAction->setIcon(QIcon(":/images/icon/help"));

  // undo action
  _undoAction = new QAction(this);
  _undoAction->setEnabled(false);
  _undoAction->setText(tr("Undo"));
  _undoAction->setShortcut(QKeySequence("Ctrl+Z"));
  _undoAction->setIcon(QIcon(":/images/icon/undo"));

  // redo action
  _redoAction = new QAction(this);
  _redoAction->setEnabled(false);
  _redoAction->setText(tr("Redo"));
  _redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));
  _redoAction->setIcon(QIcon(":/images/icon/redo"));

  // cut action
  _cutAction = new QAction(this);
  _cutAction->setEnabled(false);
  _cutAction->setText(tr("Cut"));
  _cutAction->setShortcut(QKeySequence("Ctrl+X"));
  _cutAction->setIcon(QIcon(":/images/icon/cut"));

  // copy action
  _copyAction = new QAction(this);
  _copyAction->setEnabled(false);
  _copyAction->setText(tr("Copy"));
  _copyAction->setShortcut(QKeySequence("Ctrl+C"));
  _copyAction->setIcon(QIcon(":/images/icon/copy"));

  // paste action
  _pasteAction = new QAction(this);
  _pasteAction->setEnabled(false);
  _pasteAction->setText(tr("Paste"));
  _pasteAction->setShortcut(QKeySequence("Ctrl+V"));
  _pasteAction->setIcon(QIcon(":/images/icon/paste"));

  // delete action
  _deleteAction = new QAction(this);
  _deleteAction->setEnabled(false);
  _deleteAction->setText(tr("Delete"));
  _deleteAction->setShortcut(QKeySequence("Del"));
  _deleteAction->setIcon(QIcon(":/images/icon/delete"));

  // snapshot action
  _snapshotAction = new QAction(this);
  _snapshotAction->setEnabled(true);
  _snapshotAction->setText(tr("Snapshot..."));
  _snapshotAction->setIcon(QIcon(":/images/icon/snapshot"));

  // media action
  _mediaAction = new QAction(this);
  _mediaAction->setEnabled(false);
  _mediaAction->setText(tr("Media"));
  _mediaAction->setIcon(QIcon(":/images/icon/media-insert"));

  // context action
  _contextAction = new QAction(this);
  _contextAction->setEnabled(false);
  _contextAction->setText(tr("Context"));
  _contextAction->setIcon(QIcon(":/images/icon/context-insert"));

  // switch action
  _switchAction = new QAction(this);
  _switchAction->setEnabled(false);
  _switchAction->setText(tr("Switch"));
  _switchAction->setIcon(QIcon(":/images/icon/switch-insert"));

  // body action
  _bodyAction = new QAction(this);
  _bodyAction->setEnabled(true);
  _bodyAction->setText(tr("Body"));
  _bodyAction->setIcon(QIcon(":/images/icon/body-insert"));

  // port action
  _portAction = new QAction(this);
  _portAction->setEnabled(false);
  _portAction->setText(tr("Port"));
  _portAction->setIcon(QIcon(":/images/icon/port-insert"));

  // switchport action
  _switchPortAction = new QAction(this);
  _switchPortAction->setEnabled(false);
  _switchPortAction->setText(tr("Switch Port"));
  _switchPortAction->setIcon(QIcon(":/images/icon/switchport-insert"));

  // area action
  _areaAction = new QAction(this);
  _areaAction->setEnabled(false);
  _areaAction->setText(tr("Area"));
  _areaAction->setIcon(QIcon(":/images/icon/area-insert"));

  // property action
  _propertyAction = new QAction(this);
  _propertyAction->setEnabled(false);
  _propertyAction->setText(tr("Property"));
  _propertyAction->setIcon(QIcon(":/images/icon/property-insert"));

#ifdef WITH_GRAPHVIZ
  // autoadjust action
  _autoadjustAction = new QAction(this);
  _autoadjustAction->setEnabled(true);
  _autoadjustAction->setText(tr("Auto adjust"));
#endif

  // properties action
  _propertiesAction = new QAction(this);
  _propertiesAction->setEnabled(false);
  _propertiesAction->setText(tr("Properties"));
  _propertiesAction->setIcon(QIcon(":/images/icon/properties"));
}

void StructuralMenu::createMenus()
{
  // insert menu
  _insertMenu = new QMenu();
  _insertMenu->setTitle(tr("Insert"));
  _insertMenu->setIcon(QIcon(":/images/icon/insert"));

  _insertMenu->addAction(_mediaAction);
  _insertMenu->addSeparator();
  _insertMenu->addAction(_contextAction);
  _insertMenu->addAction(_switchAction);
  _insertMenu->addAction(_bodyAction);
  _insertMenu->addSeparator();
  _insertMenu->addAction(_areaAction);
  _insertMenu->addAction(_propertyAction);
  _insertMenu->addAction(_portAction);
  _insertMenu->addAction(_switchPortAction);

  addAction(_helpAction);
  addSeparator();
  addMenu(_insertMenu);
  addSeparator();
  addAction(_undoAction);
  addAction(_redoAction);
  addSeparator();
  addAction(_cutAction);
  addAction(_copyAction);
  addAction(_pasteAction);
  addSeparator();
  addAction(_deleteAction);
  addSeparator();
  addAction(_snapshotAction);
  addSeparator();
#ifdef WITH_GRAPHVIZ
  addAction(_autoadjustAction);
  addSeparator();
#endif
  addAction(_propertiesAction);

}

void StructuralMenu::createConnections()
{
  connect(_mediaAction,SIGNAL(triggered()),SLOT(performMedia()));
  connect(_bodyAction,SIGNAL(triggered()),SLOT(performBody()));
  connect(_contextAction,SIGNAL(triggered()),SLOT(performContext()));
  connect(_switchAction,SIGNAL(triggered()),SLOT(performSwitch()));
  connect(_portAction,SIGNAL(triggered()),SLOT(performPort()));
  connect(_areaAction,SIGNAL(triggered()),SLOT(performArea()));
  connect(_switchPortAction,SIGNAL(triggered()),SLOT(performSwitchPort()));
  connect(_propertyAction,SIGNAL(triggered()),SLOT(performProperty()));
}

void StructuralMenu::switchHelp(bool state)
{
  _helpAction->setEnabled(state);
}

void StructuralMenu::switchUndo(bool state)
{
  _undoAction->setEnabled(state);
}

void StructuralMenu::switchRedo(bool state)
{
  _redoAction->setEnabled(state);
}

void StructuralMenu::switchCut(bool state)
{
  _cutAction->setEnabled(state);
}

void StructuralMenu::switchCopy(bool state)
{
  _copyAction->setEnabled(state);
}

void StructuralMenu::switchPaste(bool state)
{
  _pasteAction->setEnabled(state);
}

void StructuralMenu::switchDelete(bool state)
{
  _deleteAction->setEnabled(state);
}

void StructuralMenu::switchSnapshot(bool state)
{
  _snapshotAction->setEnabled(state);
}

void StructuralMenu::switchMedia(bool state)
{
  _mediaAction->setEnabled(state);
}

void StructuralMenu::switchContext(bool state)
{
  _contextAction->setEnabled(state);
}

void StructuralMenu::switchSwitch(bool state)
{
  _switchAction->setEnabled(state);
}

void StructuralMenu::switchBody(bool state)
{
  _bodyAction->setEnabled(state);
}

void StructuralMenu::switchArea(bool state)
{
  _areaAction->setEnabled(state);
}

void StructuralMenu::switchProperty(bool state)
{
  _propertyAction->setEnabled(state);
}

void StructuralMenu::switchPort(bool state)
{
  _portAction->setEnabled(state);
}

void StructuralMenu::switchSwitchPort(bool state)
{
  _switchPortAction->setEnabled(state);
}

#ifdef WITH_GRAPHVIZ
void StructuralMenu::switchAutoAdjust(bool state)
{
  _autoadjustAction->setEnabled(state);
}
#endif

void StructuralMenu::switchProperties(bool state)
{
  _propertiesAction->setEnabled(state);
}

void StructuralMenu::performMedia()
{
  qreal top = _insertTop - STR_DEFAULT_CONTENT_H/2;
  qreal left = _insertLeft - STR_DEFAULT_CONTENT_W/2;

  emit performedInsert(Structural::Media, StructuralUtil::createProperties(top, left));
}

void StructuralMenu::performContext()
{
  qreal top = _insertTop - STR_DEFAULT_COMPOSITION_H/2;
  qreal left = _insertLeft - STR_DEFAULT_COMPOSITION_W/2;

  emit performedInsert(Structural::Context, StructuralUtil::createProperties(top, left));
}

void StructuralMenu::performSwitch()
{
  qreal top = _insertTop - STR_DEFAULT_COMPOSITION_H/2;
  qreal left = _insertLeft - STR_DEFAULT_COMPOSITION_W/2;

  emit performedInsert(Structural::Switch, StructuralUtil::createProperties(top, left));
}

void StructuralMenu::performBody()
{
  qreal top = _insertTop - STR_DEFAULT_BODY_H/2;
  qreal left = _insertLeft - STR_DEFAULT_BODY_W/2;

  emit performedInsert(Structural::Body, StructuralUtil::createProperties(top, left));
}

void StructuralMenu::performArea()
{
  qreal top = _insertTop - STR_DEFAULT_INTERFACE_H/2;
  qreal left = _insertLeft - STR_DEFAULT_INTERFACE_W/2;

  emit performedInsert(Structural::Area, StructuralUtil::createProperties(top, left));
}

void StructuralMenu::performProperty()
{
  qreal top = _insertTop - STR_DEFAULT_INTERFACE_H/2;
  qreal left = _insertLeft - STR_DEFAULT_INTERFACE_W/2;

  emit performedInsert(Structural::Property, StructuralUtil::createProperties(top, left));
}

void StructuralMenu::performPort()
{
  qreal top = _insertTop - STR_DEFAULT_INTERFACE_H/2;
  qreal left = _insertLeft - STR_DEFAULT_INTERFACE_W/2;

  emit performedInsert(Structural::Port, StructuralUtil::createProperties(top, left));
}

void StructuralMenu::performSwitchPort()
{
  qreal top = _insertTop - STR_DEFAULT_INTERFACE_H/2;
  qreal left = _insertLeft - STR_DEFAULT_INTERFACE_W/2;

  emit performedInsert(Structural::SwitchPort, StructuralUtil::createProperties(top, left));
}
