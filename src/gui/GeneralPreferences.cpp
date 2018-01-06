/* Copyright (C) 2011-2018 PUC-Rio/TeleMídia Lab.

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

#include "GeneralPreferences.h"
#include "ui_GeneralPreferences.h"

#include <util/ComposerSettings.h>
using namespace cpr::core;

#include <QDebug>
#include <QLocale>

CPR_GUI_BEGIN_NAMESPACE

GeneralPreferences::GeneralPreferences (QWidget *parent)
    : IPreferencesPage (parent), ui (new Ui::GeneralPreferences)
{
  ui->setupUi (this);
  GlobalSettings settings;
  settings.beginGroup ("languages");
  QStringList langs = settings.value ("supportedLanguages").toStringList ();

  for (const QString &cur : langs)
  {
    QLocale locale (cur);

    ui->comboBox->insertItem (ui->comboBox->count (),
                              QLocale::languageToString (locale.language ()),
                              cur);
  }

  QString current = settings.value ("currentLanguage").toString ();
  int index = langs.indexOf (current);
  if (index >= 0)
    ui->comboBox->setCurrentIndex (index);
  settings.endGroup ();

  settings.beginGroup ("theme");
  QString currentQss = settings.value ("stylesheet").toString ();
  fileChooser
      = new FileChooser (tr (""), FileChooser::OPEN_FILENAME, "", "", this);

  fileChooser->setText (currentQss);
  settings.endGroup ();

  ((QFormLayout *)ui->groupBox->layout ())
      ->setWidget (1, QFormLayout::FieldRole, fileChooser);
}

GeneralPreferences::~GeneralPreferences () {}

void
GeneralPreferences::applyValues ()
{
  GlobalSettings settings;
  settings.beginGroup ("languages");
  settings.setValue (
      "currentLanguage",
      ui->comboBox->itemData (ui->comboBox->currentIndex ()).toString ());
  settings.endGroup ();

  settings.beginGroup ("theme");
  settings.setValue ("stylesheet", fileChooser->getText ());
  settings.endGroup ();

  QFile css (fileChooser->getText ());
  css.open (QFile::ReadOnly);
  QString css_content = css.readAll ();
  qApp->setStyleSheet (css_content);
  foreach (QWidget *widget, qApp->topLevelWidgets ())
    widget->setStyleSheet (css_content);
  css.close ();
}

void
GeneralPreferences::setDefaultValues ()
{
}

CPR_GUI_END_NAMESPACE
