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

#ifndef GENERALPREFERENCES_H
#define GENERALPREFERENCES_H

#include "IPreferencesPage.h"
#include "FileChooser.h"

namespace Ui {
  class GeneralPreferences;
}

CPR_GUI_BEGIN_NAMESPACE

class GeneralPreferences : public IPreferencesPage
{
public:
  /*!
   * \brief Constructor.
   */
  explicit GeneralPreferences(QWidget *parent = 0);
  /*!
   * \brief Destructor.
   */
  virtual ~GeneralPreferences();

  const QString getName() {return tr("General");}
  const QIcon getIcon() {return QIcon(":/mainwindow/config");}

public slots:
  /*!
   * \brief Save the current setted values to settings.
   */
  void applyValues();
  /*!
   * \brief Set the default values to settings.
   */
  void setDefaultValues();

private:
  Ui::GeneralPreferences *ui;
  FileChooser *fileChooser;
};

CPR_GUI_END_NAMESPACE

#endif // GENERALPREFERENCES_H
