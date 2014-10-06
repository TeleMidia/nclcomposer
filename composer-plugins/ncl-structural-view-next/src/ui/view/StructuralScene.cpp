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
  connect(_menu, SIGNAL(insert(Structural::EntityName)), SLOT(performInsert(Structural::EntityName)));
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

void StructuralScene::performInsert(Structural::EntityName name)
{
  switch (name) {
    case Structural::Body:
    {
      QMap<QString, QString> properties;
      properties["LOCAL:TOP"] = QString::number(_insertPoint.y() - DEFAULT_BODY_HEIGHT/2);
      properties["LOCAL:LEFT"] = QString::number(_insertPoint.x() - DEFAULT_BODY_WIDTH/2);

      StructuralView* view = (StructuralView*) views().at(0);
//      view->create(Structural::Body, properties, QMap<QString, QString>());

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
