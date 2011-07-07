#include "qnlycanvas.h"

QnlyCanvas::QnlyCanvas(QWidget* parent) : QGraphicsView(parent)
{

}

QnlyCanvas::~QnlyCanvas()
{

}

void QnlyCanvas::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

    QSize size = event->size();

    QGraphicsScene* s = scene();

    s->setSceneRect(0,0,size.width(),size.height());

    foreach(QGraphicsItem* item, s->items())
    {
        QnlyGraphicsRegion* region = (QnlyGraphicsRegion*) item;

        region->adjust();
    }
}
