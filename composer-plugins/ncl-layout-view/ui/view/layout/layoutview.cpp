#include "layoutview.h"

#include "layoutscene.h"

#include <QDebug>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

LayoutView::LayoutView(QWidget* parent)
    : AbstractView(parent)
{
//    setBackgroundBrush(Qt::blue);
}

LayoutView::~LayoutView()
{
}

void LayoutView::createScene()
{
    //TODO: LayoutView::createScene(...)
    AbstractScene* scene = new LayoutScene(this);

    addScene(scene);
}

void LayoutView::addScene(AbstractScene* scene)
{
    //TODO: LayoutView::addScene(...)
    setScene(scene);
}

void LayoutView::removeScene(AbstractScene* scene)
{
    //TODO: LayoutView::removeScene(...)
}

void LayoutView::resizeEvent(QResizeEvent* event)
{
    //TODO: LayoutView::resizeEvent(...)

    AbstractView::resizeEvent(event);

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
