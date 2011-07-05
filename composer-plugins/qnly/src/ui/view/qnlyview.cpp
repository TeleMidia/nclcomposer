#include "qnlyview.h"

QnlyView::QnlyView(QWidget* parent) : QStackedWidget(parent)
{
    setWindowTitle("Qnly");

    createActions();
    createMenus();
    createConnections();

    nregion = 0;

    nregionbase = 0;
}

QnlyView::~QnlyView()
{

}

int QnlyView::getnItem() const
{
    return nregion;
}

void QnlyView::setnItem(const int &nregion)
{
    this->nregion = nregion;
}

int QnlyView::getnView() const
{
    return nregionbase;
}

void QnlyView::setnView(const int &nview)
{
    this->nregionbase = nregionbase;
}

void QnlyView::createActions()
{
    /* help action */
    helpAction = new QAction(this);
    helpAction->setText(tr("Help"));
    helpAction->setShortcut(QKeySequence("F1"));

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
    deleteAction->setText(tr("Remove"));
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

    /* view base action */
    viewAction = new QAction(this);
    viewAction->setText(tr("Regionbase"));;

    /* bring to front action */
    bringfrontAction = new QAction(this);
    bringfrontAction->setText(tr("Bring to Front"));
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

    /* attributes action */
    propertiesAction = new QAction(this);
    propertiesAction->setText(tr("attributes"));
}

void QnlyView::createMenus()
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

void QnlyView::createConnections()
{

}

void QnlyView::addItem( QnlyGraphicsItem* item, QnlyGraphicsItem* parentitem,
                        QnlyGraphicsView* view)
{
    if (item != NULL && view != NULL)
    {
        // validating
        QString itemUID;

        if (!item->getUid().isEmpty())
        {
            if (!items.contains(item->getUid()))
            {
                itemUID = item->getUid();

            }
            else
            {
                qDebug() << "QnlyView::addItem Item already contains "
                        << item->getUid();
                return; // canceling addition
            }

        }
        else
        {
            itemUID = (QString) QUuid::createUuid();
            item->setUid(itemUID);
        }

        QString parentitemUID;

        if (parentitem != NULL)
        {
            if (items.contains(parentitem->getUid()))
            {
                parentitemUID = parentitem->getUid();

            }
            else
            {
                qDebug() << "QnlyView::addItem ParentItem doesn't in items  "
                        << parentitem->getUid();
                return; // canceling addition
            }

        }
        else
        {
            parentitemUID = "";
        }

        QString viewUID;

        if (!view->getUid().isEmpty())
        {
            if (views.contains(view->getUid()))
            {
                viewUID = view->getUid();
            }
            else
            {
                return; // canceling addition
            }

        }else{
            return; // canceling addition
        }

        // drawing
        qDebug() << "Calling internal QnlyView::addItem ";
        view->addItem(item, parentitem);

        // setting
        QMap<QString, QString> attributes;

        if (!item->getId().isEmpty()){
            attributes["id"] = item->getId();

        }else{
            attributes["id"] = "rg" + QString::number(nregion+1);

            item->setId(attributes["id"]); // updating item
        }

        if (!item->getTitle().isEmpty()){
            attributes["title"] = item->getTitle();

        }else{
            attributes["title"] = "rg" + QString::number(nregion+1);

            item->setTitle(attributes["title"]); // updating item
        }

        if (item->getRelativeLeft() >= 0 && item->getRelativeLeft() <= 1){
            attributes["top"] = QString::number(item->getRelativeTop()*100) + "%";

        }else{
            // no default value
        }

        if (item->getRelativeLeft() >= 0 && item->getRelativeLeft() <= 1){
            attributes["left"] = QString::number(item->getRelativeLeft()*100)+"%";

        }else{
            // no default values
        }

        if (item->getRelativeRight() >= 0 && item->getRelativeRight() <= 1){
            attributes["right"] = QString::number(item->getRelativeRight()*100)+"%";

        }else{
            // no default values
        }

        if (item->getRelativeBottom() >= 0 && item->getRelativeBottom() <= 1){
            attributes["bottom"] = QString::number(item->getRelativeBottom()*100)+"%";

        }else{
            // no default values
        }

        if (item->getRelativeWidth() >= 0 && item->getRelativeWidth() <= 1){
            attributes["width"] = QString::number(item->getRelativeWidth()*100)+"%";

        }else{
            // no default values
        }

        if (item->getRelativeHeight() >= 0 && item->getRelativeHeight() <= 1){
            attributes["height"] = QString::number(item->getRelativeHeight()*100)+"%";

        }else{
            // no default values
        }

        if (!item->getColor().isEmpty()){
            attributes["color"] = item->getColor();

        }else{
            // no default values
        }

        if (item->getzIndex() >= 0){
            attributes["zIndex"] = QString::number(item->getzIndex());

        }else{
            // no default values
        }

        // adding
        items[itemUID] = item;

        nregion++;

        // emitting
        emit itemAdded(itemUID, parentitemUID, viewUID, attributes);
    }
}

