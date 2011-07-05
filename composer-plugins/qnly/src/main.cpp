#include <QApplication>

#include "ui/qnlymainwindow.h"

#include "ui/view/qnlyview.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    QnlyMainWindow window;
    window.showMaximized();

    return application.exec();
}
