#ifndef LAYOUTCONTAINER_H
#define LAYOUTCONTAINER_H

#include "ui/view/abstractcontainer.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class LayoutContainer : public AbstractContainer
{
public:
    LayoutContainer(QWidget* parent = 0);
    virtual ~LayoutContainer();
};

}
}
}
}
}

#endif // LAYOUTCONTAINER_H
