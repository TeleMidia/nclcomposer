#include "qnlygraphicsscene.h"

QnlyGraphicsScene::QnlyGraphicsScene(QObject* parent) : QGraphicsScene(parent)
{
    createActions();
    createMenus();
    createConnections();
}

QnlyGraphicsScene::~QnlyGraphicsScene()
{
    delete(viewMenu);
    delete(insertMenu);
    delete(arrangeMenu);
    delete(switchMenu);
    delete(contextMenu);
}

void QnlyGraphicsScene::createActions()
{
    /* help action */
    helpAction = new QAction(this);
    helpAction->setText(tr("Help"));
    helpAction->setShortcut(QKeySequence("F1"));
    helpAction->setShortcutContext(Qt::WidgetShortcut);

    /* undo action */
    undoAction = new QAction(this);
    undoAction->setText(tr("Undo"));
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));

    /* redo action */
    redoAction = new QAction(this);
    redoAction->setText(tr("Redo"));
    redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

    /* cut action */
    cutAction = new QAction(this);
    cutAction->setText(tr("Cut"));
    cutAction->setShortcut(QKeySequence("Ctrl+X"));

    /* copy action */
    copyAction = new QAction(this);
    copyAction->setText(tr("Copy"));
    copyAction->setShortcut(QKeySequence("Ctrl+C"));

    /* paste action */
    pasteAction = new QAction(this);
    pasteAction->setText(tr("Paste"));
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));

    /* delete action */
    deleteAction = new QAction(this);
    deleteAction->setText(tr("Delete"));
    deleteAction->setShortcut(QKeySequence("Del"));

    /* zoomin action */
    zoominAction = new QAction(this);
    zoominAction->setText(tr("Zoom In"));
    zoominAction->setShortcut(QKeySequence("Ctrl++"));

    /* zoomout action */
    zoomoutAction = new QAction(this);
    zoomoutAction->setText(tr("Zoom Out"));
    zoomoutAction->setShortcut(QKeySequence("Ctrl+-"));

    /* reset action */
    zoomresetAction = new QAction(this);
    zoomresetAction->setText(tr("Reset"));
    zoomresetAction->setShortcut(QKeySequence("Ctrl+0"));

    /* fullscreen action */
    fullscreenAction = new QAction(this);
    fullscreenAction->setCheckable(true);
    fullscreenAction->setText(tr("Full Screen"));
    fullscreenAction->setShortcut(QKeySequence("Ctrl+F"));

    /* export action */
    exportAction = new QAction(this);
    exportAction->setText(tr("Export..."));

    /* item action */
    itemAction = new QAction(this);
    itemAction->setText(tr("Region"));

    /* view action */
    viewAction = new QAction(this);
    viewAction->setText(tr("Regionbase"));

    /* bring to front action */
    bringfrontAction = new QAction(this);
    bringfrontAction->setText(tr("Bring to Front"));
    bringfrontAction->setShortcutContext(Qt::ApplicationShortcut);
    bringfrontAction->setShortcut(QKeySequence("Shift+Ctrl+]"));

    /* bring forward action */
    bringforwardAction = new QAction(this);
    bringforwardAction->setText(tr("Bring Forward"));
    bringforwardAction->setShortcut(QKeySequence("Ctrl+]"));

    /* send backward action */
    sendbackwardAction = new QAction(this);
    sendbackwardAction->setText(tr("Send Backward"));
    sendbackwardAction->setShortcut(QKeySequence("Ctrl+["));

    /* send to back action */
    sendbackAction = new QAction(this);
    sendbackAction->setText(tr("Send to Back"));
    sendbackAction->setShortcut(QKeySequence("Shift+Ctrl+["));

    /* hide action */
    hideAction = new QAction(this);
    hideAction->setText(tr("Hide"));

    /* properties action */
    propertiesAction = new QAction(this);
    propertiesAction->setText(tr("Properties"));
}

