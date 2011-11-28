#ifndef QNSTGRAPHICSCONTENT_H
#define QNSTGRAPHICSCONTENT_H

#include "qnstgraphicsnode.h"

#include "qnstgraphicsarea.h"
#include "qnstgraphicsproperty.h"

class QnstGraphicsContent : public QnstGraphicsNode
{
    Q_OBJECT

public:
    QnstGraphicsContent(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsContent();

    QString getIcon() const;

    void setIcon(QString icon);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

private:
    void createActions();

    void createMenus();

    void createConnections();

    QMenu* viewMenu;

    QMenu* insertMenu;

    QMenu* showMenu;

    QMenu* arrangeMenu;

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

    QAction* propertiesAction;

    QString icon;
};

#endif // QNSTGRAPHICSCONTENT_H
