#include "abstractitem.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

AbstractItem::AbstractItem(QGraphicsItem* parent)
    : QGraphicsItem(parent)
{
}

AbstractItem::~AbstractItem()
{
    //TODO: AbstractItem::~AbstractItem()
    //TODO: Delete Items
}

QVector<AbstractItem*>* AbstractItem::getItems() const
{
//    return items;
}

AbstractItem* AbstractItem::getItem(int index) const
{
//    return items->at(index);
}

}
}
}
}
}
