#ifndef QNLYMAINWINDOW_H
#define QNLYMAINWINDOW_H

#include <QMainWindow>

#include "ui/view/qnlyview.h"

class QnlyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QnlyMainWindow(QWidget* parent = 0);

    virtual ~QnlyMainWindow();

private:
    void createMenus();

    void createActions();

    void createWidgets();

    void createConnections();

    void createToolBar();

    void createStatusBar();

    QnlyView* view;
};


#endif // QNLYMAINWINDOW_H
