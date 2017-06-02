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

#include "RunGingaConfig.h"
#include "ui_RunGingaConfig.h"

#include <QFileDialog>
#include <QMessageBox>

#include <util/ComposerSettings.h>
using namespace cpr::core;

namespace composer {
namespace gui {

RunGingaConfig::RunGingaConfig(QWidget *parent):
  IPreferencesPage(parent), ui(new Ui::RunGingaConfig)
{
  ui->setupUi(this);

  // \fixme fix the tab order
  setTabOrder(ui->lineEdit_RemoteIP, ui->lineEdit_RemoteUser);
  setTabOrder(ui->lineEdit_RemoteUser, ui->lineEdit_RemotePassword);
  setTabOrder(ui->lineEdit_RemotePassword, ui->lineEdit_RemoteCmd);
  setTabOrder(ui->lineEdit_RemoteCmd, ui->lineEdit_RemoteStopCmd);

  initializeDefaultValues(); // After that I can assume all values exists.
  loadValuesFromSettings(); // Load the values from settings to UI

  connect(ui->localginga_Group, SIGNAL(toggled(bool)),
          this, SLOT(changeToLocal(bool)));

  connect(ui->remotevm_Group, SIGNAL(toggled(bool)),
          this, SLOT(changeToRemote(bool)));

  connect(ui->pushButton_Browse, SIGNAL(pressed()),
          this, SLOT(browseButtonPressed()));

  ui->pushButton_Browse->setEnabled(true);
}

RunGingaConfig::~RunGingaConfig()
{

}

void RunGingaConfig::initializeDefaultValues()
{
  GlobalSettings settings;
  settings.beginGroup("runginga");

  if(!settings.contains("remote_ip"))
    settings.setValue("remote_ip", "192.168.1.101");

  if(!settings.contains("remote_user"))
    settings.setValue("remote_user", "root");

  if(!settings.contains("remote_password"))
    settings.setValue("remote_password", "telemidia");

  if(!settings.contains("remote_start_cmd"))
    settings.setValue("remote_start_cmd", "/misc/launcher.sh");

  if(!settings.contains("remote_stop_cmd"))
    settings.setValue("remote_stop_cmd", "killall -9 ginga");

  if(!settings.contains("remote_path"))
    settings.setValue("remote_path", "/misc/ncl30");

  if(!settings.contains("run_remote"))
    settings.setValue("run_remote", false);

  if(!settings.contains("local_ginga_cmd"))
    settings.setValue("local_ginga_cmd", "ginga");

  if(!settings.contains("local_ginga_args"))
      settings.setValue("local_ginga_args", "--ncl ${nclpath}");

  if(!settings.contains("local_ginga_passive_args"))
      settings.setValue("local_ginga_passive_args", "--device-class 1");

  if(!settings.contains("local_ginga_active_args"))
      settings.setValue("local_ginga_active_args", "--device-class 2");

  settings.endGroup();
}

void RunGingaConfig::setDefaultValues()
{
  GlobalSettings settings;
  settings.remove("runginga/*");
}

void RunGingaConfig::loadValuesFromSettings()
{
  GlobalSettings settings;
  settings.beginGroup("runginga");
    //Enable/disable remote_run
    ui->remotevm_Group->setChecked(settings.value("run_remote").toBool());
    ui->localginga_Group->setChecked(!settings.value("run_remote").toBool());

    //Remote Run data
    ui->lineEdit_RemoteIP->setText(settings.value("remote_ip").toString());
    ui->lineEdit_RemoteUser->setText(settings.value("remote_user").toString());
    ui->lineEdit_RemotePassword->setText(settings.value("remote_password").toString());
    ui->lineEdit_RemoteCmd->setText(settings.value("remote_start_cmd").toString());
    ui->lineEdit_RemoteStopCmd->setText(settings.value("remote_stop_cmd").toString());
    ui->lineEdit_RemotePath->setText(settings.value("remote_path").toString());

    //Local Run data
    ui->lineEdit_local_Command->setText(settings.value("local_ginga_cmd").toString());
    ui->plainTextEdit_local_args->setPlainText(settings.value("local_ginga_args").toString());
    ui->plainTextEdit_local_passive_args->setPlainText(settings.value("local_ginga_passive_args").toString());
    ui->plainTextEdit_local_active_args->setPlainText(settings.value("local_ginga_active_args").toString());
  settings.endGroup();
}

void RunGingaConfig::applyValues()
{
  GlobalSettings settings;

  settings.beginGroup("runginga");
  settings.setValue("remote_ip", ui->lineEdit_RemoteIP->text());
  settings.setValue("remote_user", ui->lineEdit_RemoteUser->text());
  settings.setValue("remote_password", ui->lineEdit_RemotePassword->text());
  settings.setValue("remote_start_cmd", ui->lineEdit_RemoteCmd->text());
  settings.setValue("remote_stop_cmd", ui->lineEdit_RemoteStopCmd->text());
  settings.setValue("remote_path", ui->lineEdit_RemotePath->text());

  settings.setValue("run_remote", ui->remotevm_Group->isChecked());

  settings.setValue("local_ginga_cmd", ui->lineEdit_local_Command->text());
  settings.setValue("local_ginga_args",
                    ui->plainTextEdit_local_args->toPlainText());
  settings.setValue("local_ginga_passive_args",
                    ui->plainTextEdit_local_passive_args->toPlainText());
  settings.setValue("local_ginga_active_args",
                    ui->plainTextEdit_local_active_args->toPlainText());
  settings.endGroup();
}

void RunGingaConfig::changeToLocal(bool toLocal)
{
  if(ui->remotevm_Group->isChecked() == toLocal)
    ui->remotevm_Group->setChecked(!toLocal);
}

void RunGingaConfig::changeToRemote(bool toRemote)
{
#ifndef WITH_LIBSSH2
  if(toRemote)
  {
    QMessageBox::warning(this, tr("Error"),
                       tr("You can not enable remote because your system was not compiled with support for it!"));
    changeToLocal(true);
    return;
  }
#endif

  if(ui->localginga_Group->isChecked() ==  toRemote)
    ui->localginga_Group->setChecked(!toRemote);
}

void RunGingaConfig::browseButtonPressed()
{
  QString gingaPath = QFileDialog::getOpenFileName(this,
                                            tr("Ginga executable location"),
                                            ui->lineEdit_local_Command->text());
  if(gingaPath != "")
  {
#ifdef WIN32
    gingaPath = gingaPath.replace("\\", "/");
#endif
    ui->lineEdit_local_Command->setText(gingaPath);
  }
}

} } //end namespace
