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

#ifndef ABOUTPLUGINS_H
#define ABOUTPLUGINS_H

#include <modules/LanguageControl.h>
#include <modules/PluginControl.h>
#include <util/ComposerSettings.h>
using namespace cpr::core;

#include <QDialog>
#include <QStringListModel>
#include <QTreeWidget>

namespace Ui
{
class AboutDialog;
class PluginDetailsDialog;
class ShortcutsDialog;
}

CPR_GUI_BEGIN_NAMESPACE

class AboutDialog : public QDialog
{
  Q_OBJECT

public:
  /*!
     \brief Constructor.
     \param parent The parent of this class.
    */
  AboutDialog (QWidget *parent = 0);
  /*!
     \brief Destructor.
    */
  virtual ~AboutDialog ();

private:
  Ui::AboutDialog *ui;

private slots:
  void showLicense ();
  void on_button_Homepage_pressed ();
};

class PluginDetailsDialog : public QDialog
{
private:
  Ui::PluginDetailsDialog *ui;
  IPluginFactory *currentPluginFactory;

public:
  PluginDetailsDialog (QWidget *parent = 0);
  virtual ~PluginDetailsDialog ();

  void setCurrentPlugin (IPluginFactory *plugin);
};

class AboutPluginsDialog : public QDialog
{
  Q_OBJECT

public:
  AboutPluginsDialog (QWidget *parent);
  virtual ~AboutPluginsDialog () {}

public slots:
  void loadPlugins ();

private slots:
  void selectedAboutCurrentPluginFactory ();
  void showPluginDetails ();
  void saveLoadPluginData (int);

private:
  QTreeWidget *_treeWidgetPlugins;
  QPushButton *_detailsButton;
  PluginDetailsDialog *_pluginDetailsDialog;
};

class ShortcutsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ShortcutsDialog (QWidget *parent = 0);
  ~ShortcutsDialog ();

private:
  QStringListModel _model;
  Ui::ShortcutsDialog *_ui;
};

CPR_GUI_END_NAMESPACE

#endif // ABOUTPLUGINS_H
