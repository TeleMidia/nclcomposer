#ifndef STRUCTURALENTITY_H
#define STRUCTURALENTITY_H

#include <QCursor>
#include <QDrag>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>

#include "Structural.h"
#include "StructuralMenu.h"

class StructuralMenu;

class StructuralEntity : public QObject, public QGraphicsItem
{
  Q_OBJECT

public:
  StructuralEntity (StructuralEntity *parent = 0);
  virtual ~StructuralEntity ();

  QString getId () const;
  void setId (const QString &id);

  QString getUid () const;
  void setUid (const QString &uid);

  StructuralCategory getCategory () const;
  void setCategory (StructuralCategory category);

  StructuralType getType () const;
  void setType (StructuralType type);

  StructuralResize getStructuralResize () const;
  void setStructuralResize (StructuralResize resize);

  QMap<QString, QString> getProperties () const;
  void setProperties (const QMap<QString, QString> &properties);

  QString getProperty (const QString &name) const;
  void setProperty (const QString &name, const QString &value);

  StructuralEntity *getParent () const;
  void setParent (StructuralEntity *parent);

  QVector<StructuralEntity *> getChildren () const;
  void addChild (StructuralEntity *entity);
  void removeChild (StructuralEntity *entity);
  bool isChild (StructuralEntity *child) const;

  QVector<QString> getRestrictions () const;
  void addRestriction (const QString &restriction);
  void removeRestriction (const QString &restriction);

  bool isMoveable () const;
  void setMoveable (bool moveable);

  bool isSelectable () const;
  void setSelectable (bool selectable);

  bool isHoverable () const;
  void setHoverable (bool hoverable);

  bool isDraggable ();
  void setDraggable (bool draggable);

  bool isResizable () const;
  void setResizable (bool resizable);

  bool isMoving () const;
  void setMoving (bool moving);

  bool isHidden () const;
  void setHidden (bool hidden);

  bool isResizing () const;
  void setResizing (bool resizing);

  bool isHovering () const;
  void setHovering (bool hovering);

  bool isDragging () const;
  void setDragging (bool dragging);

  bool isSelected () const;
  void setSelected (bool selectAsked);

  bool isCollapsed () const;
  void setCollapsed (bool collapsed);

  bool isReference () const;
  void setReference (bool reference);

  qreal getTop () const;
  void setTop (qreal top);

  qreal getLeft () const;
  void setLeft (qreal left);

  qreal getWidth () const;
  void setWidth (qreal width);

  qreal getHeight () const;
  void setHeight (qreal height);

  qreal getMoveTop () const;
  void setMoveTop (qreal moveTop);

  qreal getMoveLeft () const;
  void setMoveLeft (qreal moveLeft);

  qreal getPressTop () const;
  void setPressTop (qreal pressTop);

  qreal getPressLeft () const;
  void setPressLeft (qreal pressLeft);

  qreal getPressWidth () const;
  void setPressWidth (qreal pressWidth);

  qreal getPressHeight () const;
  void setPressHeight (qreal pressHeight);

  qreal getResizeTop () const;
  void setResizeTop (qreal resizeTop);

  qreal getResizeLeft () const;
  void setResizeLeft (qreal resizeLeft);

  qreal getResizeWidth () const;
  void setResizeWidth (qreal resizeWidth);

  qreal getResizeHeight () const;
  void setResizeHeight (qreal resizeHeight);

  qreal getUncollapsedTop () const;
  void setUncollapsedTop (qreal uncollapedTop);

  qreal getUncollapsedLeft () const;
  void setUncollapsedLeft (qreal uncollapedLeft);

  qreal getUncollapsedWidth () const;
  void setUncollapsedWidth (qreal uncollapedWidth);

  qreal getUncollapsedHeight () const;
  void setUncollapsedHeight (qreal uncollapedHeight);

  QRect getUncollapsedRect ();

  qreal getzIndex () const;
  void setzIndex (qreal zIndex);

  void setMenu (StructuralMenu *menu);

  QString getError () const;
  void setError (const QString &error);

  QString getWarning () const;
  void setWarning (const QString &warning);

  QString getInfo () const;
  void setInfo (const QString &info);

  virtual void adjust (bool collision = false, bool recursion = true);

signals:
  void insertAsked (QString uid, QString parent,
                    QMap<QString, QString> properties,
                    QMap<QString, QString> settings);

  //  void removed (QString uid, QMap<QString, QString> settings);

  void changeAsked (QString uid, QMap<QString, QString> props,
                    QMap<QString, QString> previous,
                    QMap<QString, QString> settings);

  void selectAsked (QString uid, QMap<QString, QString> stgs);
  void moveAsked (QString uid, QString parent, QMap<QString, QString> props,
                  QMap<QString, QString> stgs);

protected:
  virtual void draw (QPainter *painter) = 0;
  virtual void delineate (QPainterPath *painter) const;

  virtual void move (QGraphicsSceneMouseEvent *event);
  virtual void resize (QGraphicsSceneMouseEvent *event);

  virtual QPainterPath shape () const;
  virtual QRectF boundingRect () const;
  virtual void paint (QPainter *painter,
                      const QStyleOptionGraphicsItem *option, QWidget *widget);

  virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
  virtual void mousePressEvent (QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent *event);

  virtual void hoverMoveEvent (QGraphicsSceneHoverEvent *event);
  virtual void hoverEnterEvent (QGraphicsSceneHoverEvent *event);
  virtual void hoverLeaveEvent (QGraphicsSceneHoverEvent *event);

  virtual void dragEnterEvent (QGraphicsSceneDragDropEvent *event);
  virtual void dragMoveEvent (QGraphicsSceneDragDropEvent *event);
  virtual void dropEvent (QGraphicsSceneDragDropEvent *event);

  virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent *event);

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
  bool _collapsed;

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
  StructuralCategory _category;

  StructuralEntity *_parent;
  QVector<StructuralEntity *> _children;

  QVector<QString> _restrictions;

  QString _info;
  QString _error;
  QString _warnning;

  StructuralMenu *_menu;
  StructuralResize _resize;
};

#endif // STRUCTURALENTITY_H
