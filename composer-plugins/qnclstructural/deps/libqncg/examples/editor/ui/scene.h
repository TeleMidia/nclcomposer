#ifndef SCENE_H
#define SCENE_H

#include <QMenu>
#include <QAction>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

#include "node.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(QObject* parent = 0);

    ~Scene();

signals:
    void entityChanged(QncgGraphicsEntity* entity);

    void entitySelected(QncgGraphicsEntity* entity);

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

protected slots:
    void performNode();

private:
    void createActions();

    void createMenus();

    void createConnections();

    QMenu* insertMenu;

    QMenu* contextMenu;

    QAction* nodeAction;
};

#endif // SCENE_H
