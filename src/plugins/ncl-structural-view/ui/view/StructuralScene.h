#ifndef STRUCTURALSCENE_H
#define STRUCTURALSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

#include "StructuralMenu.h"

class StructuralScene : public QGraphicsScene
{
public:
  StructuralScene (StructuralMenu *menu, QObject *parent = 0);
  virtual ~StructuralScene ();

protected:
  virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent *event);

private:
  StructuralMenu *_menu;
};

#endif // STRUCTURALSCENE_H