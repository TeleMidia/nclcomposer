#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qncg");

    createWidgets();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createWidgets()
{
    view = new View();
    view->setParent(this);

    setCentralWidget(view);
}
