#ifndef LAYOUTITEM_H
#define LAYOUTITEM_H

#include "ui/view/abstractitem.h"

#include <QPainter>
#include <QAction>
#include <QMenu>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class LayoutItem : public AbstractItem
{
    Q_OBJECT

public:
    LayoutItem(QGraphicsItem* parent = 0);

    virtual ~LayoutItem();

    QSizeF getSize() const;
    void setSize(const QSizeF size);

    bool isMoving() const;
    void setMoving(bool moving);

    bool isResizing() const;
    void setResizing(bool resizing);

    bool isSelected() const;
    void setSelected(bool selected);

    QString getTile() const;
    void setTitle(const QString &title);

    qreal getpTop() const;
    void setpTop(const qreal & ptop);

    qreal getpLeft() const;
    void setpLeft(const qreal & pleft);

    qreal getpWidth() const;
    void setpWidth(const qreal & pwidth);

    qreal getpHeight() const;
    void setpHeight(const qreal & pheight);

    void adjustItem();

public slots:
    virtual void addItem(AbstractItem* item);
    virtual void removeItem(AbstractItem* item);

protected:
    virtual QPainterPath shape() const;
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);

    virtual void contextMenuEvent(
            QGraphicsSceneContextMenuEvent* contextMenuEvent);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
protected slots:
    void addItem();

private:
    void move(QGraphicsSceneMouseEvent* event);
    void resize(QGraphicsSceneMouseEvent* event);
    void resize(LayoutItem* parent, qreal pw, qreal ph);

    void createMenus();
    void createActions();
    void createConnections();

    QMenu* insertMenu;
    QMenu* contextMenu;

    QPointF insertPoint;

    QPointF pressPoint;


    QSizeF size;

    QAction* regionAction;

    bool moving;
    bool resizing;
    bool topleft;
    bool selected;

    QString title;

    int rindex;

    qreal ptop;
    qreal pleft;
    qreal pwidth;
    qreal pheight;
};

}
}
}
}
}

#endif // LAYOUTITEM_H
