#include <QApplication>

#include "qnstmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setApplicationName("nclstructural");
    application.setApplicationVersion("1.0.0");
    application.setOrganizationName("telemidia");
    application.setOrganizationDomain("telemidia.puc-rio.br");

    QnstMainWindow window;
    window.setWindowTitle("NCL Structural");
    window.setMinimumWidth(800);
    window.setMinimumHeight(600);
    window.show();

    return application.exec();
}
