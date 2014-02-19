#ifndef QNSTSCENE_H
#define QNSTSCENE_H

#include <QGraphicsSceneContextMenuEvent>

#include <QGraphicsScene>

#include <QDebug>

#include "StructuralMenu.h"

#include "StructuralEntity.h"
#include "StructuralGraphicsBody.h"

class StructuralScene : public QGraphicsScene
{
  Q_OBJECT

public:
  StructuralScene(QObject* parent = 0);

  ~StructuralScene();

  QVector <StructuralGraphicsEntity* > getRoots() const;

  void addRoot(StructuralGraphicsEntity* root);

  void removeRoot(StructuralGraphicsEntity* root);

signals:
  void entityAdded(StructuralGraphicsEntity* entity);

  void entityRemoved(StructuralGraphicsEntity* entity);

  void entityChanged(StructuralGraphicsEntity* entity);

  void entitySelected(StructuralGraphicsEntity* entity);

  void entityAboutToChange(StructuralGraphicsEntity* entity,
                           QMap<QString, QString> properties);

  // void undoRequested();

  // void redoRequested();

  void cutRequested();

  void copyRequested();

  void pasteRequested();

  void deleteRequested();

  void exportRequested();

  void zoominRequested();

  void zoomoutRequested();

  void zoomresetRequested();

  void fullscreenRequested();

protected:
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

protected slots:
  void performBody();

private:
  void createObjects();

  void createConnections();

  StructuralMenu* menu;

  QVector <StructuralGraphicsEntity*> roots;
};

#endif // QNSTSCENE_H
