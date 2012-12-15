#ifndef QNSTGRAPHICSENTITY_H
#define QNSTGRAPHICSENTITY_H

#include <QAction>
#include <QFileDialog>

#include <QDebug>

#include "qnstentity.h"
#include "qncggraphicsentity.h"

#include "qnstmenu.h"

class QnstMenu;

//
// ATTENTION: This code needs a refactoring.
//

class QnstGraphicsEntity : public QncgGraphicsEntity, public QnstEntity
{
Q_OBJECT

public:
  QnstGraphicsEntity(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsEntity();

  QnstGraphicsEntity* getnstGraphicsParent() const;

  void setnstGraphicsParent(QnstGraphicsEntity* parent);

  void addAngle(QString uid, int angle);

  void removeAngle(QString uid, int angle);

  QSet<QnstGraphicsEntity*> getnstGraphicsEntities();

  void addnstGraphicsEntity(QnstGraphicsEntity* entity);

  void removenstGraphicsEntity(QnstGraphicsEntity* entity);

  bool hasMouseHover();

  void setMouseHover(bool hover);

  virtual void setnstId(const QString &id);

  virtual void updateToolTip() {}

  virtual void setProperties(const QMap <QString, QString> &properties);

  virtual void getProperties(QMap <QString, QString> &properties);

  virtual void adjust(bool avoidCollision = false) = 0;

signals:
  void entityAdded(QnstGraphicsEntity* entity);

  void entityRemoved(QnstGraphicsEntity* entity);

  void entityChanged(QnstGraphicsEntity* entity);

  void entityAboutToChange(QnstGraphicsEntity* entity,
                           QMap<QString, QString> properties);

  void entitySelected(QnstGraphicsEntity* entity);

  void undoRequested();

  void redoRequested();

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
  virtual void draw(QPainter* painter) = 0;

  virtual void delineate(QPainterPath* painter) const = 0;

  virtual void move(QGraphicsSceneMouseEvent* event) = 0;

  virtual void resize(QGraphicsSceneMouseEvent* event) = 0;

  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);

  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

protected slots:
  void requestEntityChange();

  void requestEntityPreparation(QMap<QString, QString> properties);

  void requestEntitySelection();

  virtual bool createEntity(Qnst::EntityType type);

public:
  QnstMenu* menu;
  bool isDraggable();
  void setDraggable(bool isDraggable);
  void setError(bool hasError);

private:
  bool draggable;
  QnstGraphicsEntity* parent;

  QSet<QnstGraphicsEntity*> entities;

protected:
  bool hover, hasError;
  QString dropsrc;
};

#endif // QNSTGRAPHICSENTITY_H
