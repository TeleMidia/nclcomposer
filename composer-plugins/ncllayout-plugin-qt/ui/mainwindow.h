#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui/view/layout/layouteditor.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class MainWindow : public LayoutEditor
{
public:
    MainWindow(QWidget* parent = 0);
    virtual ~MainWindow();
};

}
}
}
}
}

#endif // MAINWINDOW_H
