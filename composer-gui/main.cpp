#include <QtGui/QApplication>
#include <QResource>
#include "include/gui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QResource::registerResource("images.qrc");
    MainWindow w;
    w.show();
    return a.exec();
}
