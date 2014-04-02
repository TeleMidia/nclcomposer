#include "StructuralScene.h"

StructuralScene::StructuralScene(QObject* parent)
  : QGraphicsScene(parent)
{
  createMenus();
  createConnections();
}

StructuralScene::~StructuralScene()
{
  delete _menu;
}

void StructuralScene::createMenus()
{
  _menu = new StructuralMenu();
  _menu->undoAction->setEnabled(true);
  _menu->redoAction->setEnabled(true);

  _menu->snapshotAction->setEnabled(true);

  _menu->insertMenu->setEnabled(true);
  _menu->bodyAction->setEnabled(true);
}

void StructuralScene::createConnections()
{
  connect(_menu->bodyAction, SIGNAL(triggered()), SLOT(performBody()));
}

void StructuralScene::performUndo()
{
  StructuralView* view = (StructuralView*) views().at(0);
  view->performUndo();
}

void StructuralScene::performRedo()
{
  StructuralView* view = (StructuralView*) views().at(0);
  view->performRedo();
}

void StructuralScene::performBody()
{
  QString uid = QUuid::createUuid().toString();
  QString parent = "";
  QMap<QString, QString> properties;
  properties[":nst:subtype"] = "body";
  properties[":nst:top"] = QString::number(height()/2 - DEFAULT_BODY_HEIGHT/2);
  properties[":nst:left"] = QString::number(width()/2 - DEFAULT_BODY_WIDTH/2);
  properties[":nst:width"] = QString::number(DEFAULT_BODY_WIDTH);
  properties[":nst:height"] = QString::number(DEFAULT_BODY_HEIGHT);

  QMap<QString, QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = QUuid::createUuid().toString();

  StructuralView* view = (StructuralView*) views().at(0);
  view->insert(uid, parent, properties, settings);
}

void StructuralScene::performSnapshot()
{
  StructuralView* view = (StructuralView*) views().at(0);
  view->performSnapshot();
}

void StructuralScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  QGraphicsScene::contextMenuEvent(event);

  if (!event->isAccepted())
  {
    _menu->exec(event->screenPos());

    event->accept();
  }
}
