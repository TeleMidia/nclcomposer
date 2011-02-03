#include "abstractview.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

AbstractView::AbstractView(QWidget* parent)
    : QGraphicsView(parent)
{
    scenes = new QVector<AbstractScene*>();
}

AbstractView::~AbstractView()
{
    //TODO: AbstractView::~AbstractView()
    //TODO: Delete Scenes
}

QVector<AbstractScene*>* AbstractView::getScenes() const
{
    return scenes;
}

AbstractScene* AbstractView::getScene(int index) const
{
    return scenes->at(index);
}

}
}
}
}
}
