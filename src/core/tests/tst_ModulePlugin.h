/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef TST_MODULEPLUGIN_H
#define TST_MODULEPLUGIN_H

#include <QObject>
#include <QTest>

#include "modules/PluginControl.h"
#include "modules/LanguageControl.h"
using namespace composer::core;

class tst_ModulePlugin : public QObject
{
  Q_OBJECT

public:
  void setBenchmark(bool isBench) { isBenchmark = isBench; }

private:
  PluginControl *pgControl;
  QString pluginDir;
  bool isBenchmark;

private slots:
  void initTestCase();
  void loadPluginDir();
  void pluginName_data();
  void pluginName();
  void pluginLoadBenchmark_data();
  void pluginLoadBenchmark();
  void cleanupTestCase();

};

#endif // TST_MODULEPLUGIN_H
