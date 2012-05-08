/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "RunGingaConfig.h"
#include "ui_RunGingaConfig.h"

#include <QDebug>

#include <core/util/ComposerSettings.h>
using namespace composer::core::util;

namespace composer {
namespace gui {

RunGingaConfig::RunGingaConfig(QWidget *parent):
  IPreferencesPage(parent), ui(new Ui::RunGingaConfig)
{
  ui->setupUi(this);

  ComposerSettings settings;

  // \fixme fix the tab order
  setTabOrder(ui->lineEdit_RemoteIP, ui->lineEdit_RemoteUser);
  setTabOrder(ui->lineEdit_RemoteUser, ui->lineEdit_RemotePassword);
  setTabOrder(ui->lineEdit_RemotePassword, ui->lineEdit_RemoteCmd);
  setTabOrder(ui->lineEdit_RemoteCmd, ui->lineEdit_RemoteStopCmd);

  settings.beginGroup("runginga");
  if(settings.contains("remote_ip"))
    ui->lineEdit_RemoteIP->setText(settings.value("remote_ip").toString());

  if(settings.contains("remote_user"))
    ui->lineEdit_RemoteUser->setText(settings.value("remote_user").toString());

  if(settings.contains("remote_password"))
    ui->lineEdit_RemotePassword->setText(
          settings.value("remote_password").toString());

  if(settings.contains("remote_start_cmd"))
    ui->lineEdit_RemoteCmd->setText(
          settings.value("remote_start_cmd").toString());

  if(settings.contains("remote_stop_cmd"))
    ui->lineEdit_RemoteStopCmd->setText(
          settings.value("remote_stop_cmd").toString());

  if(settings.contains("remote_path"))
    ui->lineEdit_RemotePath->setText(settings.value("remote_path").toString());

  if(settings.contains("run_remote"))
  {
    ui->remotevm_Group->setChecked(settings.value("run_remote").toBool());
    ui->localginga_Group->setChecked(!settings.value("run_remote").toBool());
  }

  if(settings.contains("local_ginga_cmd"))
    ui->lineEdit_local_command->setText(
          settings.value("local_ginga_cmd").toString());

  settings.endGroup();

  connect(ui->remotevm_Group, SIGNAL(clicked(bool)),
          this, SLOT(changeCurrentPlayMode(bool)));

  connect(ui->localginga_Group, SIGNAL(toggled(bool)),
          this, SLOT(changeToLocal(bool)));

  connect(ui->remotevm_Group, SIGNAL(toggled(bool)),
          this, SLOT(changeToRemote(bool)));
}

RunGingaConfig::~RunGingaConfig()
{

}

void RunGingaConfig::applyValues()
{
  ComposerSettings settings;

  settings.beginGroup("runginga");
  settings.setValue("remote_ip", ui->lineEdit_RemoteIP->text());
  settings.setValue("remote_user", ui->lineEdit_RemoteUser->text());
  settings.setValue("remote_password", ui->lineEdit_RemotePassword->text());
  settings.setValue("remote_start_cmd", ui->lineEdit_RemoteCmd->text());
  settings.setValue("remote_stop_cmd", ui->lineEdit_RemoteStopCmd->text());
  settings.setValue("remote_path", ui->lineEdit_RemotePath->text());

  settings.setValue("run_remote", ui->remotevm_Group->isChecked());

  settings.setValue("local_ginga_cmd", ui->lineEdit_local_command->text());

  settings.endGroup();
}

void RunGingaConfig::setDefaultValues()
{

}

void RunGingaConfig::changeToLocal(bool toLocal)
{
  qDebug() << "RunGingaConfig::changeToLocal" << toLocal;
  if(ui->remotevm_Group->isChecked() == toLocal)
    ui->remotevm_Group->setChecked(!toLocal);
}

void RunGingaConfig::changeToRemote(bool toRemote)
{
  qDebug() << "RunGingaConfig::changeToRemote" << toRemote;
  if(ui->localginga_Group->isChecked() ==  toRemote)
    ui->localginga_Group->setChecked(!toRemote);
}

} } //end namespace
