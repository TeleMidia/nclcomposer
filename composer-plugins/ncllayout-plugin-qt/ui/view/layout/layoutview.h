#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

#include <QResizeEvent>

#include "ui/view/abstractview.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class LayoutView : public AbstractView
{
    Q_OBJECT

public:
    LayoutView(QWidget* parent = 0);
    virtual ~LayoutView();

public slots:
    void createScene();
    void addScene(AbstractScene* scene);
    void removeScene(AbstractScene* scene);

protected:
    virtual void resizeEvent(QResizeEvent* event);
};

}
}
}
}
}

#endif // LAYOUTVIEW_H
