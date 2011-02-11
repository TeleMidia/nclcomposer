#include "layoutitem.h"

#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsScene>
#include <QInputDialog>
#include <QGraphicsProxyWidget>

#include <QLineEdit>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

LayoutItem::LayoutItem(QGraphicsItem* parent)
    : AbstractItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
//    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setMoving(false);
    setResizing(false);
    setSelected(false);
    rindex = -1;

    title = "newregion";


    topleft  = false;
    createActions();
    createMenus();
    createConnections();
}

LayoutItem::~LayoutItem()
{
}

QSizeF LayoutItem::getSize() const
{
    return size;
}

void LayoutItem::setSize(const QSizeF size)
{
    this->size = size;
}

void LayoutItem::createMenus()
{
    //TODO: LayoutDocument::createMenus()

    contextMenu = new QMenu();

    insertMenu = new QMenu("insert", contextMenu);
    insertMenu->addAction(regionAction);

    contextMenu->addMenu(insertMenu);
}

void LayoutItem::createActions()
{
    //TODO: LayoutDocument::createActions()
    regionAction = new QAction(this);
    regionAction->setText("Region");
}

void LayoutItem::createConnections()
{
    //TODO: LayoutDocument::createConnections()
    connect(regionAction, SIGNAL(triggered()),SLOT(addItem()));
}


void LayoutItem::addItem()
{
    //TODO: LayoutItem::addItem(...)
    LayoutItem* item = new LayoutItem(this);
    item->setpWidth(0.25);
    item->setpHeight(0.25);
    item->setpLeft(insertPoint.x()/getSize().width());
    item->setpTop(insertPoint.y()/getSize().height());
    item->adjustItem();
}


void LayoutItem::addItem(AbstractItem* item)
{
    //TODO: LayoutItem::addItem(...)
}

void LayoutItem::removeItem(AbstractItem* item)
{
    //TODO: LayoutItem::removeItem(...)
}

QPainterPath LayoutItem::shape() const
{
    //TODO: LayoutItem::shape()
    QPainterPath path;

    path.addRect(0,0,size.width(),size.height());

    if (isSelected()){
        path.setFillRule(Qt::WindingFill);
        path.addRect(0,0,10,10);
    }

    return path;
}

bool LayoutItem::isMoving() const
{
    return moving;
}

void LayoutItem::resize(QGraphicsSceneMouseEvent* event)
{
    switch (rindex){
    case 0:{
            qreal cx = pos().x();
            qreal cy = pos().y();
            qreal nx = cx + (event->pos().x() - pressPoint.x());
            qreal ny = cy + (event->pos().y() - pressPoint.y());

            if (nx < 0){
                nx = 0;
            }

            if (ny < 0){
                ny = 0;
            }

            qreal pw = (size.width()+cx-nx)/size.width();
            qreal ph = (size.height()+cy-ny)/size.height();

            setPos(nx, ny);

            setSize(QSizeF(size.width()+cx-nx, size.height()+cy-ny));

            qDebug() << pw << ph;
            resize(this, pw,ph);

        }
    }

//    updatep();
}

void LayoutItem::resize(LayoutItem* parent, qreal pw, qreal ph)
{
//    if (pw > 0 && ph > 0){
        for (int i=0;i<parent->childItems().size();i++){
            LayoutItem* item = (LayoutItem*) parent->childItems().at(i);

            item->setPos(item->pos().x()*pw, item->pos().y()*ph);
            item->setSize(QSizeF(item->getSize().width()*pw,
                                 item->getSize().height()*ph));


//            item->updatep();

            resize(item, pw,ph);
        }
//    }
}

void LayoutItem::move(QGraphicsSceneMouseEvent* event)
{
    if (parentItem() != NULL){
        int cx = pos().x();
        int cy = pos().y();
        int nx = cx + (event->pos().x() - pressPoint.x());
        int ny = cy + (event->pos().y() - pressPoint.y());

        if (nx < 0){
            nx = 0;
        }

        if (nx + size.width() > parentItem()->boundingRect().width()){
            nx = parentItem()->boundingRect().width() - size.width();
        }


        if (ny < 0){
            ny = 0;
        }

        if (ny + size.height() > parentItem()->boundingRect().height()){
            ny = parentItem()->boundingRect().height() - size.height();
        }

        setPos(nx,ny);

    }else{
        int cx = pos().x();
        int cy = pos().y();
        int nx = cx + (event->pos().x() - pressPoint.x());
        int ny = cy + (event->pos().y() - pressPoint.y());

        if (nx < 0){
            nx = 0;
        }

        if (nx + size.width() > scene()->width()){
            nx = scene()->width() - size.width();
        }


        if (ny < 0){
            ny = 0;
        }

        if (ny + size.height() > scene()->height()){
            ny = scene()->height() - size.height();
        }

        setPos(nx,ny);
    }

//    updatep();
}

void LayoutItem::setMoving(bool moving)
{
    this->moving = moving;
}

qreal LayoutItem::getpTop() const
{
    return ptop;
}

void LayoutItem::setpTop(const qreal & ptop)
{
    this->ptop = ptop;
}

