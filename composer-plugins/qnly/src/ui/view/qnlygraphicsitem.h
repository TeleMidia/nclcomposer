#ifndef QNLYGRAPHICSITEM_H
#define QNLYGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QWidget>
#include <QObject>
#include <QPainterPath>
#include <QSizeF>
#include <QAction>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QRectF>
#include <QPointF>
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QLineEdit>
#include <QTextDocument>

#include "qnlygraphicsscene.h"

class QnlyGraphicsScene;

class QnlyGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    QnlyGraphicsItem(QnlyGraphicsItem* parent = 0);

    virtual ~QnlyGraphicsItem();

    enum QnlyResizeType {
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        TopLeft,
    };

    bool isMoving() const;

    void setMoving(const bool &moving);

    bool isResizing() const;

    void setResizing(const bool &resizing);

    bool isSelected() const;

    void setSelected(const bool &selected);

    bool isPainted() const;

    void setPainted(const bool &painted);

    bool hasChanged() const;

    void setChanged(const bool &changed);

    bool isValidated() const;

    void setValidated(const bool &validated);

    QString getId() const;

    void setId(const QString &id);

    QString getUid() const;

    void setUid(const QString &uid);

    QString getTitle() const;

    void setTitle(const QString &title);

    QString getColor() const;

    void setColor(const QString &color);

    qreal getzIndex() const;

    void setzIndex(const qreal &zindex);

    qreal getzIndexTop() const;

    qreal getTop() const;

    qreal getMoveTop() const;

    qreal getPressTop() const;

    qreal getResizeTop() const;

    qreal getRelativeTop() const;

    void setRelativeTop(const qreal &relativeTop);

    qreal getLeft() const;

    qreal getMoveLeft() const;

    qreal getPressLeft() const;

    qreal getResizeLeft() const;

    qreal getRelativeLeft() const;

    void setRelativeLeft(const qreal &relativeLeft);

    qreal getRight() const;

    qreal getRelativeRight() const;

    void setRelativeRight(const qreal &relativeRight);

    qreal getBottom() const;

    qreal getRelativeBottom() const;

    void setRelativeBottom(const qreal &relativeBottom);

    qreal getWidth() const;

    qreal getPressWidth() const;

    qreal getResizeWidth() const;

    qreal getRelativeWidth() const;

    void setRelativeWidth(const qreal &relativeWidth);

    qreal getHeight() const;

    qreal getPressHeight() const;

    qreal getResizeHeight() const;

    qreal getRelativeHeight() const;

    void setRelativeHeight(const qreal &relativeHeight);

    QnlyResizeType getResizeType() const;

    void adjust(bool repaint = true);



signals:
    void itemAdded(QnlyGraphicsItem* item, QnlyGraphicsItem* parent);

    void itemRemoved(QnlyGraphicsItem* item);

    void itemSelected(QnlyGraphicsItem* item);

    void itemChanged(QnlyGraphicsItem* item, const QMap<QString, QString> &attributes);

    void helpPerformed();

    void undoPerformed();

    void redoPerformed();

    void cutPerformed();

    void copyPerformed();

    void pastePerformed();

    void deletePerformed();

    void exportPerformed();

    void zoomInPerformed();

    void zoomOutPerformed();

    void zoomResetPerformed();

    void fullscreenPerformed();

    void regionPerformed();

    void regionBasePerformed();

    void frontPerformed();

    void forwardPerformed();

    void backwardPerformed();

    void backPerformed();

    void switchPerformed();

    void propertiesPerformed();

protected slots:
    void hide(bool v);

    virtual void performHelp();

    virtual void performUndo();

    virtual void performRedo();

    virtual void performCut();

    virtual void performCopy();

    virtual void performPaste();

    virtual void performDelete();

    virtual void performExport();

    virtual void performZoomIn();

    virtual void performZoomOut();

    virtual void performZoomReset();

    virtual void performFullscreen(bool checked);

    virtual void performRegion();

    virtual void performRegionBase();

    virtual void performFront();

    virtual void performForward();

    virtual void performBackward();

    virtual void performBack();

    virtual void performHide();

    virtual void performSwitch();

    virtual void performProperties();

protected:


    void setTop(const qreal &top);

    void setMoveTop(const qreal &moveTop);

    void setPressTop(const qreal &pressTop);

    void setResizeTop(const qreal &resizeTop);

    void setLeft(const qreal &left);

    void setMoveLeft(const qreal &moveLeft);

    void setPressLeft(const qreal &pressLeft);

    void setResizeLeft(const qreal &resizeLeft);

    void setRight(const qreal &right);

    void setBottom(const qreal &bottom);

    void setWidth(const qreal &width);

    void setPressWidth(const qreal &pressWidth);

    void setResizeWidth(const qreal &resizeWidth);

    void setHeight(const qreal &height);

    void setPressHeight(const qreal &pressHeight);

    void setResizeHeight(const qreal &resizeHeight);

    void setzIndexTop(const qreal &zindexTop);

    void setResizeType(const QnlyResizeType &resizeType);

    virtual void move(QGraphicsSceneMouseEvent* event);

    virtual void resize(QGraphicsSceneMouseEvent* event);

    virtual QPainterPath shape() const;

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

    void keyReleaseEvent(QKeyEvent * event);

    void keyPressEvent(QKeyEvent * event);

private:
    void createActions();

    void createMenus();

    void createConnections();

    bool moving;

    bool resizing;

    bool selected;

    bool validated;

    bool painted;

    bool changed;

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

    qreal zindex;

    qreal zindexTop;

    QMenu* viewMenu;

    QMenu* insertMenu;

    QMenu* showMenu;

    QMenu* arrangeMenu;

    QMenu* switchMenu;

    QMenu* contextMenu;

    QAction* helpAction;

    QAction* undoAction;

    QAction* redoAction;

    QAction* cutAction;

    QAction* copyAction;

    QAction* pasteAction;

    QAction* deleteAction;

    QAction* zoomInAction;

    QAction* zoomOutAction;

    QAction* zoomResetAction;

    QAction* hideAction;

    QAction* fullscreenAction;

    QAction* exportAction;

    QAction* itemAction;

    QAction* viewAction;

    QAction* frontAction;

    QAction* forwardAction;

    QAction* backwardAction;

    QAction* backAction;

    QAction* propertiesAction;

    QString id;

    QString uid;

    QString title;

    QString color;

    QnlyResizeType resizeType;
};

#endif // QNLYGRAPHICSITEM_H
