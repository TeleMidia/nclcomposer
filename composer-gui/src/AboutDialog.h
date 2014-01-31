#ifndef ABOUTPLUGINS_H
#define ABOUTPLUGINS_H

#include <QDialog>

namespace Ui {
  class AboutDialog;
}

namespace composer {
  namespace gui {

class AboutDialog : public QDialog
{
  Q_OBJECT

public:
  /*!
     \brief Constructor.
     \param parent The parent of this class.
    */
  AboutDialog(QWidget *parent = 0);
  /*!
     \brief Destructor.
    */
  virtual ~AboutDialog();

private:
  Ui::AboutDialog *ui;

private slots:
  void showLicense();
};

} } // end namespace
#endif // ABOUTPLUGINS_H
