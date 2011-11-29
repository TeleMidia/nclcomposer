#include <QApplication>

#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("Qncg");
    application.setApplicationVersion("0.1.0");
    application.setOrganizationName("Telemidia Lab");
    application.setOrganizationDomain("telemidia.puc-rio.br");

    MainWindow window;
    window.setMinimumWidth(800);
    window.setMinimumHeight(600);
    window.show();

    return application.exec();
}

