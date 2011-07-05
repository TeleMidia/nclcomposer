#include "qnlygraphicsitem.h"

#include <QDebug>

QnlyGraphicsItem::QnlyGraphicsItem(QnlyGraphicsItem* parent) : QObject(parent), QGraphicsItem(parent)
{
    /* creating */
    createActions();
    createMenus();
    createConnections();

    /* setting defaults */
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    setMoving(false);
    setResizing(false);
    setSelected(false);

    setValidated(true);

    setId("");
    setUid("");
    setTitle("");

    setRelativeTop(0);
    setRelativeLeft(0);
    setRelativeRight(0);
    setRelativeBottom(0);

    setRelativeWidth(1);
    setRelativeHeight(1);

    setzIndex(-1);
    setzIndexTop(0);

    setColor("#E4E4E4");
}

QnlyGraphicsItem::~QnlyGraphicsItem()
{
    delete(viewMenu);
    delete(insertMenu);
    delete(arrangeMenu);
    delete(contextMenu);
}

bool QnlyGraphicsItem::isMoving() const
{
    return moving;
}

void QnlyGraphicsItem::setMoving(const bool &moving)
{
    this->moving = moving;
}

bool QnlyGraphicsItem::isResizing() const
{
    return resizing;
}

void QnlyGraphicsItem::setResizing(const bool &resizing)
{
    this->resizing = resizing;
}

bool QnlyGraphicsItem::isSelected() const
{
    return selected;
}

void QnlyGraphicsItem::setSelected(const bool &selected)
{
    this->selected = selected;

    setFocus(Qt::MouseFocusReason);
}

bool QnlyGraphicsItem::isValidated() const
{
    return validated;
}

void QnlyGraphicsItem::setValidated(const bool &validated)
{
    this->validated = validated;
}

QString QnlyGraphicsItem::getId() const
{
    return id;
}

void QnlyGraphicsItem::setId(const QString &id)
{
    this->id = id;

    setToolTip(title+" "+"("+id+")");
}

QString QnlyGraphicsItem::getUid() const
{
    return uid;
}

void QnlyGraphicsItem::setUid(const QString &uid)
{
    this->uid = uid;
}

QString QnlyGraphicsItem::getTitle() const
{
    return title;
}

void QnlyGraphicsItem::setTitle(const QString &title)
{
    this->title = title;

    setToolTip(title+" "+"("+id+")");
}

QString QnlyGraphicsItem::getColor() const
{
    return color;
}

void QnlyGraphicsItem::setColor(const QString &color)
{
    this->color = color;
}

bool QnlyGraphicsItem::isPainted() const
{
    return painted;
}

void QnlyGraphicsItem::setPainted(const bool &painted)
{
    this->painted = painted;
}

void QnlyGraphicsItem::setzIndex(const qreal &zindex)
{
    this->zindex = zindex;

    setZValue(zindex);
}

qreal QnlyGraphicsItem::getzIndex() const
{
    return zindex;
}

void QnlyGraphicsItem::setzIndexTop(const qreal &zindexTop)
{
    this->zindexTop = zindexTop;
}

qreal QnlyGraphicsItem::getzIndexTop() const
{
    return zindexTop;
}

qreal QnlyGraphicsItem::getTop() const
{
    return top;
}

void QnlyGraphicsItem::setTop(const qreal &top)
{
    this->top = top;

    setY(top-4);
}

qreal QnlyGraphicsItem::getMoveTop() const
{
    return moveTop;
}

void QnlyGraphicsItem::setMoveTop(const qreal &moveTop)
{
    this->moveTop = moveTop;
}

qreal QnlyGraphicsItem::getPressTop() const
{
    return pressTop;
}

void QnlyGraphicsItem::setPressTop(const qreal &pressTop)
{
    this->pressTop = pressTop;
}

qreal QnlyGraphicsItem::getResizeTop() const
{
    return resizeTop;
}

void QnlyGraphicsItem::setResizeTop(const qreal &resizeTop)
{
    this->resizeTop = resizeTop;
}

qreal QnlyGraphicsItem::getRelativeTop() const
{
    return relativeTop;
}

void QnlyGraphicsItem::setRelativeTop(const qreal &relativeTop)
{
    this->relativeTop = relativeTop;
}

qreal QnlyGraphicsItem::getLeft() const
{
    return left;
}

void QnlyGraphicsItem::setLeft(const qreal &left)
{
    this->left = left;

    setX(left-4);
}

