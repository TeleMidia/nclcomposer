#include "StructuralScene.h"

#include "util/Utilities.h"

StructuralScene::StructuralScene (StructuralMenu *menu, QObject *parent)
    : QGraphicsScene (parent), _menu (menu)
{
  setSceneRect (0, 0, ST_DEFAULT_SCENE_W, ST_DEFAULT_SCENE_H);
}

void
StructuralScene::contextMenuEvent (QGraphicsSceneContextMenuEvent *event)
{
  QGraphicsScene::contextMenuEvent (event);

  if (!event->isAccepted ())
  {
    _menu->exec (event->screenPos ());
    event->accept ();
  }
}

bool
StructuralScene::hasEntity (const QString &uid)
{
  return _entities.contains (uid);
}

StructuralEntity *
StructuralScene::getEntity (const QString &uid)
{
//  CPR_ASSERT (hasEntity (uid));
  return _entities.value (uid, nullptr);
}

QMap <QString, StructuralEntity *> &
StructuralScene::getEntities ()
{
  return _entities;
}

QMap <QString, QString> &
StructuralScene::getRefs ()
{
  return _refs;
}

void
StructuralScene::clear ()
{
  // TODO: remove entities

  _refs.clear ();
  _entities.clear ();

  QGraphicsScene::clear ();
}

StructuralEntity *
StructuralScene::getBody ()
{
  StructuralEntity *body = nullptr;

  for (StructuralEntity *ent : _entities.values ())
  {
    if (ent->getType () == Structural::Body)
    {
      body = ent;
      break;
    }
  }

  return body;
}
