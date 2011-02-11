#ifndef LAYOUTEDITOR_H
#define LAYOUTEDITOR_H

#include "ui/view/abstracteditor.h"
#include "ui/view/layout/layoutview.h"

#include "layoutcontainer.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class LayoutEditor : public AbstractEditor
{
public:
    LayoutEditor(QWidget* parent = 0);
    virtual ~LayoutEditor();

    void addView(LayoutView* layout);

private:
    void createWidgets();



    LayoutContainer* layoutContainer;
};

}
}
}
}
}

#endif // LAYOUTEDITOR_H
