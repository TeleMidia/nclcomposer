#ifndef XMETAWIZARDMAINWINDOW_H
#define XMETAWIZARDMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class XMetaWizardMainWindow;
}

class XMetaWizardMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit XMetaWizardMainWindow(QWidget *parent = 0);
    ~XMetaWizardMainWindow();

public slots:
    void runWizardGenerator ();
    void runWizardEngine ();
    
};

#endif // XMETAWIZARDMAINWINDOW_H
