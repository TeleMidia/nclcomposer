#ifndef QNSTGRAPHICSINTERFACE_H
#define QNSTGRAPHICSINTERFACE_H

#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>

#include "qnstentity.h"
#include "qncggraphicsinterface.h"
#include "qnstgraphicsnode.h"

class QnstGraphicsInterface : public QncgGraphicsInterface, public QnstEntity
{
    Q_OBJECT

public:
    QnstGraphicsInterface(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsInterface();

    QString getIcon() const;

    void setIcon(QString icon);

signals:
    void entityAdded(QnstEntity* entity);

    void entityRemoved(QnstEntity* entity);

    void entityChanged(QnstEntity* entity);

    void entitySelected(QnstEntity* entity);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

    virtual void	keyPressEvent(QKeyEvent* event);

protected slots:
    void requestEntityChange();

    void requestEntitySelection();

    void performDelete();

private:
    void createActions();

    void createMenus();

    void createConnections();

    QMenu* viewMenu;

    QMenu* insertMenu;

    QMenu* showMenu;

    QMenu* arrangeMenu;

    QMenu* adjustMenu;

    QMenu* organizeMenu;

    QMenu* contextMenu;

    QAction* helpAction;

    QAction* undoAction;

    QAction* redoAction;

    QAction* cutAction;

    QAction* copyAction;

    QAction* pasteAction;

    QAction* deleteAction;

    QAction* zoominAction;

    QAction* zoomoutAction;

    QAction* zoomresetAction;

    QAction* hideAction;

    QAction* fullscreenAction;

    QAction* exportAction;

    QAction* bodyAction;

    QAction* contextAction;

    QAction* switchAction;

    QAction* imageAction;

    QAction* videoAction;

    QAction* audioAction;

    QAction* textAction;

    QAction* scriptAction;

    QAction* settingsAction;

    QAction* portAction;

    QAction* propertyAction;

    QAction* areaAction;

    QAction* aggregatorAction;

    QAction* bringfrontAction;

    QAction* bringforwardAction;

    QAction* sendbackwardAction;

    QAction* sendbackAction;

    QAction* compactAction;

    QAction* clockAction;

    QAction* propertiesAction;

    QString icon;
};

#endif // QNSTGRAPHICSINTERFACE_H
