#include "StructuralScene.h"

StructuralScene::StructuralScene (QObject *parent) : QGraphicsScene (parent)
{
  setSceneRect (0, 0, STR_DEFAULT_SCENE_W, STR_DEFAULT_SCENE_H);
}

StructuralScene::~StructuralScene () {}

StructuralMenu *
StructuralScene::getMenu () const
{
  return _menu;
}

void
StructuralScene::setMenu (StructuralMenu *menu)
{
  _menu = menu;
}

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
