#ifndef QNSTSCENE_H
#define QNSTSCENE_H

#include <QGraphicsSceneContextMenuEvent>

#include <QGraphicsScene>
#include <QUuid>

#include <QDebug>

#include "StructuralMenu.h"
#include "StructuralView.h"

class StructuralView;

class StructuralScene : public QGraphicsScene
{
  Q_OBJECT

public:
  StructuralScene(QObject* parent = 0);
  virtual ~StructuralScene();

protected:
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

protected slots:
  void performUndo();
  void performRedo();
  void performSnapshot();
  void performInsert(Structural::EntitySubtype);

private:
  void createMenus();
  void createConnections();

  StructuralMenu* _menu;

  QPointF _insertPoint;
};

#endif // QNSTSCENE_H
