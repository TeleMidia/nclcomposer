/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef RUNGINGACONFIG_H
#define RUNGINGACONFIG_H

#include <QWidget>

namespace Ui {
  class RunGingaConfig;
}

namespace composer {
namespace gui {

/*!
 *
 * \todo User input validation.
 */
class RunGingaConfig : public QWidget
{
  Q_OBJECT
private:
  Ui::RunGingaConfig *ui;

public:
  explicit RunGingaConfig(QWidget *parent = 0);
  ~RunGingaConfig();

public slots:
  void applyValues();
};

} } //end namespace
#endif // RUNGINGACONFIG_H
