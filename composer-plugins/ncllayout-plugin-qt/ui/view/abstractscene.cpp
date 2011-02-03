#include "abstractscene.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

AbstractScene::AbstractScene(QObject* parent)
    : QGraphicsScene(parent)
{
//    items = new QVector<AbstractItem*>();
}

AbstractScene::~AbstractScene()
{
    //TODO: AbstractDocument::~AbstractDocument()
    //TODO: Delete Items
}

}
}
}
}
}
