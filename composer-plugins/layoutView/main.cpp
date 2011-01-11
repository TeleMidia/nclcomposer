#include <QtGui/QApplication>
#include "LayoutView.h"
#include <QGraphicsScene>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LayoutView w;
    w.show();
    return a.exec();
}
