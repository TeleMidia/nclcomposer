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

#include "NewProjectWizard.h"
#include "ui_NewProjectWizard.h"

#include <QDir>
#include <QFileDialog>

CPR_GUI_BEGIN_NAMESPACE

NewProjectWizard::NewProjectWizard (QWidget *parent)
    : QWizard (parent), ui (new Ui::NewProjectWizard)
{
  ui->setupUi (this);

  connect (ui->pushButton_Browser, SIGNAL (pressed ()), this,
           SLOT (browseDir ()));

  ui->lineEdit_Name->setFocus ();

  ui->lineEdit_CreateIn->setText (QDir::homePath ());
}

NewProjectWizard::~NewProjectWizard () { delete ui; }

void
NewProjectWizard::browseDir ()
{
  QString path = QFileDialog::getExistingDirectory (
      this, tr ("New Project Location"), ui->lineEdit_CreateIn->text ());

  if (!path.isNull () && !path.isEmpty ())
    ui->lineEdit_CreateIn->setText (path);
}

QString
NewProjectWizard::getProjectFullPath ()
{
  QString dir = ui->lineEdit_CreateIn->text ();

  return dir + "/" + ui->lineEdit_Name->text ();
}

bool
NewProjectWizard::shouldCopyDefaultConnBase ()
{
  return ui->checkBox_ImportConnectorBase->isChecked ();
}

bool
NewProjectWizard::shouldCreateADefaultRegion ()
{
  return ui->checkBox_AddDefaultRegion->isChecked ();
}

CPR_GUI_END_NAMESPACE
