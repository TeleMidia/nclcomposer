#ifndef QNSTENTITY_H
#define QNSTENTITY_H

#include <QAction>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QObject>
#include <QCursor>
#include <QDrag>
#include <QUuid>
#include <QStyleOptionGraphicsItem>

#include <QDebug>

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
  void setStructuralCategory(const StructualCategory category);

  StructuralType getStructuralType() const;
  virtual void setStructuralType(const StructuralType type);

  StructuralResize getStructuralResize() const;
  virtual void setStructuralResize(const StructuralResize resize);

  QMap<QString, QString> getStructuralProperties() const;
  void setStructuralProperties(const QMap<QString, QString> &properties);

  QString getStructuralProperty(const QString &name) const;
  virtual void setStructuralProperty(const QString &name, const QString &value);

  StructuralEntity* getStructuralParent() const;
  void setStructuralParent(StructuralEntity* parent);

  QVector<StructuralEntity*> getStructuralEntities();
  virtual void addStructuralEntity(StructuralEntity* entity);
  virtual void removeStructuralEntity(StructuralEntity* entity);

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

public slots:
  virtual void newChild(StructuralType _subtype);
  void performInsert(StructuralType _subtype);

signals:
  void inserted(QString _uid, QString _parent, QMap<QString, QString> _properties, QMap<QString, QString> settings);
  void removed(QString _uid, QMap<QString, QString> settings);
  void changed(QString _uid, QMap<QString, QString> _properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void selected(QString _uid, QMap<QString, QString> settings);
  void move(QString uid, QString newParent);

protected:
  virtual void draw(QPainter* painter) = 0;
  virtual void delineate(QPainterPath* painter) const = 0;
  virtual void move(QGraphicsSceneMouseEvent* event) = 0;
  virtual void resize(QGraphicsSceneMouseEvent* event) = 0;

  virtual QPainterPath shape() const;
  virtual QRectF boundingRect() const;
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*event);

  virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

private:
  void createMenus();
  void createConnections();

  void updateCursor(QGraphicsSceneHoverEvent* event);

  QString _id;
  QString _uid;
  QMap<QString, QString> _properties;

  StructuralEntity* _parent;
  QVector<StructuralEntity*> _children;

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

  qreal _top;
  qreal _left;
  qreal _width;
  qreal _height;

  qreal _zIndex;

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

  StructualCategory _type;
  StructuralType _subtype;
  StructuralResize _resize;

/****************************************************************************/

  QPointF _insertPoint;
public:
  void addAngle(QString _uid, int angle);
  void removeAngle(QString _uid, int angle);
  bool hasMouseHover();
  void setMouseHover(bool hover);
  void setMouseHoverHighlight(bool enable);
  virtual void refresh() {}
  virtual void adjust(bool avoidCollision = false, bool rec = true) = 0;

protected:
  bool hover, hasError, enableMouseHoverHighlight;
  QString erroMsg;
  QString dropsrc;

public:

  StructuralMenu* menu;
  void setError(bool hasError);
  void setErrorMsg(QString erroMsg);

protected:
  virtual void drawMouseHoverHighlight(QPainter *painter);
  virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
//  virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent* event);
//  virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
  virtual void dropEvent(QGraphicsSceneDragDropEvent* event);
};

#endif // QNSTENTITY_H
