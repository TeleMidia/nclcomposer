#ifndef QNLYMAINWINDOW_H
#define QNLYMAINWINDOW_H

#include <QMainWindow>

#include "ui/view/qnlyview.h"

namespace Ui {
class QnlyMainWindow;
}

/*!
 * \brief The main windoe of Layout View.
 *
 * It groups all layout views, toolbars etc.
 */
class QnlyMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    /*!
     * \brief QnlyMainWindow constructor
     * \param parent
     */
    explicit QnlyMainWindow(QWidget *parent = 0);

    /*!
     * \brief Destructor
     */
    ~QnlyMainWindow();

    // \todo unset QnlyView
    void setQnlyView(QnlyView *view);
    
private:
    Ui::QnlyMainWindow *ui;
    QnlyView *view;

private slots:
    void addRegion();
    void removeRegion();
};

#endif // QNLYMAINWINDOW_H
