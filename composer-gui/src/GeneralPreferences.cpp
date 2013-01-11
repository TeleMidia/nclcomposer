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

#include <core/util/ComposerSettings.h>
using namespace composer::core::util;

#include <QLocale>

namespace composer {
    namespace gui {

GeneralPreferences::GeneralPreferences(QWidget *parent)
  : ui(new Ui::GeneralPreferences), IPreferencesPage(parent)
{
  ui->setupUi(this);
  ComposerSettings settings;
  settings.beginGroup("languages");
  QStringList list = settings.value("supportedLanguages").toStringList();

  foreach (QString cur, list)
  {
    QLocale locale(cur);

    ui->comboBox->insertItem(ui->comboBox->count(),
                             QLocale::languageToString(locale.language()),
                             cur);
  }

  QString current = settings.value("currentLanguage").toString();
  int index = list.indexOf(current);
  if(index >= 0)
    ui->comboBox->setCurrentIndex(index);
  settings.endGroup();


  QString currentQss = settings.value("default_stylesheets_dirs").toString();
  fileChooser = new FileChooser(tr("QSS File Theme:"),
                                FileChooser::GET_EXISTINGDIRECTORY,
                                "", "", this);

  fileChooser->setText(currentQss);
  ui->formLayout->setWidget( 6, QFormLayout::FieldRole, fileChooser);
}

GeneralPreferences::~GeneralPreferences()
{

}

void GeneralPreferences::applyValues()
{
  ComposerSettings settings;
  settings.beginGroup("languages");
  settings.setValue("currentLanguage",
                    ui->comboBox->itemData(ui->comboBox->currentIndex())
                    .toString());
  settings.endGroup();

  settings.setValue("default_stylesheets_dirs", fileChooser->getText());
}

void GeneralPreferences::setDefaultValues()
{

}

}} //end namespace
