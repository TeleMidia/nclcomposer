#ifndef LAYOUTSCENE_H
#define LAYOUTSCENE_H

#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>

#include "layoutitem.h"

#include <QGraphicsScene>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class LayoutScene : public QGraphicsScene
{
    Q_OBJECT

public:
    LayoutScene(QObject* parent = 0);
    virtual ~LayoutScene();

    void adjustItems();

public slots:
    virtual void createItem();
//    virtual void addItem(AbstractItem* item);
//    virtual void removeItem(LayoutItem* item);

protected:
    virtual void contextMenuEvent(
            QGraphicsSceneContextMenuEvent* contextMenuEvent);

signals:
    void itemAdded(LayoutItem* item);
    void itemRemoved(LayoutItem* item);


private:
    void createMenus();
    void createActions();
    void createConnections();

    QMenu* insertMenu;
    QMenu* contextMenu;

    QPointF insertPoint;

    QAction* regionAction;
};

}
}
}
}
}

#endif // LAYOUTSCENE_H
