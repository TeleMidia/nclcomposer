#include "StructuralScene.h"

StructuralScene::StructuralScene(QObject* parent)
    : QGraphicsScene(parent)
{
  createObjects();
  createConnections();
}

StructuralScene::~StructuralScene()
{

}

QVector<StructuralGraphicsEntity*> StructuralScene::getRoots() const
{
  return roots;
}

void StructuralScene::addRoot(StructuralGraphicsEntity* entity)
{
  if (entity != NULL)
  {
    roots.append(entity);

    // connect(entity, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
    // connect(entity, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

    connect(entity, SIGNAL(cutRequested()), SIGNAL(cutRequested()));
    connect(entity, SIGNAL(copyRequested()), SIGNAL(copyRequested()));
    connect(entity, SIGNAL(pasteRequested()), SIGNAL(pasteRequested()));

    connect(entity, SIGNAL(deleteRequested()), SIGNAL(deleteRequested()));

    connect(entity, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

    connect(entity, SIGNAL(zoominRequested()), SIGNAL(zoominRequested()));
    connect(entity, SIGNAL(zoomoutRequested()), SIGNAL(zoomoutRequested()));
    connect(entity, SIGNAL(zoomresetRequested()), SIGNAL(zoomresetRequested()));
    connect(entity, SIGNAL(fullscreenRequested()), SIGNAL(fullscreenRequested()));

    connect(entity, SIGNAL(entityAdded(StructuralGraphicsEntity*)), SIGNAL(entityAdded(StructuralGraphicsEntity*)));
    connect(entity, SIGNAL(entityChanged(StructuralGraphicsEntity*)), SIGNAL(entityChanged(StructuralGraphicsEntity*)));
    connect(entity, SIGNAL(entityRemoved(StructuralGraphicsEntity*)), SIGNAL(entityRemoved(StructuralGraphicsEntity*)));
    connect(entity, SIGNAL(entityAboutToChange(StructuralGraphicsEntity*,QMap<QString,QString>)), SIGNAL(entityAboutToChange(StructuralGraphicsEntity*,QMap<QString,QString>)));
    connect(entity, SIGNAL(entitySelected(StructuralGraphicsEntity*)), SIGNAL(entitySelected(StructuralGraphicsEntity*)));

    addItem(entity);

    menu->menuInsert->setEnabled(false);
    menu->actionAddBody->setEnabled(false);
  }
}

void StructuralScene::removeRoot(StructuralGraphicsEntity* entity)
{
  if (entity != NULL)
  {
    int index = roots.indexOf(entity);

    if (index >= 0)
    {
      roots.remove(index);

      removeItem(entity);

      menu->menuInsert->setEnabled(true);
      menu->actionAddBody->setEnabled(true);
    }
  }
}

void StructuralScene::createObjects()
{
  menu = new StructuralMenu();
  menu->actionExport->setEnabled(true);

  menu->menuInsert->setEnabled(true);
  menu->actionAddBody->setEnabled(true);
}

void StructuralScene::createConnections()
{
  connect(menu, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

  // \fixme Even working this is not the correct way to do that
  connect(menu, SIGNAL(menuAddEntityTriggered(Structural::EntityType)),
                SLOT(performBody()));
}

void StructuralScene::performBody()
{
  StructuralGraphicsBody* entity = new StructuralGraphicsBody();
  entity->setTop(height()/2 - DEFAULT_BODY_HEIGHT/2);
  entity->setLeft(width()/2 - DEFAULT_BODY_WIDTH/2);
  entity->setWidth(DEFAULT_BODY_WIDTH);
  entity->setHeight(DEFAULT_BODY_HEIGHT);

  addRoot(entity);

  emit entityAdded(entity);
}

void StructuralScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  QGraphicsScene::contextMenuEvent(event);

  if (!event->isAccepted())
  {
    menu->exec(event->screenPos());

    event->accept();
  }
}
