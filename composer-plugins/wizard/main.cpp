#include "xmetawizardmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XMetaWizardMainWindow w;
    w.show();
    
    return a.exec();
}
