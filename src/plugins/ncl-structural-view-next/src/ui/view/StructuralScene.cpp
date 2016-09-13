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
//  _menu->undoAction->setEnabled(false);
//  _menu->redoAction->setEnabled(false);

//  _menu->snapshotAction->setEnabled(true);

//  _menu->insertMenu->setEnabled(true);
//  _menu->bodyAction->setEnabled(true);
  _menu->updateInsertAction(Structural::NoType);
}

void StructuralScene::createConnections()
{
//  connect(_menu, SIGNAL(insert(Structural::EntitySubtype)), SLOT(performInsert(Structural::EntitySubtype)));
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

void StructuralScene::performInsert(Structural::StructuralType name)
{
  switch (name) {
    case Structural::Body:
    {
      QMap<QString, QString> properties;
      properties[STR_PROPERTY_ENTITY_TOP] = QString::number(_insertPoint.y() - STR_DEFAULT_BODY_H/2);
      properties[STR_PROPERTY_ENTITY_LEFT] = QString::number(_insertPoint.x() - STR_DEFAULT_BODY_W/2);

      QMap<QString, QString> settings;

      StructuralView* view = (StructuralView*) views().at(0);
      view->create(Structural::Body, properties, settings);

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

    _menu->exec(event->screenPos());

    event->accept();
  }
}
