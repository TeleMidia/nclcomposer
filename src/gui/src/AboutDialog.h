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

#ifndef ABOUTPLUGINS_H
#define ABOUTPLUGINS_H

#include "PluginDetailsDialog.h"

#include <modules/PluginControl.h>
#include <modules/LanguageControl.h>
using namespace composer::core;

#include <util/ComposerSettings.h>
using namespace composer::core::util;

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
  class AboutDialog;
}

namespace composer {
  namespace gui {

class AboutDialog : public QDialog
{
  Q_OBJECT

public:
  /*!
     \brief Constructor.
     \param parent The parent of this class.
    */
  AboutDialog(QWidget *parent = 0);
  /*!
     \brief Destructor.
    */
  virtual ~AboutDialog();

private:
  Ui::AboutDialog *ui;

private slots:
  void showLicense();
  void on_button_Homepage_pressed();
};


class AboutPluginsDialog : public QDialog
{
  Q_OBJECT

public:
  AboutPluginsDialog(QWidget *parent);
  virtual ~AboutPluginsDialog() { }

public slots:
  void loadPlugins();

private slots:
  void selectedAboutCurrentPluginFactory();
  void showPluginDetails();
  void saveLoadPluginData(int);

private:
  QTreeWidget *_treeWidgetPlugins;
  QMap <QTreeWidgetItem*, IPluginFactory*> _treeWidgetItem2plFactory;
  QPushButton *_detailsButton;
  PluginDetailsDialog *_pluginDetailsDialog;

};

} } // end namespace
#endif // ABOUTPLUGINS_H
