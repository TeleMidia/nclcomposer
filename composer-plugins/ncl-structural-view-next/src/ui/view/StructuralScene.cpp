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
  connect(_menu, SIGNAL(insert(Structural::EntitySubtype)), SLOT(performInsert(Structural::EntitySubtype)));
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

void StructuralScene::performInsert(Structural::EntitySubtype name)
{
  switch (name) {
    case Structural::Body:
    {
      QMap<QString, QString> properties;
      properties[":nst:top"] = QString::number(_insertPoint.y() - DEFAULT_BODY_HEIGHT/2);
      properties[":nst:left"] = QString::number(_insertPoint.x() - DEFAULT_BODY_WIDTH/2);

      StructuralView* view = (StructuralView*) views().at(0);
      view->create(Structural::Body, properties, QMap<QString, QString>());

      break;
    }

    default:
    {
      break;
    }
  }
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
    _insertPoint = event->scenePos();

    qDebug() << _insertPoint;

    _menu->exec(event->screenPos());

    event->accept();
  }
}
