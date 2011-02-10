#include "layoutview.h"

#include "layoutscene.h"

#include <QDebug>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

LayoutView::LayoutView(QWidget* parent)
    : QGraphicsView(parent)
{
    setMouseTracking(true);
//    setBackgroundBrush(QPixmap(":/backgrounds/layout"));
//    setBackgroundBrush(Qt::blue);
}

LayoutView::~LayoutView()
{
}

void LayoutView::createScene()
{
    //TODO: LayoutView::createScene(...)
    QGraphicsScene* scene = new LayoutScene(this);

    addScene(scene);
}

void LayoutView::addScene(QGraphicsScene* scene)
{
    //TODO: LayoutView::addScene(...)
    setScene(scene);
}

void LayoutView::removeScene(QGraphicsScene* scene)
{
    //TODO: LayoutView::removeScene(...)
}

void LayoutView::resizeEvent(QResizeEvent* event)
{
    //TODO: LayoutView::resizeEvent(...)

    QGraphicsView::resizeEvent(event);

//    qDebug() << "LayoutView::resizeEvent(QResizeEvent* event)" << event->size();

    LayoutScene* maluco = (LayoutScene*) scene();
    maluco->setSceneRect(QRectF(QPointF(0,0),
                                  QSizeF(event->size())));
    maluco->adjustItems();
}


}
}
}
}
}
