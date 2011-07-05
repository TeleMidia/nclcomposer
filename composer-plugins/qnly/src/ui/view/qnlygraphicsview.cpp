#include "qnlygraphicsview.h"

QnlyGraphicsView::QnlyGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    createScene();
    createConnections();
}

QnlyGraphicsView::~QnlyGraphicsView()
{
}

QString QnlyGraphicsView::getId() const
{
    return id;
}

void QnlyGraphicsView::setId(const QString &id)
{
    this->id = id;
}

QString QnlyGraphicsView::getUid() const
{
    return uid;
}

void QnlyGraphicsView::setUid(const QString &uid)
{
    this->uid = uid;
}

QString QnlyGraphicsView::getRegion() const
{
    return region;
}

void QnlyGraphicsView::setRegion(const QString &region)
{
    this->region = region;
}

QString QnlyGraphicsView::getDevice() const
{
    return device;
}

void QnlyGraphicsView::setDevice(const QString &device)
{
    this->device = device;
}

void QnlyGraphicsView::createScene()
{
    scene = new QnlyGraphicsScene(this);

    setScene(scene);
}

void QnlyGraphicsView::createConnections()
{
    connect(scene, SIGNAL(fullscreenPerformed(bool)), SIGNAL(fullscreenPerformed(bool)));

    connect(scene, SIGNAL(itemAdded(QnlyGraphicsItem*,QnlyGraphicsItem*)), SLOT(addItem(QnlyGraphicsItem*,QnlyGraphicsItem*)));
    connect(scene, SIGNAL(itemRemoved(QnlyGraphicsItem*)), SLOT(removeItem(QnlyGraphicsItem*)));
    connect(scene, SIGNAL(itemSelected(QnlyGraphicsItem*)), SLOT(selectItem(QnlyGraphicsItem*)));
    connect(scene, SIGNAL(itemChanged(QnlyGraphicsItem*,QMap<QString,QString>)), SLOT(changeItem(QnlyGraphicsItem*,QMap<QString,QString>)));
}

void QnlyGraphicsView::addItem(QnlyGraphicsItem* item, QnlyGraphicsItem* parent)
{
    if (item != NULL){
        if (!item->isPainted()){
            scene->addItem(item, parent);
        }

        emit itemAdded(item, parent, this);
    }
}

void QnlyGraphicsView::removeItem(QnlyGraphicsItem* item)
{
    if (item != NULL){
        if (!item->isPainted()){
            scene->removeItem(item);
        }

        emit itemRemoved(item, this);
    }
}

void QnlyGraphicsView::selectItem(QnlyGraphicsItem* item)
{
    if (item != NULL){
        if (!item->isSelected()){
            scene->selectItem(item);
        }

        emit itemSelected(item, this);
    }
}

void QnlyGraphicsView::changeItem(QnlyGraphicsItem* item, const QMap<QString, QString> &attributes)
{
    qDebug() << "QnlyGraphicsView::changeItem";
    if (item != NULL){
//        if (!item->hasChanged()){
//            scene->changeItem(item, attributes);
//        }

        emit itemChanged(item, this, attributes);
    }
}

void QnlyGraphicsView::resizeEvent(QResizeEvent* event)
{
    /* resizing view */
    QGraphicsView::resizeEvent(event);

    /* resizing scene */
    scene->setSceneRect(0,0,event->size().width(), event->size().height());

    /* adjusting items */
    for (int i = 0; i < scene->items().size(); ++i){
        QnlyGraphicsItem* item = (QnlyGraphicsItem*) scene->items().at(i);
        item->adjust();
    }
}


