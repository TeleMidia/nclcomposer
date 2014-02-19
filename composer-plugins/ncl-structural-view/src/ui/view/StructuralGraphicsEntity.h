#ifndef QNSTGRAPHICSENTITY_H
#define QNSTGRAPHICSENTITY_H

#include <QAction>
#include <QFileDialog>

#include <QDebug>

#include "StructuralEntity.h"
#include "qncggraphicsentity.h"

#include "StructuralMenu.h"

class StructuralMenu;

//
// ATTENTION: This code needs a refactoring.
//

class StructuralGraphicsEntity : public QncgGraphicsEntity, public StructuralEntity
{
Q_OBJECT

public:
  StructuralGraphicsEntity(StructuralGraphicsEntity* parent = 0);

  virtual ~StructuralGraphicsEntity();

  StructuralGraphicsEntity* getnstGraphicsParent() const;

  void setnstGraphicsParent(StructuralGraphicsEntity* parent);

  void addAngle(QString uid, int angle);

  void removeAngle(QString uid, int angle);

  QSet<StructuralGraphicsEntity*> getnstGraphicsEntities();

  void addnstGraphicsEntity(StructuralGraphicsEntity* entity);

  void removenstGraphicsEntity(StructuralGraphicsEntity* entity);

  bool hasMouseHover();

  void setMouseHover(bool hover);

  void setMouseHoverHighlight(bool enable);

  virtual void setnstId(const QString &id);

  virtual void updateToolTip() {}

  virtual void setProperties(const QMap <QString, QString> &properties);

  virtual void getProperties(QMap <QString, QString> &properties);

  virtual void adjust(bool avoidCollision = false) = 0;

signals:
  void entityAdded(StructuralGraphicsEntity* entity);

  void entityRemoved(StructuralGraphicsEntity* entity);

  void entityChanged(StructuralGraphicsEntity* entity);

  void entityAboutToChange(StructuralGraphicsEntity* entity,
                           QMap<QString, QString> properties);

  void entitySelected(StructuralGraphicsEntity* entity);

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

  virtual void drawMouseHoverHighlight(QPainter *painter);

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

  virtual bool createEntity(Structural::EntityType type);

public:
  StructuralMenu* menu;
  bool isDraggable();
  void setDraggable(bool isDraggable);
  void setError(bool hasError);
  void setErrorMsg(QString erroMsg);

private:
  bool draggable;
  StructuralGraphicsEntity* parent;

  QSet<StructuralGraphicsEntity*> entities;

protected:
  bool hover, hasError, enableMouseHoverHighlight;
  QString erroMsg;
  QString dropsrc;
};

#endif // QNSTGRAPHICSENTITY_H
