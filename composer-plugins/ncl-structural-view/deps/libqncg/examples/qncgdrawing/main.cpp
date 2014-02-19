#include <QApplication>

#include "ui/view.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("Qncg");
    application.setApplicationVersion("0.1.0");
    application.setOrganizationName("Telemidia Lab");
    application.setOrganizationDomain("telemidia.puc-rio.br");

    View view;
    view.setWindowTitle("Qncg");
    view.setMinimumWidth(800);
    view.setMinimumHeight(600);
    view.show();

    return application.exec();
}

