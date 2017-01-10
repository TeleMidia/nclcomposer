/* Copyright (c) 2011-2012 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */

#ifndef IPREFENCESPAGE_H
#define IPREFENCESPAGE_H

#include <QWidget>
#include <QIcon>

namespace composer {
    namespace gui {

class IPreferencesPage : public QWidget
{
  Q_OBJECT

public:
  IPreferencesPage(QWidget *parent) : QWidget(parent) { }

  virtual const QString getName() { return windowTitle(); }
  virtual const QIcon getIcon() = 0;

public slots:
  virtual void applyValues() = 0;
  virtual void setDefaultValues() = 0;
};

}} //end namespace
#endif
