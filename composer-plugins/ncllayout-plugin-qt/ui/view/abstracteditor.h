#ifndef ABSTRACTEDITOR_H
#define ABSTRACTEDITOR_H

#include <QMainWindow>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class AbstractEditor : public QMainWindow
{
public:
    AbstractEditor(QWidget* parent = 0);
    virtual ~AbstractEditor();
};

}
}
}
}
}

#endif // ABSTRACTEDITOR_H