void QnlyView::removeItem(QnlyGraphicsItem* item, QnlyGraphicsView* view)
{
    /*
    if (item != NULL && view != NULL){

        QString item;

        if (item->get() != NULL){
            if (items.contains(item->get())){
                item = item->get();

            }else{
                return; // canceling remotion
            }
        }else{
            return; // canceling remotion
        }

        QString view;

        if (view->get() != NULL){
            if (views.contains(view->get())){
                view = view->get();

            }else{
                return; // canceling remotion
            }

        }else{
            return; // canceling remotion
        }


        if (item->isPainted()){
            view->removeItem(item);
        }

        items.remove(item->get());


        emit itemRemoved(item, view);
    }
    */
}

void QnlyView::selectItem(QnlyGraphicsItem* item, QnlyGraphicsView* view)
{
  /*  if (item != NULL && view != NULL){

        QString item;

        if (item->get() != NULL){
            if (items.contains(item->get())){
                item = item->get();

            }else{
                return; // canceling selection
            }
        }else{
            return; // canceling selection
        }

        QString view;

        if (view->get() != NULL){
            if (views.contains(view->get())){
                view = view->get();

            }else{
                return; // canceling selection
            }

        }else{
            return; // canceling selection
        }


        if (!item->isSelected()){
            view->selectItem(item);
        }


        emit itemSelected(item, view);
    }*/
}

void QnlyView::changeItem(QnlyGraphicsItem* item, QnlyGraphicsView* view, const QMap<QString, QString> &attributes)
{

    qDebug() << " QnlyView::changeItem";
    if (item != NULL && view != NULL){

        QString itemUID;

        if (!item->getUid().isEmpty()){
            if (items.contains(item->getUid())){
                itemUID = item->getUid();

            }else{
                return; // canceling change
            }
        }else{
            return; // canceling change
        }

        QString viewUID;

        if (!view->getUid().isEmpty()){
            if (views.contains(view->getUid())){
                viewUID = view->getUid();

            }else{
                return; // canceling change
            }

        }else{
            return; // canceling change
        }


        if (!item->hasChanged()){
            view->changeItem(item, attributes);
        }

        emit itemChanged(itemUID, viewUID, attributes);
    }
}

void QnlyView::addView(QnlyGraphicsView* view)
{
   if (view != NULL){

        QString viewUID;

        if (!view->getUid().isEmpty()){
            if (!views.contains(view->getUid())){
                viewUID = view->getUid();

            }else{
                return; // canceling addition
            }
        }else{

//            view->setUid(view); // updating view
        }

        QMap<QString, QString> attributes;

        if (!view->getId().isEmpty()){
            attributes["id"] = view->getId();

        }else{
            attributes["id"] = "rgbase" + QString::number(nregionbase);

            view->setId(attributes["id"]); // updating view
        }

        if (!view->getDevice().isEmpty()){
            attributes["device"] = view->getDevice();

        }else{
            // no default value
        }

        if (!view->getRegion().isEmpty()){
            attributes["region"] = view->getRegion();

        }else{
            // no default value
        }


//        connect(view, SIGNAL(fullscreenPerformed(bool)), SLOT(performFullscreen(bool)));

//        connect(view, SIGNAL(itemAdded(QnlyGraphicsItem*,QnlyGraphicsItem*,QnlyGraphicsView*)), SLOT(addItem(QnlyGraphicsItem*,QnlyGraphicsItem*,QnlyGraphicsView*)));
//        connect(view, SIGNAL(itemRemoved(QnlyGraphicsItem*,QnlyGraphicsView*)), SLOT(removeItem(QnlyGraphicsItem*,QnlyGraphicsView*)));
//        connect(view, SIGNAL(itemSelected(QnlyGraphicsItem*,QnlyGraphicsView*)), SLOT(selectItem(QnlyGraphicsItem*,QnlyGraphicsView*)));
        connect(view, SIGNAL(itemChanged(QnlyGraphicsItem*,QnlyGraphicsView*,QMap<QString,QString>)), SLOT(changeItem(QnlyGraphicsItem*,QnlyGraphicsView*,QMap<QString,QString>)));


        addWidget(view);
        setCurrentWidget(view);

        views[viewUID] = view;

        nregionbase++;
//        setViewCounter(nview+1);


//        emit viewAdded(view, attributes);
    }
}

