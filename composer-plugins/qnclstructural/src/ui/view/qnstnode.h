#ifndef QNSTNODE_H
#define QNSTNODE_H

#include "QMenu"
#include "QAction"
#include "QObject"
#include <QGraphicsSceneMouseEvent>
#include "QGraphicsSceneContextMenuEvent"

#include "qncgnode.h"
#include "qnstedge.h"
#include "qnstinterface.h"
#include "qnstentity.h"

#include "ui_qnstdbind.h"

#include <QDialog>

class QnstInterface;

class QnstNode : public QncgNode, public QnstEntity
{
    Q_OBJECT

public:
    QnstNode(QnstNode* parent = 0);

    ~QnstNode();

    QString getColor() const;

    void setColor(QString color);

    QString getBorderColor() const;

    void setBordertColor(QString borderColor);

    void setName(QString name);

signals:
    void entitySelected(QnstEntity* e);

    void entityAdded(QnstEntity* e);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void link(QGraphicsSceneMouseEvent* event);

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

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

    QAction* mediaAction;

    QAction* portAction;

    QAction* propertyAction;

    QAction* areaAction;

    QAction* aggregatorAction;

    QAction* bringfrontAction;

    QAction* bringforwardAction;

    QAction* sendbackwardAction;

    QAction* sendbackAction;

    QAction* propertiesAction;

private:
    void createActions();

    void createMenus();

    void createConnections();

    QString color;

    QString borderColor;

private slots:
    void internalselection();
};

#endif // QNSTNODE_H
