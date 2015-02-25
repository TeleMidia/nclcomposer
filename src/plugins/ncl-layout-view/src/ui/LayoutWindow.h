#ifndef QNLYMAINWINDOW_H
#define QNLYMAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QMap>
#include "LayoutView.h"

namespace Ui {
  class QnlyMainWindow;
}

/*!
 * \brief The main windoe of Layout View.
 *
 * It groups all layout views, toolbars etc.
 */
class LayoutWindow : public QMainWindow
{
  Q_OBJECT

public:
  /*!
   * \brief QnlyMainWindow constructor
   * \param parent
   */
  explicit LayoutWindow(QWidget *parent = 0);

  /*!
   * \brief Destructor
   */
  virtual ~LayoutWindow();

  // \todo unset QnlyView
  void setQnlyView(LayoutView *view);

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
  LayoutView *view;

};

#endif // QNLYMAINWINDOW_H
