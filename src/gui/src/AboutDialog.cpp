/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

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
                                  qApp->applicationVersion());

  ui->label_buildDate->setText(QString (BUILD_DATE));

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