qreal LayoutItem::getpLeft() const
{
    return pleft;
}

void LayoutItem::setpLeft(const qreal & pleft)
{
    this->pleft = pleft;
}

qreal LayoutItem::getpWidth() const
{
    return pwidth;
}

void LayoutItem::setpWidth(const qreal & pwidth)
{
    this->pwidth = pwidth;
}

qreal LayoutItem::getpHeight() const
{
    return pheight;;
}

void LayoutItem::setpHeight(const qreal & pheight)
{
    this->pheight = pheight;
}

bool LayoutItem::isResizing() const
{
    return resizing;
}

void LayoutItem::setResizing(bool resizing)
{
    this->resizing = resizing;
}

QString LayoutItem::getTile() const
{
    return title;
}

void LayoutItem::setTitle(const QString &title)
{
    this->title = title;
}

QRectF LayoutItem::boundingRect() const
{
    //TODO: LayoutItem::boundingRect()
    return QRectF(0,0,size.width(),size.height());
}

bool LayoutItem::isSelected() const
{
    return selected;
}

void LayoutItem::setSelected(bool selected)
{
    this->selected = selected;
}

void LayoutItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )
{

//    setPos(0,0);

//    if (parentItem() != NULL){
//        LayoutItem* item = (LayoutItem*) parentItem();
//        setSize(QSizeF(item->getSize().width(),item->getSize().height()));
//    }else{
//        setSize(QSizeF(scene()->width(),scene()->height()));
//    }
//    title = QInputDialog::getText(NULL,"New Title", "Title");

//       update();

    title = QInputDialog::getText(NULL,"Title","Title");

    update();

//    QLineEdit* line = new QLineEdit();

//    QGraphicsProxyWidget *proxy = scene()->addWidget(line);
//    proxy->setPos(event->pos());
//    proxy->setParentItem(this);


    AbstractItem::mouseDoubleClickEvent(event);
}

void LayoutItem::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    //TODO: LayoutItem::paint(...)
    painter->setPen(QPen(QBrush(Qt::black), 3));
    painter->setBrush(QColor(198,211,238));
    painter->drawRect(1.5,1.5,size.width()-1.5,size.height()-1.5);

     painter->setBrush(Qt::blue);
    painter->drawText(QRectF(25,25,size.width()*0.8,size.height()*0.2),title);

    if (isSelected()){
        painter->setPen(Qt::NoPen);;
        painter->setBrush(Qt::black);;
        painter->drawRect(0,0,10,10);
    }
}

void LayoutItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (isResizing()){
        resize(event);
    }else if (isMoving()){
        move(event);
    }
}

void LayoutItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setSelected(true);
    qDebug() << title << "selected";

     update();

    if (event->button() == Qt::RightButton){
        event->ignore();
    }else if (event->button() == Qt::LeftButton){
        pressPoint = event->pos();
        if (QRectF(0,0,10,10).contains(event->pos())){
            rindex = 0;
            setResizing(true);
        }else{
            setMoving(true);
        }
    }

    AbstractItem::mousePressEvent(event);
}

void LayoutItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    setMoving(false);
    setResizing(false);

    updatep();

    adjustItem();

    AbstractItem::mouseReleaseEvent(event);
}

void LayoutItem::contextMenuEvent(
            QGraphicsSceneContextMenuEvent* contextMenuEvent)
{
    //TODO: LayoutDocument::contextMenuEvent(...)
    AbstractItem::contextMenuEvent(contextMenuEvent);


    if (!contextMenuEvent->isAccepted()){
        insertPoint = contextMenuEvent->pos();
        contextMenu->exec(contextMenuEvent->screenPos());
        contextMenuEvent->setAccepted(true);
    }

     qDebug() << "region";
}

void LayoutItem::adjustItem()
{
//    qDebug() << title << "adjustItem()";

    if (parentItem() != NULL){
        LayoutItem* item = (LayoutItem*) parentItem();

        setPos(item->getSize().width()*pleft,
               item->getSize().height()*ptop);

        setSize(QSizeF(item->getSize().width()*pwidth,
                       item->getSize().height()*pheight));
    }else{
        setPos(scene()->width()*pleft,
               scene()->height()*ptop);

        setSize(QSizeF(scene()->width()*pwidth,
                       scene()->height()*pheight));
    }

    adjustItem(this);
}

void LayoutItem::adjustItem(LayoutItem* parent)
{
    for (int i=0;i<parent->childItems().size();i++){
        LayoutItem* item = (LayoutItem*) parent->childItems().at(i);

        item->adjustItem();
    }
}

void LayoutItem::updatep()
{
    if (parentItem() != NULL){
        LayoutItem* item = (LayoutItem*) parentItem();

        setpLeft(pos().x()/item->getSize().width());
        setpTop(pos().y()/item->getSize().height());
        setpWidth(getSize().width()/item->getSize().width());
        setpHeight(getSize().height()/item->getSize().height());
    }else{
        setpLeft(pos().x()/scene()->width());
        setpTop(pos().y()/scene()->height());
        setpWidth(getSize().width()/scene()->width());
        setpHeight(getSize().height()/scene()->height());
    }
}



}
}
}
}
}
