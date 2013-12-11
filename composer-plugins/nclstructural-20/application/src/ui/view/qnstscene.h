#ifndef QNSTSCENE_H
#define QNSTSCENE_H

#include <QGraphicsSceneContextMenuEvent>

#include <QGraphicsScene>
#include <QUuid>

#include <QDebug>

#include "qnstmenu.h"

class QnstScene : public QGraphicsScene
{
  Q_OBJECT

public:
  QnstScene(QObject* parent = 0);

  ~QnstScene();

  QVector <QnstEntity* > getRoots() const;

  void addRoot(QnstEntity* root);

  void removeRoot(QnstEntity* root);

signals:
  void entityAdded(QnstEntity* entity);

  void entityRemoved(QnstEntity* entity);

  void entityChanged(QnstEntity* entity);

  void entitySelected(QnstEntity* entity);

  void entityAboutToChange(QnstEntity* entity,
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

  QnstMenu* menu;

  QVector <QnstEntity*> roots;
};

#endif // QNSTSCENE_H
