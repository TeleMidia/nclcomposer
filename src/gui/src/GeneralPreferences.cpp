/* Copyright (c) 2011-2012 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "GeneralPreferences.h"
#include "ui_GeneralPreferences.h"

#include <util/ComposerSettings.h>
using namespace composer::core::util;

#include <QLocale>
#include <QDebug>

namespace composer {
    namespace gui {

GeneralPreferences::GeneralPreferences(QWidget *parent)
  : IPreferencesPage(parent), ui(new Ui::GeneralPreferences)
{
  ui->setupUi(this);
  GlobalSettings settings;
  settings.beginGroup("languages");
  QStringList langs = settings.value("supportedLanguages").toStringList();

  for (const QString &cur: langs)
  {
    QLocale locale(cur);

    ui->comboBox->insertItem(ui->comboBox->count(),
                             QLocale::languageToString(locale.language()),
                             cur);
  }

  QString current = settings.value("currentLanguage").toString();
  int index = langs.indexOf(current);
  if(index >= 0)
    ui->comboBox->setCurrentIndex(index);
  settings.endGroup();

  settings.beginGroup("theme");
  QString currentQss = settings.value("stylesheet").toString();
  fileChooser = new FileChooser(tr(""),
                                FileChooser::OPEN_FILENAME,
                                "", "", this);

  fileChooser->setText(currentQss);
  settings.endGroup();

  ((QFormLayout *)ui->groupBox->layout())->setWidget(1, QFormLayout::FieldRole, fileChooser);
}

GeneralPreferences::~GeneralPreferences()
{

}

void GeneralPreferences::applyValues()
{
  GlobalSettings settings;
  settings.beginGroup("languages");
  settings.setValue("currentLanguage",
                    ui->comboBox->itemData(ui->comboBox->currentIndex())
                    .toString());
  settings.endGroup();

  settings.beginGroup("theme");
  settings.setValue("stylesheet", fileChooser->getText());
  settings.endGroup();

  QFile css (fileChooser->getText());
  css.open(QFile::ReadOnly);
  QString css_content = css.readAll();
  qApp->setStyleSheet(css_content);
  foreach(QWidget *widget, qApp->topLevelWidgets())
    widget->setStyleSheet(css_content);
  css.close();
}

void GeneralPreferences::setDefaultValues()
{

}

}} //end namespace
