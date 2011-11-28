#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "view.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget* parent = 0);

    virtual ~MainWindow();

private:
    void createWidgets();

    View* view;
};

#endif // MAINWINDOW_H
