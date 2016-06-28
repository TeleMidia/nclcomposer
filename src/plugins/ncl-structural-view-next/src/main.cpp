#include <QApplication>
#include <QDebug>

#include "StructuralWindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("isbdesigner");
    application.setApplicationVersion("1.0.0");
    application.setOrganizationName("telemidia");
    application.setOrganizationDomain("telemidia.puc-rio.br");

    StructuralWindow window;
    window.setWindowTitle("skd");

    window.setMinimumWidth(800);
    window.setMinimumHeight(600);
    window.show();

    return application.exec();
}
