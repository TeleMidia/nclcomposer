#include <QtGui/QApplication>
#include <QResource>
#include "ComposerMainWindow.h"

using namespace composer::ui;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    QResource::registerResource("images.qrc");
    QCoreApplication::setOrganizationName("Telemidia Lab");
    QCoreApplication::setOrganizationDomain("telemidia.pucrio.br");
    QCoreApplication::setApplicationName("composer");
    QPixmap mPix(":/mainwindow/icon");
    QSplashScreen splash(mPix);
    splash.blockSignals(true);
    splash.show();

    ComposerMainWindow w;
    splash.finish(&w);

    w.show();
    return a.exec();
}