qreal QnlyGraphicsItem::getMoveLeft() const
{
    return moveLeft;
}

void QnlyGraphicsItem::setMoveLeft(const qreal &moveLeft)
{
    this->moveLeft = moveLeft;
}

qreal QnlyGraphicsItem::getPressLeft() const
{
    return pressLeft;
}

void QnlyGraphicsItem::setPressLeft(const qreal &pressLeft)
{
    this->pressLeft = pressLeft;
}

qreal QnlyGraphicsItem::getResizeLeft() const
{
    return resizeLeft;
}

void QnlyGraphicsItem::setResizeLeft(const qreal &resizeLeft)
{
    this->resizeLeft = resizeLeft;
}

qreal QnlyGraphicsItem::getRelativeLeft() const
{
    return relativeLeft;
}

void QnlyGraphicsItem::setRelativeLeft(const qreal &relativeLeft)
{
    this->relativeLeft = relativeLeft;
}

qreal QnlyGraphicsItem::getRight() const
{
    return right;
}

void QnlyGraphicsItem::setRight(const qreal &right)
{
    this->right = right;
}

qreal QnlyGraphicsItem::getRelativeRight() const
{
    return relativeRight;
}

void QnlyGraphicsItem::setRelativeRight(const qreal &relativeRight)
{
    this->relativeRight = relativeRight;
}

qreal QnlyGraphicsItem::getBottom() const
{
    return bottom;
}

void QnlyGraphicsItem::setBottom(const qreal &bottom)
{
    this->bottom = bottom;
}

qreal QnlyGraphicsItem::getRelativeBottom() const
{
    return relativeBottom;
}

void QnlyGraphicsItem::setRelativeBottom(const qreal &relativeBottom)
{
    this->relativeBottom = relativeBottom;
}

qreal QnlyGraphicsItem::getWidth() const
{
    return width;
}

void QnlyGraphicsItem::setWidth(const qreal &width)
{
    this->width = width;
}

qreal QnlyGraphicsItem::getPressWidth() const
{
    return pressWidth;
}

void QnlyGraphicsItem::setPressWidth(const qreal &pressWidth)
{
    this->pressWidth = pressWidth;
}

qreal QnlyGraphicsItem::getResizeWidth() const
{
    return resizeWidth;
}

bool QnlyGraphicsItem::hasChanged() const
{
    return changed;
}

void QnlyGraphicsItem::setChanged(const bool &changed)
{
    this->changed = changed;
}

void QnlyGraphicsItem::setResizeWidth(const qreal &resizeWidth)
{
    this->resizeWidth = resizeWidth;
}

qreal QnlyGraphicsItem::getRelativeWidth() const
{
    return relativeWidth;
}

void QnlyGraphicsItem::setRelativeWidth(const qreal &relativeWidth)
{
    this->relativeWidth = relativeWidth;
}

qreal QnlyGraphicsItem::getHeight() const
{
    return height;
}

void QnlyGraphicsItem::setHeight(const qreal &height)
{
    this->height = height;
}

qreal QnlyGraphicsItem::getPressHeight() const
{
    return pressHeight;
}

void QnlyGraphicsItem::setPressHeight(const qreal &pressHeight)
{
    this->pressHeight = pressHeight;
}

qreal QnlyGraphicsItem::getResizeHeight() const
{
    return resizeHeight;
}

void QnlyGraphicsItem::setResizeHeight(const qreal &resizeHeight)
{
    this->resizeHeight = resizeHeight;
}

qreal QnlyGraphicsItem::getRelativeHeight() const
{
   return relativeHeight;
}

void QnlyGraphicsItem::setRelativeHeight(const qreal &relativeHeight)
{
     this->relativeHeight = relativeHeight;
}

QnlyGraphicsItem::QnlyResizeType QnlyGraphicsItem::getResizeType() const
{
    return resizeType;
}

void QnlyGraphicsItem::setResizeType(const QnlyResizeType &resizeType)
{
    this->resizeType = resizeType;
}

