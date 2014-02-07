#ifndef QNLYMAINWINDOW_H
#define QNLYMAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QMap>
#include "View.h"

namespace Ui {
  class QnlyMainWindow;
}

/*!
 * \brief The main windoe of Layout View.
 *
 * It groups all layout views, toolbars etc.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  /*!
   * \brief QnlyMainWindow constructor
   * \param parent
   */
  explicit MainWindow(QWidget *parent = 0);

  /*!
   * \brief Destructor
   */
  virtual ~MainWindow();

  // \todo unset QnlyView
  void setQnlyView(View *view);

public slots:
  void addRegion();

  void removeSelectedRegion();

  void addRegionBaseToCombobox(const QString &uuid,
                               const QMap<QString,QString> &properties);

  void removeRegionBaseFromCombobox(const QString &uuid);

  void updateRegionBaseInComboBox(const QString &uuid,
                                  const QMap<QString,QString> &properties);

  void comboBoxChangedCurrentRegionBase(int pos);

  void selectRegionBaseInComboBox(const QString &uuid);

signals:
  void regionBaseSelectedFromComboBox(const QString &uuid);
  void createNewRegionBase(const QString &id,
                           const QMap <QString, QString> &attrs);

private slots:
  void on_actionAction_Export_triggered();

private:
  Ui::QnlyMainWindow *ui;
  View *view;

};

#endif // QNLYMAINWINDOW_H
