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

#ifndef RUNGINGACONFIG_H
#define RUNGINGACONFIG_H

#include "IPreferencesPage.h"

namespace Ui {
  class RunGingaConfig;
}

namespace composer {
namespace gui {

/*!
 * \todo User input validation.
 */
class RunGingaConfig :
        public IPreferencesPage
{
  Q_OBJECT

public:
  explicit RunGingaConfig(QWidget *parent = 0);
  ~RunGingaConfig();

  const QString getName() {return tr("Run Configuration");}
  const QIcon getIcon() {return QIcon(":/mainwindow/images/play_button.png");}

public slots:
  void applyValues();
  void setDefaultValues();

  void changeToLocal(bool);
  void changeToRemote(bool);
  void browseButtonPressed();


private:
  Ui::RunGingaConfig *ui;

  void initializeDefaultValues();
  void loadValuesFromSettings();
};

} } //end namespace
#endif // RUNGINGACONFIG_H