void QnlyGraphicsItem::createActions()
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
    deleteAction->setText(tr("Delete"));
    deleteAction->setShortcut(QKeySequence("Del"));

    /* zoomin action */
    zoomInAction = new QAction(this);
    zoomInAction->setText(tr("Zoom In"));
    zoomInAction->setShortcut(QKeySequence("Ctrl++"));

    /* zoomout action */
    zoomOutAction = new QAction(this);
    zoomOutAction->setText(tr("Zoom Out"));
    zoomOutAction->setShortcut(QKeySequence("Ctrl+-"));

    /* reset action */
    zoomResetAction = new QAction(this);
    zoomResetAction->setText(tr("Reset"));
    zoomResetAction->setShortcut(QKeySequence("Ctrl+0"));

    /* hide action */
    hideAction = new QAction(this);
    hideAction->setText(tr("Hide"));
    hideAction->setShortcut(QKeySequence("Ctrl+H"));

    /* fullscreen action */
    fullscreenAction = new QAction(this);
    fullscreenAction->setCheckable(true);
    fullscreenAction->setText(tr("Full Screen"));
    fullscreenAction->setShortcut(QKeySequence("Ctrl+F"));

    /* export action */
    exportAction = new QAction(this);
    exportAction->setText(tr("Export..."));

    /* region action */
    itemAction = new QAction(this);
    itemAction->setText(tr("Region"));

    /* regionBase action */
    viewAction = new QAction(this);
    viewAction->setText(tr("Region Base"));

    /* bring to front action */
    frontAction = new QAction(this);
    frontAction->setText(tr("Bring to Front"));
    frontAction->setShortcutContext(Qt::ApplicationShortcut);
    frontAction->setShortcut(QKeySequence("Shift+Ctrl+]"));


    /* bring forward action */
    forwardAction = new QAction(this);
    forwardAction->setText(tr("Bring Forward"));
    forwardAction->setShortcut(QKeySequence("Ctrl+]"));

    /* send backward action */
    backwardAction = new QAction(this);
    backwardAction->setText(tr("Send Backward"));
    backwardAction->setShortcut(QKeySequence("Ctrl+["));

    /* send to back action */
    backAction = new QAction(this);
    backAction->setText(tr("Send to Back"));
    backAction->setShortcut(QKeySequence("Shift+Ctrl+["));

    /* properties action */
    propertiesAction = new QAction(this);
    propertiesAction->setText(tr("Properties"));
}

void QnlyGraphicsItem::createMenus()
{
    /* view menu */
    viewMenu = new QMenu();
    viewMenu->setTitle(tr("View"));
    viewMenu->addAction(zoomInAction);
    viewMenu->addAction(zoomOutAction);
    viewMenu->addAction(zoomResetAction);
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
    arrangeMenu->addAction(frontAction);
    arrangeMenu->addAction(forwardAction);
    arrangeMenu->addAction(backwardAction);
    arrangeMenu->addAction(backAction);

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

void QnlyGraphicsItem::createConnections()
{
    connect(helpAction, SIGNAL(triggered()), SLOT(performHelp()));
    connect(undoAction, SIGNAL(triggered()), SLOT(performUndo()));
    connect(redoAction, SIGNAL(triggered()), SLOT(performRedo()));
    connect(cutAction, SIGNAL(triggered()), SLOT(performCut()));
    connect(copyAction, SIGNAL(triggered()), SLOT(performCopy()));
    connect(pasteAction, SIGNAL(triggered()), SLOT(performPaste()));
    connect(deleteAction, SIGNAL(triggered()), SLOT(performDelete()));
    connect(exportAction, SIGNAL(triggered()), SLOT(performExport()));
    connect(zoomInAction, SIGNAL(triggered()), SLOT(performZoomIn()));
    connect(zoomOutAction, SIGNAL(triggered()), SLOT(performZoomOut()));
    connect(zoomResetAction, SIGNAL(triggered()), SLOT(performZoomReset()));
    connect(fullscreenAction, SIGNAL(triggered(bool)), SLOT(performFullscreen(bool)));
    connect(itemAction, SIGNAL(triggered()), SLOT(performRegion()));
    connect(viewAction, SIGNAL(triggered()), SLOT(performRegionBase()));
    connect(frontAction, SIGNAL(triggered()), SLOT(performFront()));
    connect(forwardAction, SIGNAL(triggered()), SLOT(performForward()));
    connect(backwardAction, SIGNAL(triggered()), SLOT(performBackward()));
    connect(backAction, SIGNAL(triggered()), SLOT(performBack()));
    connect(hideAction, SIGNAL(triggered()), SLOT(performHide()));
    connect(propertiesAction, SIGNAL(triggered()), SLOT(performProperties()));
}

void QnlyGraphicsItem::performHelp()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit helpPerformed();
}

void QnlyGraphicsItem::performUndo()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit undoPerformed();
}

void QnlyGraphicsItem::performRedo()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit redoPerformed();
}

void QnlyGraphicsItem::performCut()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit cutPerformed();
}

