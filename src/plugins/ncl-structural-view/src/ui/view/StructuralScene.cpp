#include "StructuralScene.h"

StructuralScene::StructuralScene (StructuralMenu *menu, QObject *parent) :
  QGraphicsScene (parent),
  _menu (menu)
{
  setSceneRect (0, 0, STR_DEFAULT_SCENE_W, STR_DEFAULT_SCENE_H);
}

StructuralScene::~StructuralScene () {}

void
StructuralScene::contextMenuEvent (QGraphicsSceneContextMenuEvent *event)
{
  QGraphicsScene::contextMenuEvent (event);

  if (!event->isAccepted ())
  {
    _menu->setInsertTop (event->scenePos ().y ());
    _menu->setInsertLeft (event->scenePos ().x ());
    _menu->adjust ();

    _menu->exec (event->screenPos ());

    event->accept ();
  }
}
