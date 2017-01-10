/* Copyright (c) 2011-2012 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */

#ifndef GENERALPREFERENCES_H
#define GENERALPREFERENCES_H

#include <extensions/IPreferencesPage.h>
#include "FileChooser.h"

namespace Ui {
  class GeneralPreferences;
}

namespace composer {
    namespace gui {

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

}} //end namespace
#endif // GENERALPREFERENCES_H