void QnlyGraphicsItem::performCopy()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit copyPerformed();
}

void QnlyGraphicsItem::performPaste()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit pastePerformed();
}

void QnlyGraphicsItem::performDelete()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit itemRemoved(this);

    emit deletePerformed();
}

void QnlyGraphicsItem::performExport()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit exportPerformed();
}

void QnlyGraphicsItem::performZoomIn()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit zoomInPerformed();
}

void QnlyGraphicsItem::performZoomOut()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit zoomOutPerformed();
}

void QnlyGraphicsItem::performZoomReset()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit zoomResetPerformed();
}

void QnlyGraphicsItem::performFullscreen(bool checked)
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit fullscreenPerformed();
}

void QnlyGraphicsItem::performRegion()
{
    /* performing */
    QnlyGraphicsItem* item = new QnlyGraphicsItem(this);
    item->setRelativeTop(0.1);
    item->setRelativeLeft(0.1);
    item->setRelativeRight(0.1);
    item->setRelativeBottom(0.1);
    item->setRelativeWidth(0.8);
    item->setRelativeHeight(0.8);
    item->setzIndex(zindexTop+1);
    item->setPainted(true);

    setzIndexTop(zindexTop+1);

    item->adjust();

    /* emitting signals */
    emit itemAdded(item, this);
    emit itemSelected(item);

    QAction *showAction = new QAction(this);

    showAction->setCheckable(true);
    showAction->setChecked(true);
    showAction->setText(item->getTitle()+" "+"("+item->getId()+")");

    showMenu->addAction(showAction);

    connect(showAction,SIGNAL(triggered(bool)),item, SLOT(hide(bool)));

    emit regionPerformed();
}

void QnlyGraphicsItem::performRegionBase()
{
    /* performing */

    /* emitting signals */
    emit regionBasePerformed();
}

void QnlyGraphicsItem::performFront()
{
    qDebug() << "oi";
    /* performing */
    int index = -1;

    int quantity = parentItem()->childItems().size();

    QVector<QnlyGraphicsItem*> zitems(quantity+1);

    for (int i = 0; i < quantity; ++i){
        QnlyGraphicsItem* item = (QnlyGraphicsItem*) parentItem()->childItems().at(i);


        qDebug() << item->getzIndex();

        zitems[item->getzIndex()] =  item;
    }

    for (int i = zindex; i <= quantity-1; ++i){
        QnlyGraphicsItem* item = zitems.at(i);
        QnlyGraphicsItem* nextItem = zitems.at(i+1);

        qreal zindexTmp = nextItem->getzIndex();
        nextItem->setzIndex(item->getzIndex());
        item->setzIndex(zindexTmp);

        qSwap(zitems[i],zitems[i+1]);
    }

    adjust();

    /* emitting signals */

    emit frontPerformed();
}

void QnlyGraphicsItem::performForward()
{
    /* performing */

    // nothing to do

    /* emitting signals */

    emit forwardPerformed();
}

void QnlyGraphicsItem::performBack()
{
    /* performing */

    // nothing to do

    /* emitting signals */

    emit backPerformed();
}

void QnlyGraphicsItem::performBackward()
{
    /* performing */

    // nothing to do

    /* emitting signals */

    emit backwardPerformed();
}

void QnlyGraphicsItem::performHide()
{
    /* performing */

    hide(false);

    /* emitting signals */
//    emit switchPerformed();
}

void QnlyGraphicsItem::performSwitch()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit switchPerformed();
}

void QnlyGraphicsItem::performProperties()
{
    /* performing */

    // nothing to do

    /* emitting signals */
    emit propertiesPerformed();
}

void QnlyGraphicsItem::move(QGraphicsSceneMouseEvent* event)
{
    /* setting */
    qreal x = left;
    qreal y = top;

    /* setting minimal position */
    qreal minx;
    qreal miny;

    if (parentItem() != NULL){
        minx = 4;
        miny = 4;
    }else{
        minx = 0;
        miny = 0;
    }

    /* setting maximal position */
    qreal maxx;
    qreal maxy;

    if (parentItem() != NULL){
        maxx = parentItem()->boundingRect().width() - width - 4;
        maxy = parentItem()->boundingRect().height() - height - 4;
    }else{
        maxx = scene()->width() - width;
        maxy = scene()->height() - height;
    }

    /* setting delta */
    qreal dx = event->pos().x() - pressLeft; // (x1 - x0)
    qreal dy = event->pos().y() - pressTop;  // (y1 - y0)

    /* setting next position */
    qreal nextx = x + dx;
    qreal nexty = y + dy;

    /* adjusting */
    if (nextx < minx){
        nextx = minx;
    }

    if (nexty < miny){
        nexty = miny;
    }

    if (nextx > maxx){
        nextx = maxx;
    }

    if (nexty > maxy){
        nexty = maxy;
    }

    /* moving */
    setMoveTop(nexty);
    setMoveLeft(nextx);

    scene()->update();
}

