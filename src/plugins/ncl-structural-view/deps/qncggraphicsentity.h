#ifndef QNCGGRAPHICSENTITY_H
#define QNCGGRAPHICSENTITY_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QObject>
#include <QCursor>

#include <QDebug>

#include "qncg.h"

typedef Qncg::EntityType QncgEntityType;
typedef Qncg::ResizeType QncgResizeType;

class QncgGraphicsEntity :public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    QncgGraphicsEntity(QncgGraphicsEntity* parent = 0);

    virtual ~QncgGraphicsEntity();

    void setMoveable(bool moveable);

    bool isMoveable() const;

    void setSelectable(bool selected);

    bool isSelectable() const;

    void setResizable(bool resizable);

    bool isResizable() const;

    void setMoving(bool moving);

    bool isMoving() const;

    void setResizing(bool resizing);

    bool isResizing() const;

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

    qreal getLeft() const;

    void setLeft(qreal left);

    qreal getMoveLeft() const;

    void setMoveLeft(qreal moveLeft);

    qreal getPressLeft() const;

    void setPressLeft(qreal pressLeft);

    qreal getResizeLeft() const;

    void setResizeLeft(qreal resizeLeft);

    qreal getWidth() const;

    void setWidth(qreal width);

    qreal getPressWidth() const;

    void setPressWidth(qreal pressWidth);

    qreal getResizeWidth() const;

    void setResizeWidth(qreal resizeWidth);

    qreal getHeight() const;

    void setHeight(qreal height);

    qreal getPressHeight() const;

    void setPressHeight(qreal pressHeight);

    qreal getResizeHeight() const;

    void setResizeHeight(qreal resizeHeight);

    int getzIndex() const;

    void setzIndex(int zindex);

    QncgResizeType getncgResize() const;

    void setncgResize(QncgResizeType resizeType);

    QncgEntityType getncgType() const;

    void setncgType(QncgEntityType entityType);

    QncgGraphicsEntity* getncgGraphicsParent() const;

    void setncgGraphicsParent(QncgGraphicsEntity* parent);

    QVector<QncgGraphicsEntity*> getncgGraphicsEntities();

    void addncgGraphicsEntity(QncgGraphicsEntity* entity);

    void removencgGraphicsEntity(QncgGraphicsEntity* entity);

    virtual void adjust(bool avoidCollision = false) = 0;

signals:
    void entityAboutToChange(QMap<QString, QString> properties);

    void entityChanged();

    void entitySelected();

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

private:
    bool moveable;

    bool selectable;

    bool resizable;

    bool moving;

    bool resizing;

    bool selected;

    qreal top;

    qreal moveTop;

    qreal pressTop;

    qreal resizeTop;

    qreal left;

    qreal moveLeft;

    qreal pressLeft;

    qreal resizeLeft;

    qreal width;

    qreal pressWidth;

    qreal resizeWidth;

    qreal height;

    qreal pressHeight;

    qreal resizeHeight;

    int zindex;

    QncgEntityType entityType;

    QncgResizeType resizeType;

    QncgGraphicsEntity* parent;

    QVector<QncgGraphicsEntity*> entities;
};

#endif // QNCGGRAPHICSENTITY_H
