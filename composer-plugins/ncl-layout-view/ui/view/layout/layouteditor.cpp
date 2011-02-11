#include "layouteditor.h"

#include "layoutview.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

LayoutEditor::LayoutEditor(QWidget* parent)
    : AbstractEditor(parent)
{
    createWidgets();
}

LayoutEditor::~LayoutEditor()
{
}

void LayoutEditor::createWidgets()
{
    layoutContainer = new LayoutContainer(this);

    setCentralWidget(layoutContainer);
}

void LayoutEditor::addView(LayoutView* layout)
{
    layoutContainer->addWidget(layout);
}

}
}
}
}
}
