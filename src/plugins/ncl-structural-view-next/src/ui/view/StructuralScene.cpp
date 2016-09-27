#include "StructuralScene.h"

StructuralScene::StructuralScene(QObject* parent)
  : QGraphicsScene(parent)
{

}

StructuralScene::~StructuralScene()
{

}

StructuralMenu* StructuralScene::getMenu() const
{
  return _menu;
}

void StructuralScene::setMenu(StructuralMenu* menu)
{
  _menu = menu;
}

void StructuralScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  QGraphicsScene::contextMenuEvent(event);

  if (!event->isAccepted()) {
    _menu->setInsertTop(event->scenePos().y());
    _menu->setInsertLeft(event->scenePos().x());
    _menu->adjust();

    _menu->exec(event->screenPos());

    event->accept();
  }
}
