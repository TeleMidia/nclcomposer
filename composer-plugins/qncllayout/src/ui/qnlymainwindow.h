#ifndef QNLYMAINWINDOW_H
#define QNLYMAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QMap>
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

signals:
    void selectRegionBaseFromComboBox(const QString &uuid);
    
private:
    Ui::QnlyMainWindow *ui;
    QnlyView *view;

public slots:
    void addRegion();

    void removeSelectedRegion();

    void addRegionBaseToCombobox(const QString &uuid,
                                 const QMap<QString,QString> &properties);

    void removeRegionBaseFromCombobox(const QString &uuid);

    void comboBoxChangedCurrentRegionBase(int pos);

private slots:
    void on_actionAction_Export_triggered();
};

#endif // QNLYMAINWINDOW_H
