#ifndef STRUCTURALENTITY_H
#define STRUCTURALENTITY_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QObject>
#include <QCursor>
#include <QDrag>
#include <QStyleOptionGraphicsItem>

#include "Structural.h"
#include "StructuralMenu.h"

class StructuralMenu;

class StructuralEntity : public QObject, public QGraphicsItem
{
  Q_OBJECT

public:
  StructuralEntity(StructuralEntity* parent = 0);
  virtual ~StructuralEntity();

  QString getStructuralId() const;
  void setStructuralId(const QString &id);

  QString getStructuralUid() const;
  void setStructuralUid(const QString &uid);

  StructualCategory getStructuralCategory() const;
  void setStructuralCategory(StructualCategory category);

  StructuralType getStructuralType() const;
  void setStructuralType(StructuralType type);

  StructuralResize getStructuralResize() const;
  void setStructuralResize(StructuralResize resize);

  QMap<QString, QString> getStructuralProperties() const;
  void setStructuralProperties(const QMap<QString, QString> &properties);

  QString getStructuralProperty(const QString &name) const;
  void setStructuralProperty(const QString &name, const QString &value);

  StructuralEntity* getStructuralParent() const;
  void setStructuralParent(StructuralEntity* parent);

  QVector<StructuralEntity*> getStructuralEntities() const;
  void addStructuralEntity(StructuralEntity* entity);
  void removeStructuralEntity(StructuralEntity* entity);

  QVector<QString> getStructuralRestrictions() const;
  void addStructuralRestriction(const QString &restriction);
  void removeStructuralRestriction(const QString &restriction);

  bool isMoveable() const;
  void setMoveable(bool moveable);

  bool isSelectable() const;
  void setSelectable(bool selectable);

  bool isHoverable() const;
  void setHoverable(bool hoverable);

  bool isDraggable();
  void setDraggable(bool draggable);

  bool isResizable() const;
  void setResizable(bool resizable);

  bool isMoving() const;
  void setMoving(bool moving);

  bool isHidden() const;
  void setHidden(bool hidden);

  bool isResizing() const;
  void setResizing(bool resizing);

  bool isHovering() const;
  void setHovering(bool hovering);

  bool isDragging() const;
  void setDragging(bool dragging);

  bool isSelected() const;
  void setSelected(bool selected);

  bool isUncollapsed() const;
  void setUncollapsed(bool uncollapsed);

  bool isReference() const;
  void setReference(bool reference);

  qreal getTop() const;
  void setTop(qreal top);

  qreal getLeft() const;
  void setLeft(qreal left);

  qreal getWidth() const;
  void setWidth(qreal width);

  qreal getHeight() const;
  void setHeight(qreal height);

  qreal getMoveTop() const;
  void setMoveTop(qreal moveTop);

  qreal getMoveLeft() const;
  void setMoveLeft(qreal moveLeft);

  qreal getPressTop() const;
  void setPressTop(qreal pressTop);

  qreal getPressLeft() const;
  void setPressLeft(qreal pressLeft);

  qreal getPressWidth() const;
  void setPressWidth(qreal pressWidth);

  qreal getPressHeight() const;
  void setPressHeight(qreal pressHeight);

  qreal getResizeTop() const;
  void setResizeTop(qreal resizeTop);

  qreal getResizeLeft() const;
  void setResizeLeft(qreal resizeLeft);

  qreal getResizeWidth() const;
  void setResizeWidth(qreal resizeWidth);

  qreal getResizeHeight() const;
  void setResizeHeight(qreal resizeHeight);

  qreal getUncollapedTop() const;
  void setUncollapedTop(qreal uncollapedTop);

  qreal getUncollapedLeft() const;
  void setUncollapedLeft(qreal uncollapedLeft);

  qreal getUncollapedWidth() const;
  void setUncollapedWidth(qreal uncollapedWidth);

  qreal getUncollapedHeight() const;
  void setUncollapedHeight(qreal uncollapedHeight);

  qreal getzIndex() const;
  void setzIndex(qreal zIndex);

  StructuralMenu* getMenu() const;
  void setMenu(StructuralMenu* menu);

  QString getError() const;
  void setError(const QString &error);

  QString getWarning() const;
  void setWarning(const QString &warning);

  QString getInfo() const;
  void setInfo(const QString &info);

  virtual void adjust(bool collision = false, bool recursion = true);

signals:
  void inserted(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void removed(QString uid, QMap<QString, QString> settings);
  void changed(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void selected(QString uid, QMap<QString, QString> settings);
  void move(QString uid, QString parent);

protected:
  virtual void draw(QPainter* painter) = 0;
  virtual void delineate(QPainterPath* painter) const;

  virtual void move(QGraphicsSceneMouseEvent* event);
  virtual void resize(QGraphicsSceneMouseEvent* event);

  virtual QPainterPath shape() const;
  virtual QRectF boundingRect() const;
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*event);

  virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

  virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
  virtual void dropEvent(QGraphicsSceneDragDropEvent* event);

  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

private:
  bool _moveable;
  bool _selectable;
  bool _resizable;
  bool _draggable;
  bool _hoverable;
  bool _focusable;

  bool _moving;
  bool _hidden;
  bool _dragging;
  bool _resizing;
  bool _hovering;

  bool _selected;
  bool _focused;
  bool _uncollapsed;

  bool _reference;

  qreal _top;
  qreal _left;
  qreal _width;
  qreal _height;

  qreal _zindex;

  qreal _moveTop;
  qreal _moveLeft;

  qreal _pressTop;
  qreal _pressLeft;
  qreal _pressWidth;
  qreal _pressHeight;

  qreal _resizeTop;
  qreal _resizeLeft;
  qreal _resizeWidth;
  qreal _resizeHeight;

  qreal _uncollapsedTop;
  qreal _uncollapsedLeft;
  qreal _uncollapsedWidth;
  qreal _uncollapsedHeight;

  QString _id;
  QString _uid;

  QMap<QString, QString> _properties;

  StructuralType _type;
  StructualCategory _category;

  StructuralEntity* _parent;
  QVector<StructuralEntity*> _children;

  QVector<QString> _restrictions;

  QString _info;
  QString _error;
  QString _warnning;

  StructuralMenu* _menu;
  StructuralResize _resize;
};

#endif // STRUCTURALENTITY_H