void QnlyView::removeView(QnlyGraphicsView* view)
{
   /* if (view != NULL){

        QString view;

        if (view->get() != NULL){
            if (views.contains(view->get())){
                view = view->get();

            }else{
                return; // canceling remotion
            }

        }else{
            return; // canceling remotion
        }


        removeWidget(view);

        views.remove(view);


        emit viewRemoved(view);
    }*/
}

void QnlyView::selectView(QnlyGraphicsView* view)
{
   /* if (view != NULL){

        QString view;

        if (view->get() != NULL){
            if (views.contains(view->get())){
                view = view->get();

            }else{
                return; // canceling selection
            }

        }else{
            return; // canceling selection
        }


        setCurrentWidget(view);


        emit viewSelected(view);
    }*/
}

void QnlyView::changeView(QnlyGraphicsView* view, const QMap<QString, QString> &attributes)
{
   /* if (view != NULL){

        QString view;

        if (view->get() != NULL){
            if (views.contains(view->get())){
                view = view->get();

            }else{
                return; // canceling change
            }

        }else{
            return; // canceling change
        }


        if (attributes["id"] != NULL){
            view->setId(attributes["id"]);
        }

        if (attributes["device"] != NULL){
            view->setDevice(attributes["device"]);
        }

        if (attributes["region"] != NULL){
            view->setRegion(attributes["region"]);
        }


        emit viewChanged(view, attributes);
    }*/
}

void QnlyView::addItem(const QString item, const QString parent, const QString view, const QMap<QString, QString> &attributes)
{

}

void QnlyView::removeItem(const QString item, const QString view)
{
    /*
    QnlyGraphicsItem* item;

    if (item != NULL){
        if (items.contains(item)){
           item = items.value(item);
        }else{
            return; // canceling addition
        }

    }else{
        // no default value
    }

    QnlyGraphicsView* view;

    if (view != NULL){
        if (views.contains(view)){
            view = views.value(view);
        }else{
            return; // canceling addition
        }

    }else{
        return; // canceling addition
    }


    removeItem(item, view);*/
}

void QnlyView::selectItem(const QString item, const QString view)
{
    /*
    QnlyGraphicsItem* item;

    if (item != NULL){
        if (items.contains(item)){
           item = items.value(item);
        }else{
            return; // canceling addition
        }

    }else{
        // no default value
    }

    QnlyGraphicsView* view;

    if (view != NULL){
        if (views.contains(view)){
            view = views.value(view);
        }else{
            return; // canceling addition
        }

    }else{
        return; // canceling addition
    }


    selectItem(item, view);
    */
}

void QnlyView::changeItem(const QString item, const QString view, const QMap<QString, QString> &attributes)
{
/*
    QnlyGraphicsItem* item;

    if (item != NULL){
        if (items.contains(item)){
           item = items.value(item);
        }else{
            return; // canceling addition
        }

    }else{
        // no default value
    }

    QnlyGraphicsView* view;

    if (view != NULL){
        if (views.contains(view)){
            view = views.value(view);
        }else{
            return; // canceling addition
        }

    }else{
        return; // canceling addition
    }

    item->setChanged(false);


    changeItem(item, view, attributes);
*/
}

void QnlyView::addView(const QString view, const QMap<QString, QString> &attributes)
{
/*
    qDebug() << "QnlyView::addView";

    QnlyGraphicsView* view;

    if (view != NULL){
        if (!views.contains(view)){
            view = new QnlyGraphicsView(this);
        }else{
            return; // canceling addition
        }

    }else{
        return; // canceling addition
    }

    if (attributes["id"] != NULL){
        view->setId(attributes["id"]);

    }else{
        // no default value
    }

    if (attributes["device"] != NULL){
        view->setDevice(attributes["device"]);

    }else{
        // no default value
    }

    if (attributes["region"] != NULL){
        view->setRegion(attributes["region"]);

    }else{
        // no default value
    }


    addView(view);
    */
}

void QnlyView::removeView(const QString view)
{
    /*
    QnlyGraphicsView* view;

    if (view != NULL){
        if (views.contains(view)){
            view = views.value(view);
        }else{
            return; // canceling addition
        }

    }else{
        return; // canceling addition
    }


    removeView(view);
    */
}

