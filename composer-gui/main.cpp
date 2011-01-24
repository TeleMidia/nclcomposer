#include <QtGui/QApplication>
#include <QResource>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QResource::registerResource("images.qrc");
    QCoreApplication::setOrganizationName("telemidia");
    QCoreApplication::setOrganizationDomain("telemidia.pucrio.br");
    QCoreApplication::setApplicationName("composer");
    MainWindow w;
    w.setStyleSheet("font-size: 12px");
    w.show();
    return a.exec();
}
