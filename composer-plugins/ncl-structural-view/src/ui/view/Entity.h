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
#include <QUuid>

#include <QDebug>

#include "Globals.h"
#include "Menu.h"

class QnstMenu;

class QnstEntity : public QObject, public QGraphicsItem
{
  Q_OBJECT

public:
  QnstEntity(QnstEntity* parent = 0);
  ~QnstEntity();

  QString getnstUid() const;
  void setnstUid(const QString &uid);

  QnstType getnstType() const;
  virtual void setnstType(const QnstType type);

  QnstSubtype getnstSubtype() const;
  virtual void setnstSubtype(const QnstSubtype subtype);

  QMap<QString, QString> getnstProperties() const;
  virtual void setnstProperties(const QMap<QString, QString> &properties);

  QString getnstProperty(const QString &name);
  virtual void setnstProperty(const QString &name, const QString &value);

  QnstEntity* getnstParent() const;
  virtual void setnstParent(QnstEntity* parent);

  QVector<QnstEntity*> getnstChildren();
  virtual void insertChild(QnstEntity* child);
  virtual void removeChild(QnstEntity* child);

  bool isMoveable() const;
  void setMoveable(bool moveable);

  bool isSelectable() const;
  void setSelectable(bool selected);

  bool isHoverable() const;
  void setHoverable(bool hoverable);

  bool isDraggable();
  void setDraggable(bool draggable);

  bool isResizable() const;
  void setResizable(bool resizable);

  bool isMoving() const;
  void setMoving(bool moving);

  bool isResizing() const;
  void setResizing(bool resizing);

  bool isHovering() const;
  void setHovering(bool hovering);

  bool isDragging() const;
  void setDragging(bool dragging);

  bool isSelected() const;
  void setSelected(bool selected);

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

  qreal getResizeAnchorWidth() const;
  void setResizeAnchorWidth(qreal resizeAnchorWidth);

  qreal getResizeAnchorHeight() const;
  void setResizeAnchorHeight(qreal resizeAnchorHeight);

  QnstResizeType getResizeType() const;
  void setResizeType(QnstResizeType resizeType);

  qreal getzIndex() const;
  void setzIndex(qreal zIndex);

public slots:
  virtual void newChild(QnstSubtype subtype);

signals:
  void inserted(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void removed(QString uid, QMap<QString, QString> settings);
  void changed(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void selected(QString uid, QMap<QString, QString> settings);

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
  QString uid;
  QnstType type;
  QnstSubtype subtype;
  QMap<QString, QString> properties;

  QnstEntity* parent;
  QVector<QnstEntity*> children;

  bool moveable;
  bool selectable;
  bool resizable;
  bool draggable;
  bool hoverable;

  bool moving;
  bool dragging;
  bool resizing;
  bool hovering;

  bool selectedd;

  qreal top;
  qreal left;
  qreal width;
  qreal height;

  qreal zIndex;

  qreal moveTop;
  qreal moveLeft;

  qreal pressTop;
  qreal pressLeft;
  qreal pressWidth;
  qreal pressHeight;

  qreal resizeTop;
  qreal resizeLeft;
  qreal resizeWidth;
  qreal resizeHeight;

  qreal resizeAnchorWidth;
  qreal resizeAnchorHeight;

  QnstResizeType resizeType;

/****************************************************************************/

public:
  void addAngle(QString uid, int angle);
  void removeAngle(QString uid, int angle);
  bool hasMouseHover();
  void setMouseHover(bool hover);
  void setMouseHoverHighlight(bool enable);
  virtual void setnstId(const QString &id);
  virtual void updateToolTip() {}
  virtual void adjust(bool avoidCollision = false) = 0;

protected:
  bool hover, hasError, enableMouseHoverHighlight;
  QString erroMsg;
  QString dropsrc;


public:
  QnstMenu* menu;
  void setError(bool hasError);
  void setErrorMsg(QString erroMsg);

protected:
  virtual void drawMouseHoverHighlight(QPainter *painter);
};

#endif // QNSTENTITY_H
