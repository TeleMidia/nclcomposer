#ifndef STRUCTURALSCENE_H
#define STRUCTURALSCENE_H

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsScene>

#include "StructuralMenu.h"

class StructuralScene : public QGraphicsScene
{
public:
  StructuralScene(QObject* parent = 0);
  virtual ~StructuralScene();

  StructuralMenu* getMenu() const;
  void setMenu(StructuralMenu* menu);

protected:
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

private:
  StructuralMenu* _menu;
};

#endif // STRUCTURALSCENE_H