void QnlyGraphicsItem::resize(QGraphicsSceneMouseEvent* event)
{
    /* setting bounds */
    qreal x = left;
    qreal y = top;
    qreal w = width;
    qreal h = height;

    /* setting minimal bounds */
    qreal minx;
    qreal miny;
    qreal minw;
    qreal minh;

    if (parentItem() != NULL){
        minx = 4;
        miny = 4;
        minw = -1; // not used
        minh = -1; // not used
    }else{
        minx = 0;
        miny = 0;
        minw = -1; // not used
        minh = -1; // not used
    }

    /* setting maximal bounds */
    qreal maxx;
    qreal maxy;
    qreal maxw;
    qreal maxh;

    if (parentItem() != NULL){
        maxx = parentItem()->boundingRect().width() - width - 4;
        maxy = parentItem()->boundingRect().height() - height - 4;
        maxw = parentItem()->boundingRect().width() - 4;
        maxh = parentItem()->boundingRect().height() - 4;
    }else{
        maxx = scene()->width() - width;
        maxy = scene()->height() - height;
        maxw = scene()->width();
        maxh = scene()->height();
    }

    /* setting delta */
    qreal dx = event->pos().x() - pressLeft;    // (x1 - x0)
    qreal dy = event->pos().y() - pressTop;     // (y1 - y0)
    qreal dw = -dx;
    qreal dh = -dy;

    /* setting next bounds */
    qreal nextx = x + dx;
    qreal nexty = y + dy;
    qreal nextw = w + dw;
    qreal nexth = h + dh;

    /* adjusting */
    switch(resizeType){

    /* adjusting TOPLEFT */
    case QnlyGraphicsItem::TopLeft:{
        if (nextx < minx){
            nextx = minx;
            nextw = x + w - minx;
        }

        if (nexty < miny){
            nexty = miny;
            nexth = y + h - miny;
        }

        break;
    }

    /* adjusting TOP */
    case QnlyGraphicsItem::Top:{
        nextx = x; // fixed x
        nextw = w; // fixed width

        if (nexty < miny){
            nexty = miny;
            nexth = y + h - miny;
        }

        break;
    }

    /* adjusting TOPRIGHT */
    case QnlyGraphicsItem::TopRight:{
        nextx = x; // fixed x

        nextw = w - dw;
        if (x + nextw > maxw){
            nextw = maxw - x;
        }

        if (nexty < miny){
            nexty = miny;
            nexth = y + h - miny;
        }

        break;
    }

    /* adjusting RIGHT */
    case QnlyGraphicsItem::Right:{
        nextx = x; // fixed x

        nextw = w - dw;
        if (x + nextw > maxw){
            nextw = maxw - x;
        }

        nexty = y; // fixed y
        nexth = h; // fixed height

        break;
    }

    /* adjusting BOTTOMRIGHT */
    case QnlyGraphicsItem::BottomRight:{
        nextx = x; // fixed x

        nextw = w - dw;
        if (x + nextw > maxw){
            nextw = maxw - x;
        }

        nexty = y; // fixed y

        nexth = h - dh;
        if (y + nexth > maxh){
            nexth = maxh - y;
        }

        break;
    }

    /* adjusting BOTTOM */
    case QnlyGraphicsItem::Bottom:{
        nextx = x; // fixed x
        nextw = w; // fixed width

        nexty = y; // fixed y

        nexth = h - dh;
        if (y + nexth > maxh){
            nexth = maxh - y;
        }

        break;
    }

    /* adjusting BOTTOMLEFT */
    case QnlyGraphicsItem::BottomLeft:{
        if (nextx < minx){
            nextx = minx;
            nextw = x + w - minx;
        }

        nexty = y; // fixed y

        nexth = h - dh;
        if (y + nexth > maxh){
            nexth = maxh - y;
        }

        break;
    }

    /* adjusting LEFT */
    case QnlyGraphicsItem::Left:{
        if (nextx < minx){
            nextx = minx;
            nextw = x + w - minx;
        }

        nexty = y; // fixed y
        nexth = h; // fixed height

        break;
    }
    }

    /* resizing */
    setResizeTop(nexty);
    setResizeLeft(nextx);
    setResizeWidth(nextw);
    setResizeHeight(nexth);

    scene()->update();
}

