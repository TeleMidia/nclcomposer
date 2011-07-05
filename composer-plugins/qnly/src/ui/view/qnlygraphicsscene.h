#ifndef QNLYGRAPHICSSCENE_H
#define QNLYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QAction>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QDebug>

#include "qnlygraphicsview.h"
#include "qnlygraphicsitem.h"

class QnlyGraphicsItem;

class QnlyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    QnlyGraphicsScene(QObject* parent = 0);

    virtual ~QnlyGraphicsScene();

public slots:
    virtual void performHelp();

    virtual void performUndo();

    virtual void performRedo();

    virtual void performCut();

    virtual void performCopy();

    virtual void performPaste();

    virtual void performDelete();

    virtual void performZoomin();

    virtual void performZoomout();

    virtual void performZoomreset();

    virtual void performFullscreen(bool fullscreen);

    virtual void performExport();

    virtual void performItem();

    virtual void performView();

    virtual void performBringfront();

    virtual void performBringforward();

    virtual void performSendbackward();

    virtual void performSendback();

    virtual void performHide();

    virtual void performProperties();

    virtual void addItem(QnlyGraphicsItem* item, QnlyGraphicsItem* parent);

    virtual void removeItem(QnlyGraphicsItem* item);

    virtual void selectItem(QnlyGraphicsItem* item);

    virtual void changeItem(QnlyGraphicsItem* item, const QMap<QString, QString> &attributes);

signals:
    void fullscreenPerformed(bool status);

    void itemAdded(QnlyGraphicsItem* item, QnlyGraphicsItem* parent);

    void itemRemoved(QnlyGraphicsItem* item);

    void itemSelected(QnlyGraphicsItem* item);

    void itemChanged(QnlyGraphicsItem* item, const QMap<QString, QString> &attributes);

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);

private:
    void createActions();

    void createMenus();

    void createConnections();

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

    QAction* zoominAction;

    QAction* zoomoutAction;

    QAction* zoomresetAction;

    QAction* fullscreenAction;

    QAction* exportAction;

    QAction* itemAction;

    QAction* viewAction;

    QAction* bringfrontAction;

    QAction* bringforwardAction;

    QAction* sendbackwardAction;

    QAction* sendbackAction;

    QAction* hideAction;

    QAction* propertiesAction;

    QnlyGraphicsItem* selectedItem;
};

#endif // QNLYGRAPHICSSCENE_H
