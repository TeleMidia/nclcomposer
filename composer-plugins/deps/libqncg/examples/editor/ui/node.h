#ifndef NODE_H
#define NODE_H

#include <QMenu>
#include <QAction>

#include "qncggraphicsnode.h"

class Node : public QncgGraphicsNode
{
    Q_OBJECT

public:
    Node(QncgGraphicsEntity* parent = 0);

    ~Node();

signals:
    void entityChanged(QncgGraphicsEntity* entity);

    void entitySelected(QncgGraphicsEntity* entity);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

protected slots:
    void requestEntityChange();

    void requestEntitySelection();

    void performNode();

    void performFit();

    void performCompact();

    void performClock();

    void performInside();

    void performOutside();

    void performAttract();

    void performRepel();

private:
    void createActions();

    void createMenus();

    void createConnections();

    QMenu* insertMenu;

    QMenu* adjustMenu;

    QMenu* spreadMenu;

    QMenu* othersMenu;

    QMenu* contextMenu;

    QAction* nodeAction;

    QAction* compactAction;

    QAction* fitAction;

    QAction* clockAction;

    QAction* insideAction;

    QAction* outsideAction;

    QAction* attractAction;

    QAction* repelAction;
};

#endif // NODE_H
