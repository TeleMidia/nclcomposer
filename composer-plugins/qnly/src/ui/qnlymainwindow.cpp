#include "qnlymainwindow.h"

QnlyMainWindow::QnlyMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qnly");

    createActions();
    createMenus();
    createToolBar();
    createStatusBar();
    createWidgets();
    createConnections();
}

QnlyMainWindow::~QnlyMainWindow()
{
}

void QnlyMainWindow::createMenus()
{

}

void QnlyMainWindow::createActions()
{

}

void QnlyMainWindow::createConnections()
{

}

void QnlyMainWindow::createToolBar()
{

}

void QnlyMainWindow::createStatusBar()
{

}

void QnlyMainWindow::createWidgets()
{
    view = new QnlyView(this);

    setCentralWidget(view);
}
