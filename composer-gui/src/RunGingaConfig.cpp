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
#include <QSettings>

namespace composer {
namespace gui {

RunGingaConfig::RunGingaConfig(QWidget *parent):
  QWidget(parent), ui(new Ui::RunGingaConfig)
{
  ui->setupUi(this);

  QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                     "telemidia", "composer");

  // fix the tab order
  setTabOrder(ui->lineEdit_RemoteIP, ui->lineEdit_RemoteUser);
  setTabOrder(ui->lineEdit_RemoteUser, ui->lineEdit_RemotePassword);
  setTabOrder(ui->lineEdit_RemotePassword, ui->lineEdit_RemoteCmd);

  settings.beginGroup("runginga");
  ui->lineEdit_RemoteIP->setText(settings.value("remote_ip").toString());
  ui->lineEdit_RemoteUser->setText(settings.value("remote_user").toString());
  ui->lineEdit_RemotePassword->setText(settings.value("remote_password").toString());
  ui->lineEdit_RemoteCmd->setText(settings.value("remote_cmd").toString());
  settings.endGroup();
}

RunGingaConfig::~RunGingaConfig()
{

}

void RunGingaConfig::applyValues()
{
  qDebug() << "oi";
  QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                     "telemidia", "composer");

  settings.beginGroup("runginga");
  settings.setValue("remote_ip", ui->lineEdit_RemoteIP->text());
  settings.setValue("remote_user", ui->lineEdit_RemoteUser->text());
  settings.setValue("remote_password", ui->lineEdit_RemotePassword->text());
  settings.setValue("remote_cmd", ui->lineEdit_RemoteCmd->text());
  settings.endGroup();
}

} } //end namespace
