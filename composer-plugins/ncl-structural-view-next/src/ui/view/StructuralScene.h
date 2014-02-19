#ifndef QNSTSCENE_H
#define QNSTSCENE_H

#include <QGraphicsSceneContextMenuEvent>

#include <QGraphicsScene>
#include <QUuid>

#include <QDebug>

#include "StructuralMenu.h"

class StructuralScene : public QGraphicsScene
{
  Q_OBJECT

public:
  StructuralScene(QObject* parent = 0);

  ~StructuralScene();

  QVector <StructuralEntity* > getRoots() const;

  void addRoot(StructuralEntity* root);

  void removeRoot(StructuralEntity* root);

signals:
  void entityAdded(StructuralEntity* entity);

  void entityRemoved(StructuralEntity* entity);

  void entityChanged(StructuralEntity* entity);

  void entitySelected(StructuralEntity* entity);

  void entityAboutToChange(StructuralEntity* entity,
                           QMap<QString, QString> properties);

  void insertEntityResquested(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void removeEntityResquested(QString uid, QMap<QString, QString> settings);
  void changeEntityResquested(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void selectEntityResquested(QString uid, QMap<QString, QString> settings);

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
  void performAdd(QnstSubtype subtype);

private:
  void createObjects();

  void createConnections();

  StructuralMenu* menu;

  QVector <StructuralEntity*> roots;
};

#endif // QNSTSCENE_H
