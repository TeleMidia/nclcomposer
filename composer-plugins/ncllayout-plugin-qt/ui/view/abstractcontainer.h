#ifndef ABSTRACTCONTAINER_H
#define ABSTRACTCONTAINER_H

#include <QStackedWidget>

#include "abstractview.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class AbstractContainer : public QStackedWidget
{
public:
    AbstractContainer(QWidget* parent = 0);
    virtual ~AbstractContainer();
};

}
}
}
}
}

#endif // ABSTRACTCONTAINER_H
