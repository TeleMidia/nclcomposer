#include <QtGui/QApplication>
#include <QResource>
#include "ComposerMainWindow.h"

using namespace composer::gui;


/*!
 \mainpage
NCL Composer is a multiplataform and flexible multimedia authoring tool of
interactive applications. It was first developed to NCL (Nested Context
Language) support, but its flexible architecture allows to extend it to support
almost all other multimedia languages, e.g. SMIL, SVG, and HTML 5.
*/
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
