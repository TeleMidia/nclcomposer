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
  StructuralScene(QObject* _parent = 0);
  virtual ~StructuralScene();

  StructuralMenu* getMenu() const;
  void setMenu(StructuralMenu* menu);

  StructuralMenu* _menu;

protected:
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

protected slots:
  void performUndo();
  void performRedo();
  void performSnapshot();
  void performInsert(Structural::StructuralType);

private:
  void createMenus();
  void createConnections();

  QPointF _insertPoint;
};

#endif // QNSTSCENE_H
