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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QAbstractButton>
#include <QDialog>
#include <QDir>
#include <QListWidgetItem>
#include <QPluginLoader>

#include <extensions/IPluginFactory.h>
#include <modules/PluginControl.h>
using namespace cpr::core;

#include "IPreferencesPage.h"
using namespace cpr::gui;

namespace Ui
{
class PreferencesDialog;
}

CPR_GUI_BEGIN_NAMESPACE

class PreferencesDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PreferencesDialog (QWidget *parent = 0);
  ~PreferencesDialog ();

  void addPreferencePage (IPreferencesPage *page);
  /*!
   * \deprecated
   */
  void addPreferencePage (QIcon icon, QString name, IPreferencesPage *widget);

  /*public Q_SLOTS:
      void show();
      void selectFirst(); */

private:
  Ui::PreferencesDialog *ui;
  QListWidgetItem *currentItem;
  IPreferencesPage *currentPage;

  QMap<QString, IPreferencesPage *> pages;

private slots:
  void changeActivePage ();
  void applyCurrentValues ();
  void buttonClicked (QAbstractButton *);
};

CPR_GUI_END_NAMESPACE

#endif // PREFERENCES_H
