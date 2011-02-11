#ifndef LAYOUTCONTAINER_H
#define LAYOUTCONTAINER_H

#include <QStackedWidget>
#include <QVector>
#include <QDebug>

#include "layoutview.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class LayoutContainer : public QStackedWidget
{
    Q_OBJECT

public:
    LayoutContainer(QWidget* parent = 0);
    virtual ~LayoutContainer();



    void backexpand();

    void exporttoimg();

protected:
//    void mouseMoveEvent ( QMouseEvent * event );
    void keyPressEvent( QKeyEvent * event );



//signals:
//    void closeDock();
//    void showDock();

private:
    QVector<LayoutView*>* views;
};

}
}
}
}
}

#endif // LAYOUTCONTAINER_H
