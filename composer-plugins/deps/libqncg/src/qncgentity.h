#ifndef QNCGENTITY_H
#define QNCGENTITY_H

#include <QGraphicsItem>
#include <QWidget>
#include <QPainter>
#include <QRectF>
#include <QPainterPath>
#include <QLineF>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsLineItem>
#include <QObject>

#include <QGraphicsScene>

#define ROUND_DOUBLE(x) x<0.0?x=0.0:(x>100.0?x=0.0:x)

#include "qncg.h"

class QncgEntity :public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    QncgEntity(QncgEntity* parent = 0);

    virtual ~QncgEntity();

    void setMoveable(bool moveable);

    bool isMoveable() const;

    void setSelectable(bool selected);

    bool isSelectable() const;

    void setFocusable(bool focusable);

    bool isFocusable() const;

    void setResizable(bool resizable);

    bool isResizable() const;

    void setLinkable(bool linkable);

    bool isLinkable() const;

    void setMoving(bool moving);

    bool isMoving() const;

    void setLinking(bool linking);

    bool isLinking() const;

    void setResizing(bool resizing);

    bool isResizing() const;

    void setInvalid(bool invalid);

    bool isInvalid() const;

    void setSelected(bool selected);

    bool isSelected() const;

    qreal getTop() const;

    void setTop(qreal top);

    qreal getMoveTop() const;

    void setMoveTop(qreal moveTop);

    qreal getPressTop() const;

    void setPressTop(qreal pressTop);

    qreal getResizeTop() const;

    void setResizeTop(qreal resizeTop);

    qreal getRelativeTop() const;

    void setRelativeTop(qreal relativeTop);

    qreal getLeft() const;

    void setLeft(qreal left);

    qreal getMoveLeft() const;

    void setMoveLeft(qreal moveLeft);

    qreal getPressLeft() const;

    void setPressLeft(qreal pressLeft);

    qreal getResizeLeft() const;

    void setResizeLeft(qreal resizeLeft);

    qreal getRelativeLeft() const;

    void setRelativeLeft(qreal relativeLeft);

    qreal getRight() const;

    void setRight(qreal right);

    qreal getRelativeRight() const;

    void setRelativeRight(qreal relativeRight);

    qreal getBottom() const;

    void setBottom(qreal bottom);

    qreal getRelativeBottom() const;

    void setRelativeBottom(qreal relativeBottom);

    qreal getWidth() const;

    void setWidth(qreal width);

    qreal getPressWidth() const;

    void setPressWidth(qreal pressWidth);

    qreal getResizeWidth() const;

    void setResizeWidth(qreal resizeWidth);

    qreal getRelativeWidth() const;

    void setRelativeWidth(qreal relativeWidth);

    qreal getHeight() const;

    void setHeight(qreal height);

    qreal getPressHeight() const;

    void setPressHeight(qreal pressHeight);

    qreal getResizeHeight() const;

    void setResizeHeight(qreal resizeHeight);

    qreal getRelativeHeight() const;

    void setRelativeHeight(qreal relativeHeight);

    int getResizeType() const;

    void setResizeType(int resizeType);

    int getEntityType() const;

    void setEntityType(int entityType);

    QGraphicsLineItem* getSceneEdge() const;

    virtual void adjust() = 0;

signals:
    void entitySelected();

protected:
    virtual void validate() = 0;

    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void link(QGraphicsSceneMouseEvent* event) = 0;

    virtual void move(QGraphicsSceneMouseEvent* event) = 0;

    virtual void resize(QGraphicsSceneMouseEvent* event) = 0;

    virtual QPainterPath shape() const;

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);

    virtual void keyPressEvent(QKeyEvent *event);

    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*event);

private:
    bool moveable;

    bool selectable;

    bool focusable;

    bool resizable;

    bool linkable;

    bool moving;

    bool linking;

    bool resizing;

    bool invalid;

    bool selected;

    qreal top;

    qreal moveTop;

    qreal pressTop;

    qreal resizeTop;

    qreal relativeTop;

    qreal left;

    qreal moveLeft;

    qreal pressLeft;

    qreal resizeLeft;

    qreal relativeLeft;

    qreal right;

    qreal relativeRight;

    qreal bottom;

    qreal relativeBottom;

    qreal width;

    qreal pressWidth;

    qreal resizeWidth;

    qreal relativeWidth;

    qreal height;

    qreal pressHeight;

    qreal resizeHeight;

    qreal relativeHeight;

    int resizeType;

    int entityType;

    QGraphicsLineItem* sceneEdge;
};

#endif // QNCGENTITY_H