void QnlyView::selectView(const QString view)
{
    /*
    QnlyGraphicsView* view;

    if (view != NULL){
        if (views.contains(view)){
            view = views.value(view);
        }else{
            return; // canceling addition
        }

    }else{
        return; // canceling addition
    }


    selectView(view);
    */
}

void QnlyView::changeView(const QString view, const QMap<QString, QString> &attributes)
{
    /*
    QnlyGraphicsView* view;

    if (view != NULL){
        if (views.contains(view)){
            view = views.value(view);
        }else{
            return; // canceling addition
        }

    }else{
        return; // canceling addition
    }


    changeView(view, attributes);
    */
}

void QnlyView::selectRegion(const QString itemUID, const QString viewUID)
{

}

void QnlyView::selectRegionbase(const QString viewUID)
{

}

void QnlyView::addRegion( const QString regionUID,
                          const QString parentUID,
                          const QString regionbaseUID,
                          const QMap<QString, QString> &attributes)
{
     QnlyGraphicsItem* item = NULL;

     item = new QnlyGraphicsItem();

    if (!regionUID.isEmpty())
         item->setUid(regionUID);

     QnlyGraphicsItem* parent = NULL;

     if (!parentUID.isEmpty())
     {
         if (items.contains(parentUID))
         {
            parent = items.value(parentUID);
         }
         else
         {
             qDebug() << "QnlyView::addRegion parentUId=" << parentUID
                     << "doesn't exists in items.";
             return; // canceling addition
         }
     }

     QnlyGraphicsView* view = NULL;

     if (!regionbaseUID.isEmpty() && views.contains(regionbaseUID))
     {
         view = views.value(regionbaseUID);
     }
     else
     {
         qDebug() << "QnlyView::addRegion regionbaseUID=" << regionbaseUID
                 << "doesn't exists in views.";
         return; // canceling addition
     }

     if (!attributes["id"].isEmpty())
         item->setId(attributes["id"]);

     if (!attributes["title"].isEmpty())
         item->setTitle(attributes["title"]);

     if (!attributes["color"].isEmpty())
         item->setColor(attributes["color"]);
     else
         item->setColor("#e4e4e4"); //default value

     if (!attributes["top"].isEmpty())
     {
         if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?%")))
         {
             QString attribute = attributes["top"];
             attribute.remove(attribute.length()-1,1); // removing '%'

             qreal top = attribute.toDouble();

             if (top >= 0 && top <= 100)
                 item->setRelativeTop(top/100);
             else
                 item->setRelativeTop(0);

         }
         else if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?")))
         {
             QString attribute = attributes["top"];

             qreal top = attribute.toDouble();

             if (top >= 0 && top <= 1)
                 item->setRelativeTop(top);
            else
                 item->setRelativeTop(0);
         }

     }
     else
     {
         item->setRelativeTop(0);
     }

     if (!attributes["left"].isEmpty()){
         if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?%"))){
             QString attribute = attributes["left"];
             attribute.remove(attribute.length()-1,1); // removing '%'

             qreal left = attribute.toDouble();

             if (left >= 0 && left <= 100){
                 item->setRelativeLeft(left/100);

             }else{
                 item->setRelativeLeft(0);
             }

         }else if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?"))){
             QString attribute = attributes["left"];

             qreal left = attribute.toDouble();

             if (left >= 0 && left <= 1){
                 item->setRelativeLeft(left);

             }else{
                 item->setRelativeLeft(0);
             }
         }

     }else{
         item->setRelativeLeft(0);
     }

     if (!attributes["right"].isEmpty()){
         if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?%"))){
             QString attribute = attributes["right"];
             attribute.remove(attribute.length()-1,1); // removing '%'

             qreal right = attribute.toDouble();

             if (right >= 0 && right <= 100){
                 item->setRelativeRight(right/100);

             }else{
                 item->setRelativeRight(0);
             }

         }else if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?"))){
             QString attribute = attributes["right"];

             qreal right = attribute.toDouble();

             if (right >= 0 && right <= 1){
                 item->setRelativeRight(right);

             }else{
                 item->setRelativeRight(0);
             }
         }

     }else{
         item->setRelativeRight(0);
     }

     if (!attributes["bottom"].isEmpty()){
         if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?%"))){
             QString attribute = attributes["bottom"];
             attribute.remove(attribute.length()-1,1); // removing '%'

             qreal bottom = attribute.toDouble();

             if (bottom >= 0 && bottom <= 100){
                 item->setRelativeBottom(bottom/100);

             }else{
                 item->setRelativeBottom(0);
             }

         }else if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?"))){
             QString attribute = attributes["bottom"];

             qreal bottom = attribute.toDouble();

             if (bottom >= 0 && bottom <= 1){
                 item->setRelativeBottom(bottom);

             }else{
                 item->setRelativeBottom(0);
             }
         }

     }
     else {
         item->setRelativeBottom(0);
     }

     if (!attributes["width"].isEmpty()){
         if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?%"))){
             QString attribute = attributes["width"];
             attribute.remove(attribute.length()-1,1); // removing '%'

             qreal width = attribute.toDouble();

             if (width >= 0 && width <= 100){
                 item->setRelativeWidth(width/100);

             }else{
                 item->setRelativeWidth(1);
             }

         }else if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?"))){
             QString attribute = attributes["width"];

             qreal width = attribute.toDouble();

             if (width >= 0 && width <= 1){
                 item->setRelativeWidth(width);

             }else{
                 item->setRelativeWidth(1);
             }
         }

     }else{
         item->setRelativeWidth(1);
     }

     if (!attributes["height"].isEmpty()){
         if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?%"))){
             QString attribute = attributes["height"];
             attribute.remove(attribute.length()-1,1); // removing '%'

             qreal height = attribute.toDouble();

             if (height >= 0 && height <= 100){
                 item->setRelativeHeight(height/100);

             }else{
                 item->setRelativeHeight(1);
             }

         }else if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?"))){
             QString attribute = attributes["height"];

             qreal height = attribute.toDouble();

             if (height >= 0 && height <= 1){
                 item->setRelativeHeight(height);

             }else{
                 item->setRelativeHeight(1);
             }
         }

     }else{
         item->setRelativeHeight(1);
     }


     qDebug() << "QnlyView::addRegion calling addItem";
     addItem(item, parent, view);
}

