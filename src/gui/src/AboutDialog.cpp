#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>

namespace composer {
namespace gui {

AboutDialog::AboutDialog(QWidget *parent):
  QDialog(parent, Qt::Dialog),
  ui(new Ui::AboutDialog)
{
  ui->setupUi(this);
  ui->label_ProgramName->setText( QString("NCL Composer v.") +
                                  QString(NCLCOMPOSER_GUI_VERSION) );

  ui->label_buildDate->setText(BUILD_DATE);

  connect(ui->button_Close, SIGNAL(pressed()), this, SLOT(close()));

  connect(ui->button_aboutQt, SIGNAL(pressed()), qApp, SLOT(aboutQt()));
}

AboutDialog::~AboutDialog()
{
  delete ui;
}

void AboutDialog::showLicense()
{
  QMessageBox box(this);
  QFile file(":/LICENSE.LGPL");
  box.setInformativeText(file.readAll());
  box.setTextFormat(Qt::RichText);
  box.exec();
}

} } // end namespace

void composer::gui::AboutDialog::on_button_Homepage_pressed()
{
  QDesktopServices::openUrl(QUrl("http://composer.telemidia.puc-rio.br"));
}
