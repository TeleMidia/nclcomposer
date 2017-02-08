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

#ifndef PLUGINDETAILSDIALOG_H
#define PLUGINDETAILSDIALOG_H

#include <QDialog>

#include <extensions/IPluginFactory.h>
using namespace composer::core;

#include "ui_PluginDetailsDialog.h"

namespace composer {
    namespace gui {

class PluginDetailsDialog : public QDialog
{
private:
  Ui::PluginDetailsDialog *ui;
  IPluginFactory *currentPluginFactory;

public:
  PluginDetailsDialog(QWidget *parent = 0);
  virtual ~PluginDetailsDialog();

  void setCurrentPlugin(IPluginFactory *plugin);
};

} }
#endif // PLUGINDETAILSDIALOG_H