void QnlyGraphicsItem::adjust(bool repaint)
{
    if (parentItem() != NULL){
        QnlyGraphicsItem* item = (QnlyGraphicsItem*) parentItem();

        setTop(qRound(item->getHeight()*relativeTop + 4));
        setLeft(qRound(item->getWidth()*relativeLeft + 4));

        setWidth(qRound(item->getWidth()*relativeWidth));
        setHeight(qRound(item->getHeight()*relativeHeight));

        setRight(item->getWidth() - (left+width));      // not use to paint
        setBottom(item->getHeight() - (top+height));    // not use to paint

    }else{
        setTop(qRound(scene()->height()*relativeTop));
        setLeft(qRound(scene()->width()*relativeLeft));

        setWidth(qRound(scene()->width()*relativeWidth));
        setHeight(qRound(scene()->height()*relativeHeight));

        setRight(scene()->width() - (left+width));      // not use to paint
        setBottom(scene()->height() - (top+height));    // not use to paint
    }

    for (int i=0;i<childItems().size();++i){
        QnlyGraphicsItem* item = (QnlyGraphicsItem*) childItems().at(i);

        item->adjust(false);
    }

    if (repaint){
        scene()->update();
    }
}

QPainterPath QnlyGraphicsItem::shape() const
{
    QPainterPath path;

    path.addRect(4, 4, width, height);

    if (selected){
        path.setFillRule(Qt::WindingFill);

        path.addRect(0,0,8,8);                                  // topleft
        path.addRect((width+8)/2 - 4,0,8,8);                    // top
        path.addRect((width+8) - 8,0,8,8);                      // topright
        path.addRect((width+8) - 8,(height+8)/2 - 4,8,8);       // right
        path.addRect((width+8) - 8,(height+8) - 8,8,8);         // bottomright
        path.addRect((width+8)/2 - 4,(height+8) - 8,8,8);       // bottom
        path.addRect(0,(height+8) - 8,8,8);                     // bottomleft
        path.addRect(0,(height+8)/2 - 4,8,8);                   // left
    }

    return path;
}

QRectF QnlyGraphicsItem::boundingRect() const
{
    QRectF bounds;

    bounds.setX(0);
    bounds.setY(0);
    bounds.setWidth(width+8);
    bounds.setHeight(height+8);

    return bounds;
}

void QnlyGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (selected){
        painter->setBrush(QColor(color));
        painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));  // 0px = cosmetic border
        painter->drawRect(4,4,width-1,height-1);

    }else{
        painter->setBrush(QColor(color));
        painter->setPen(QPen(QBrush(Qt::black), 0));                // 0px = cosmetic border
        painter->drawRect(4,4,width-1,height-1);
    }

    if (moving){
        if (parentItem() != NULL){
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(Qt::black), 0));            // 0px = cosmetic border
            painter->drawRect(moveLeft+4-left,moveTop+4-top,width-1,height-1);
        }else{
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(Qt::black), 0));            // 0px = cosmetic border
            painter->drawRect(moveLeft+4-left,moveTop+4-top,width-1,height-1);
        }

    }else if (resizing){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0));                // 0px = cosmetic border
        painter->drawRect(resizeLeft+4-left,resizeTop+4-top,resizeWidth-1,resizeHeight-1);

    }else if (selected){
        painter->setBrush(QBrush(Qt::white));
        painter->setPen(QPen(QBrush(QBrush(Qt::black)), 0));        // 0px = cosmetic border

        painter->drawRect(0,0,8,8);                                 // topleft
        painter->drawRect((width+8)/2-4-1,0,8,8);                   // top
        painter->drawRect((width+8)-8-1,0,8,8);                     // topright
        painter->drawRect((width+8)-8-1,(height+8)/2-4-1,8,8);      // right
        painter->drawRect((width+8)-8-1,(height+8)-8-1,8,8);        // bottomright
        painter->drawRect((width+8)/2-4-1,(height+8)-8-1,8,8);      // bottom
        painter->drawRect(0,(height+8)-8-1,8,8);                    // bottomleft
        painter->drawRect(0,(height+8)/2-4-1,8,8);                  // left
    }

    QString text = "";

    if (width >= 25 && height >= 25 && id != ""){
        text = title+" "+"("+id+")";
    }

    painter->drawText(4+6,4+6,width-1-4-6,height-1-4-6,Qt::AlignLeft, text);
}

