#include "layoutcontainer.h"

#include <QKeyEvent>
#include <QDesktopWidget>

#include "layoutview.h"

#include <QFileDialog>
#include <QImage>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

LayoutContainer::LayoutContainer(QWidget* parent)
    : QStackedWidget(parent)
{
    setMouseTracking(true);
}

LayoutContainer::~LayoutContainer()
{
}

void LayoutContainer::keyPressEvent( QKeyEvent * event )
{
    qDebug() << "keyPressEvent";

    if (event->key() == Qt::Key_Escape){
        backexpand();
        qDebug() << "out with escape";
    }

    if (event->key() == Qt::Key_Space){
        backexpand();
        qDebug() << "out with space";
    }


    if (event->key() == Qt::Key_R &&
        event->modifiers() == Qt::ControlModifier | Qt::ShiftModifier){
        exporttoimg();;
    }
}

void LayoutContainer::exporttoimg()
{
    LayoutView* layoutView = (LayoutView*) currentWidget();

    QImage img(layoutView->scene()->width(),
               layoutView->scene()->height(),
               QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    layoutView->scene()->render(&p);
//    scene.render(&p);
    p.end();
    img.save(QFileDialog::getSaveFileName(this));
}

/*
void LayoutContainer::mouseMoveEvent ( QMouseEvent * event ){
    if (isFullScreen()){
        if (QRect(0,0,30,rect().height()).contains(event->pos())){
            emit showDock();;
        }else{
            emit closeDock();;
        }
    }
}
*/
void LayoutContainer::backexpand()
{
    this->setWindowState(Qt::WindowNoState);
    this->setWindowFlags(Qt::Widget);
    this->show();
}

}
}
}
}
}