void QnlyGraphicsScene::createMenus()
{
    /* view menu */
    viewMenu = new QMenu();
    viewMenu->setTitle(tr("View"));
    viewMenu->addAction(zoominAction);
    viewMenu->addAction(zoomoutAction);
    viewMenu->addAction(zoomresetAction);
    viewMenu->addSeparator();
    viewMenu->addAction(fullscreenAction);

    /* insert menu */
    insertMenu = new QMenu();
    insertMenu->setTitle(tr("Insert"));
    insertMenu->addAction(itemAction);
    insertMenu->addAction(viewAction);

    showMenu = new QMenu();
    showMenu->setTitle(tr("Show"));

    /* arrange menu */
    arrangeMenu = new QMenu();
    arrangeMenu->setTitle(tr("Arrange"));
    arrangeMenu->addAction(bringfrontAction);
    arrangeMenu->addAction(bringforwardAction);
    arrangeMenu->addAction(sendbackwardAction);
    arrangeMenu->addAction(sendbackAction);

    /* switch menu */
    switchMenu = new QMenu();
    switchMenu->setTitle(tr("Switch"));

    /* context menu */
    contextMenu = new QMenu();
    contextMenu->addAction(helpAction);
    contextMenu->addSeparator();
    contextMenu->addAction(undoAction);
    contextMenu->addAction(redoAction);
    contextMenu->addSeparator();
    contextMenu->addAction(cutAction);
    contextMenu->addAction(copyAction);
    contextMenu->addAction(pasteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(deleteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(exportAction);
    contextMenu->addSeparator();
    contextMenu->addMenu(viewMenu);
    contextMenu->addMenu(insertMenu);
    contextMenu->addMenu(showMenu);
    contextMenu->addMenu(arrangeMenu);
    contextMenu->addSeparator();
    contextMenu->addAction(hideAction);
    contextMenu->addSeparator();
    contextMenu->addMenu(switchMenu);
    contextMenu->addSeparator();
    contextMenu->addAction(propertiesAction);
}

void QnlyGraphicsScene::createConnections()
{
    connect(helpAction, SIGNAL(triggered()), SLOT(performHelp()));
    connect(undoAction, SIGNAL(triggered()), SLOT(performUndo()));
    connect(redoAction, SIGNAL(triggered()), SLOT(performRedo()));
    connect(cutAction, SIGNAL(triggered()), SLOT(performCut()));
    connect(copyAction, SIGNAL(triggered()), SLOT(performCopy()));
    connect(pasteAction, SIGNAL(triggered()), SLOT(performPaste()));
    connect(deleteAction, SIGNAL(triggered()), SLOT(performDelete()));
    connect(exportAction, SIGNAL(triggered()), SLOT(performExport()));
    connect(zoominAction, SIGNAL(triggered()), SLOT(performZoomin()));
    connect(zoomoutAction, SIGNAL(triggered()), SLOT(performZoomout()));
    connect(zoomresetAction, SIGNAL(triggered()), SLOT(performZoomreset()));
    connect(fullscreenAction, SIGNAL(triggered(bool)), SLOT(performFullscreen(bool)));
    connect(itemAction, SIGNAL(triggered()), SLOT(performItem()));
    connect(viewAction, SIGNAL(triggered()), SLOT(performView()));
    connect(bringfrontAction, SIGNAL(triggered()), SLOT(performBringfront()));
    connect(bringforwardAction, SIGNAL(triggered()), SLOT(performBringforward()));
    connect(sendbackwardAction, SIGNAL(triggered()), SLOT(performSendbackward()));
    connect(sendbackAction, SIGNAL(triggered()), SLOT(performBack()));
    connect(hideAction, SIGNAL(triggered()), SLOT(performHide()));
    connect(propertiesAction, SIGNAL(triggered()), SLOT(performProperties()));
}

void QnlyGraphicsScene::performHelp()
{

}

void QnlyGraphicsScene::performUndo()
{

}

void QnlyGraphicsScene::performRedo()
{

}

void QnlyGraphicsScene::performCut()
{

}

void QnlyGraphicsScene::performCopy()
{

}

void QnlyGraphicsScene::performPaste()
{

}

void QnlyGraphicsScene::performDelete()
{

}

void QnlyGraphicsScene::performZoomin()
{

}

void QnlyGraphicsScene::performZoomout()
{

}

void QnlyGraphicsScene::performZoomreset()
{

}

void QnlyGraphicsScene::performFullscreen(bool status)
{
    // emitting
    emit fullscreenPerformed(status);
}

void QnlyGraphicsScene::performExport()
{

}

void QnlyGraphicsScene::performItem()
{

}

void QnlyGraphicsScene::performView()
{

}

void QnlyGraphicsScene::performBringfront()
{

}

void QnlyGraphicsScene::performBringforward()
{

}

void QnlyGraphicsScene::performSendbackward()
{

}

void QnlyGraphicsScene::performSendback()
{

}

void QnlyGraphicsScene::performHide()
{

}

void QnlyGraphicsScene::performProperties()
{

}

void QnlyGraphicsScene::addItem(QnlyGraphicsItem* item, QnlyGraphicsItem* parent)
{
    if (item != NULL){
        if (!item->isPainted()){
            /* adding */
            if (parent != NULL){
                item->setParent(parent);
                item->setParentItem(parent);

            }else{
                QGraphicsScene::addItem(item);
            }

            item->setPainted(true);
        }

        // connections
        connect(item, SIGNAL(itemAdded(QnlyGraphicsItem*,QnlyGraphicsItem*)), SLOT(addItem(QnlyGraphicsItem*,QnlyGraphicsItem*)));
        connect(item, SIGNAL(itemSelected(QnlyGraphicsItem*)), SLOT(selectItem(QnlyGraphicsItem*)));
        connect(item, SIGNAL(itemChanged(QnlyGraphicsItem*,QMap<QString,QString>)), SIGNAL(itemChanged(QnlyGraphicsItem*,QMap<QString,QString>)));

        /* emitting signals */
        emit itemAdded(item, parent);
    }
}

void QnlyGraphicsScene::removeItem(QnlyGraphicsItem* item)
{
    if (item != NULL){
        if (item->isPainted()){
            /* removing */
            QGraphicsScene::removeItem(item);

            /* emitting signals */
            emit itemRemoved(item);
        }
    }
}

void QnlyGraphicsScene::selectItem(QnlyGraphicsItem* item)
{
    if (item != NULL){
        if (!item->isSelected()){
            /* selecting */
            if (selectedItem != NULL){
                selectedItem->setSelected(false);
            }

            item->setSelected(true);

            selectedItem = item;
        }

        /* emitting signals */
        emit itemSelected(item);
    }
}

void QnlyGraphicsScene::changeItem(QnlyGraphicsItem* item, const QMap<QString, QString> &attributes)
{
    if (item != NULL){
        if (!item->hasChanged()){
            /* changing */
            if (attributes["id"] != NULL){
                item->setId(attributes["id"]);

            }else{
                // no default value
            }

            if (attributes["title"] != NULL){
                item->setTitle(attributes["title"]);

            }else{
                // no default value
            }

            if (attributes["color"] != NULL){
                item->setColor(attributes["color"]);

            }else{
                // no default value
            }

            if (attributes["top"] != NULL){
                if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?%"))){
                    QString attribute = attributes["top"];
                    attribute.remove(attribute.length()-1,1); // removing '%'

                    qreal top = attribute.toDouble();

                    if (top >= 0 && top <= 100){
                        item->setRelativeTop(top/100);
                    }
                }else if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?"))){
                    QString attribute = attributes["top"];

                    qreal top = attribute.toDouble();

                    if (top >= 0 && top <= 1){
                        item->setRelativeTop(top);
                    }
                }
            }

            if (attributes["left"] != NULL){
                if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?%"))){
                    QString attribute = attributes["left"];
                    attribute.remove(attribute.length()-1,1); // removing '%'

                    qreal left = attribute.toDouble();

                    if (left >= 0 && left <= 100){
                        item->setRelativeLeft(left/100);
                    }

                }else if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?"))){
                    QString attribute = attributes["left"];

                    qreal left = attribute.toDouble();

                    if (left >= 0 && left <= 1){
                        item->setRelativeLeft(left);
                    }
                }
            }

            if (attributes["right"] != NULL){
                if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?%"))){
                    QString attribute = attributes["right"];
                    attribute.remove(attribute.length()-1,1); // removing '%'

                    qreal right = attribute.toDouble();

                    if (right >= 0 && right <= 100){
                        item->setRelativeRight(right/100);
                    }

                }else if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?"))){
                    QString attribute = attributes["right"];

                    qreal right = attribute.toDouble();

                    if (right >= 0 && right <= 1){
                        item->setRelativeRight(right);
                    }
                }
            }

            if (attributes["bottom"] != NULL){
                if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?%"))){
                    QString attribute = attributes["bottom"];
                    attribute.remove(attribute.length()-1,1); // removing '%'

                    qreal bottom = attribute.toDouble();

                    if (bottom >= 0 && bottom <= 100){
                        item->setRelativeBottom(bottom/100);
                    }

                }else if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?"))){
                    QString attribute = attributes["bottom"];

                    qreal bottom = attribute.toDouble();

                    if (bottom >= 0 && bottom <= 1){
                        item->setRelativeBottom(bottom);
                    }
                }
            }

            if (attributes["width"] != NULL){
                if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?%"))){
                    QString attribute = attributes["width"];
                    attribute.remove(attribute.length()-1,1); // removing '%'

                    qreal width = attribute.toDouble();

                    if (width >= 0 && width <= 100){
                        item->setRelativeWidth(width/100);
                    }

                }else if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?"))){
                    QString attribute = attributes["width"];

                    qreal width = attribute.toDouble();

                    if (width >= 0 && width <= 1){
                        item->setRelativeWidth(width);
                    }
                }
            }

            if (attributes["height"] != NULL){
                if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?%"))){
                    QString attribute = attributes["height"];
                    attribute.remove(attribute.length()-1,1); // removing '%'

                    qreal height = attribute.toDouble();

                    if (height >= 0 && height <= 100){
                        item->setRelativeHeight(height/100);
                    }

                }else if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?"))){
                    QString attribute = attributes["height"];

                    qreal height = attribute.toDouble();

                    if (height >= 0 && height <= 1){
                        item->setRelativeHeight(height);
                    }
                }
            }

            item->adjust();

            item->setChanged(true);
        }

        /* emitting signals */
        emit itemChanged(item,attributes);
    }
}

void QnlyGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsScene::contextMenuEvent(event);

    if (!event->isAccepted()){
        contextMenu->exec(event->screenPos());

        event->accept();
    }
}

void QnlyGraphicsScene::keyPressEvent(QKeyEvent *event)
{
    QGraphicsScene::keyPressEvent(event);

    if (!event->isAccepted()){

        // fullscreen
        if (event->key() == Qt::Key_F && event->modifiers() == Qt::ControlModifier){
            fullscreenAction->trigger();
        }

        event->accept();
    }
}