void QnlyGraphicsItem::hide(bool v)
{
    setVisible(v);
}

void QnlyGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (moving){
        move(event);

    }else if (resizing){
        resize(event);
    }
}

void QnlyGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton){
        event->ignore();

    }else if (event->button() == Qt::LeftButton){
        emit itemSelected(this);

        setPressTop(event->pos().y());
        setPressLeft(event->pos().x());
        setPressWidth(width);
        setPressHeight(height);

        /* avoid flickering on first move */
        setMoveTop(top);
        setMoveLeft(left);

        /* avoid flickering on first resize */
        setResizeTop(top);
        setResizeLeft(left);
        setResizeWidth(width);
        setResizeHeight(height);

        /* if over TOPLEFT resize region */
        if (QRectF(0,0,8,8).contains(event->pos())){
            setResizeType(QnlyGraphicsItem::TopLeft);
            setResizing(true);

        /* if over TOP resize region */
        }else if (QRectF((width+8)/2 - 4,0,8,8).contains(event->pos())){
            setResizeType(QnlyGraphicsItem::Top);
            setResizing(true);

        /* if over TOPRIGHT resize region */
        }else if (QRectF((width+8) - 8,0,8,8).contains(event->pos())){
            setResizeType(QnlyGraphicsItem::TopRight);
            setResizing(true);

        /* if over RIGHT resize region */
        }else if (QRectF((width+8) - 8,(height+8)/2 - 4,8,8).contains(event->pos())){
            setResizeType(QnlyGraphicsItem::Right);
            setResizing(true);

        /* if over BOTTOMRIGHT resize region */
        }else if (QRectF((width+8) - 8,(height+8) - 8,8,8).contains(event->pos())){
            setResizeType(QnlyGraphicsItem::BottomRight);
            setResizing(true);

        /* if over BOTTOM resize region */
        }else if (QRectF((width+8)/2 - 4,(height+8) - 8,8,8).contains(event->pos())){
            setResizeType(QnlyGraphicsItem::Bottom);
            setResizing(true);

        /* if over BOTTOMLEFT resize region */
        }else if (QRectF(0,(height+8) - 8,8,8).contains(event->pos())){
            setResizeType(QnlyGraphicsItem::BottomLeft);
            setResizing(true);

        /* if over LEFT resize region */
        }else if (QRectF(0,(height+8)/2 - 4,8,8).contains(event->pos())){
            setResizeType(QnlyGraphicsItem::Left);
            setResizing(true);

        /* if not over any resize region */
        }else{
            setMoving(true);
        }
    }

    QGraphicsItem::mousePressEvent(event);
}

void QnlyGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (moving){
        setMoving(false);

        if ((top != moveTop || left != moveLeft)){

            setTop(moveTop);
            setLeft(moveLeft);

            if (parentItem() != NULL){
                QnlyGraphicsItem* item = (QnlyGraphicsItem*) parentItem();

                setRelativeTop((top-4)/item->getHeight());
                setRelativeLeft((left-4)/item->getWidth());

                setRelativeWidth(width/item->getWidth());
                setRelativeHeight(height/item->getHeight());

                setRelativeRight(1 - (relativeLeft+relativeWidth));
                setRelativeBottom(1 - (relativeTop+relativeHeight));

            }else{
                setRelativeTop(top/scene()->height());
                setRelativeLeft(left/scene()->width());

                setRelativeWidth(width/scene()->width());
                setRelativeHeight(height/scene()->height());

                setRelativeRight(1 - (relativeLeft+relativeWidth));
                setRelativeBottom(1 - (relativeTop+relativeHeight));
            }

             QMap<QString, QString> attributes;

             if (parentItem() != NULL){
                 QnlyGraphicsItem* item = (QnlyGraphicsItem*) parentItem();

                 attributes["top"] = QString::number(((moveTop-4)/item->getHeight())*100)+"%";
                 attributes["left"] = QString::number(((moveLeft-4)/item->getWidth())*100)+"%";

                 attributes["right"] = QString::number((1 - (((moveLeft-4)/item->getWidth())+(width/item->getWidth())))*100)+"%";
                 attributes["bottom"] = QString::number((1 - (((moveTop-4)/item->getHeight())+(height/item->getHeight())))*100)+"%";

             }else{
                 attributes["top"] = QString::number(((moveTop-4)/scene()->height())*100)+"%";
                 attributes["left"] = QString::number(((moveLeft-4)/width/scene()->width())*100)+"%";

                 attributes["right"] = QString::number((1 - (((moveLeft-4)/scene()->width())+(width/scene()->width())))*100)+"%";
                 attributes["bottom"] = QString::number((1 - (((moveTop-4)/scene()->height())+(height/scene()->height())))*100)+"%";
             }

            setChanged(true);

            emit itemChanged(this,attributes);
        }
    }

    if (resizing){
        setResizing(false);

        if ((top != resizeTop || left != resizeLeft || width != resizeWidth || height != resizeHeight)){
            if (resizeTop > top + height){
                setResizeHeight(resizeTop - (top + height));
                setResizeTop(top + height);
            }

            if (resizeLeft > left + width){
                setResizeWidth(resizeLeft - (left + width));
                setResizeLeft(left + width);
            }

            if (resizeWidth < 0){
                setResizeLeft(resizeLeft + resizeWidth);
                setResizeWidth(-resizeWidth);
            }

            if (resizeHeight < 0){
                setResizeTop(resizeTop + resizeHeight);
                setResizeHeight(-resizeHeight);
            }

            setTop(resizeTop);
            setLeft(resizeLeft);
            setWidth(resizeWidth);
            setHeight(resizeHeight);

            if (parentItem() != NULL){
                QnlyGraphicsItem* item = (QnlyGraphicsItem*) parentItem();

                setRelativeTop((top-4)/item->getHeight());
                setRelativeLeft((left-4)/item->getWidth());
                setRelativeWidth(width/item->getWidth());
                setRelativeHeight(height/item->getHeight());

                setRelativeRight(1 - (relativeLeft+relativeWidth));
                setRelativeBottom(1 - (relativeTop+relativeHeight));

            }else{
                setRelativeTop(top/scene()->height());
                setRelativeLeft(left/scene()->width());
                setRelativeWidth(width/scene()->width());
                setRelativeHeight(height/scene()->height());

                setRelativeRight(1 - (relativeLeft+relativeWidth));
                setRelativeBottom(1 - (relativeTop+relativeHeight));
            }

            QMap<QString, QString> attributes;

            if (parentItem() != NULL){
                QnlyGraphicsItem* item = (QnlyGraphicsItem*) parentItem();

                attributes["top"] = QString::number(((top-4)/item->getHeight())*100)+"%";
                attributes["left"] = QString::number(((left-4)/item->getWidth())*100)+"%";
                attributes["height"] = QString::number((height/item->getHeight())*100)+"%";
                attributes["width"] = QString::number((width/item->getWidth())*100)+"%";

                attributes["right"] = QString::number((1 - (((left-4)/item->getWidth())+(width/item->getWidth())))*100)+"%";
                attributes["bottom"] = QString::number((1 - (((top-4)/item->getHeight())+(height/item->getHeight())))*100)+"%";

            }else{
                attributes["top"] = QString::number(((top-4)/scene()->height())*100)+"%";
                attributes["left"] = QString::number(((left-4)/width/scene()->width())*100)+"%";
                attributes["height"] = QString::number((height/scene()->height())*100)+"%";
                attributes["width"] = QString::number((width/width/scene()->width())*100)+"%";

                attributes["right"] = QString::number((1 - (((left-4)/scene()->width())+(width/scene()->width())))*100)+"%";
                attributes["bottom"] = QString::number((1 - (((top-4)/scene()->height())+(height/scene()->height())))*100)+"%";
            }

            setChanged(true);

            emit itemChanged(this,attributes);
        }
    }

    adjust();

    QGraphicsItem::mouseReleaseEvent(event);
}

void QnlyGraphicsItem::keyPressEvent( QKeyEvent * event )
{
    QGraphicsItem::keyPressEvent(event);

    if (!event->isAccepted()){
        if (event->key() == Qt::Key_BracketRight && event->modifiers() == Qt::ControlModifier + Qt::ShiftModifier){
            performFront();
        }

        event->accept();
    }
}

void QnlyGraphicsItem::keyReleaseEvent( QKeyEvent * event )
{
    QGraphicsItem::keyReleaseEvent(event);
}


void QnlyGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QGraphicsItem::contextMenuEvent(event);

    if (!event->isAccepted()){
        emit itemSelected(this);

        contextMenu->exec(event->screenPos());

        event->accept();
    }
}
