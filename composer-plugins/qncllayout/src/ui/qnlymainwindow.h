#ifndef QNLYMAINWINDOW_H
#define QNLYMAINWINDOW_H

#include <QMainWindow>

#include "ui/view/qnlyview.h"

namespace Ui {
class QnlyMainWindow;
}

class QnlyMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit QnlyMainWindow(QWidget *parent = 0);
    ~QnlyMainWindow();

    void setQnlyView(QnlyView *view);
    
private:
    Ui::QnlyMainWindow *ui;
};

#endif // QNLYMAINWINDOW_H