void QnlyView::removeRegion(const QString regionUID, const QString regionbaseUID)
{

}

void QnlyView::changeRegion(const QString regionUID, const QString regionbaseUID, const QMap<QString, QString> &attributes)
{

}

void QnlyView::addRegionbase(const QString regionbaseUID, const QMap<QString, QString> &attributes)
{
    QnlyGraphicsView* view = NULL;

    if (!regionbaseUID.isEmpty()){
        if (!views.contains(regionbaseUID)){
            view = new QnlyGraphicsView(this);
        }else{
            return; // canceling addition
        }
    }else{
        return; // canceling addition
    }

    view->setUid(regionbaseUID);

    if (!attributes["id"].isEmpty()){
        view->setId(attributes["id"]);

    }else{
        // no default value
    }

    if (!attributes["device"].isEmpty()){
        view->setDevice(attributes["device"]);

    }else{
        // no default value
    }

    if (!attributes["region"].isEmpty()){
        view->setRegion(attributes["region"]);

    }else{
        // no default value
    }

    addView(view);
}

void QnlyView::removeRegionbase(const QString regionbaseUID)
{

}

void QnlyView::changeRegionbase(const QString regionbaseUID, const QMap<QString, QString> &attributes)
{

}

void QnlyView::contextMenuEvent(QContextMenuEvent *event)
{
    QStackedWidget::contextMenuEvent(event);

    if (!event->isAccepted()){
        helpAction->setEnabled(true);

        undoAction->setEnabled(false);
        redoAction->setEnabled(false);

        cutAction->setEnabled(false);
        copyAction->setEnabled(false);
        pasteAction->setEnabled(false);

        deleteAction->setEnabled(false);

        exportAction->setEnabled(false);

        viewMenu->setEnabled(false);
        zoominAction->setEnabled(false);
        zoomoutAction->setEnabled(false);
        zoomresetAction->setEnabled(false);
        fullscreenAction->setEnabled(false);

        insertMenu->setEnabled(true);
        itemAction->setEnabled(false);
        viewAction->setEnabled(true);

        showMenu->setEnabled(false);

        arrangeMenu->setEnabled(false);
        bringfrontAction->setEnabled(false);
        bringforwardAction->setEnabled(false);
        sendbackAction->setEnabled(false);
        sendbackwardAction->setEnabled(false);

        hideAction->setEnabled(false);

        switchMenu->setEnabled(false);

        propertiesAction->setEnabled(false);

        contextMenu->exec(event->globalPos());

        event->accept();
    }
}

void QnlyView::keyPressEvent(QKeyEvent *event)
{
    QStackedWidget::keyPressEvent(event);

    if (!event->isAccepted()){

        event->accept();
    }
}
