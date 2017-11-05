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

#ifndef TST_MODULEINIT_H
#define TST_MODULEINIT_H

#include <QObject>
#include <QTest>

// #include <modules/DocumentControl.h>
#include <modules/LanguageControl.h>
#include <modules/PluginControl.h>
// #include <modules/TransactionControl.h>
using namespace composer::core;

class tst_ModuleInit : public QObject
{
  Q_OBJECT

public:
  void
  setInterations (int _interartions)
  {
    interations = _interartions;
  }
  void
  setBenchmark (bool isBench)
  {
    isBenchmark = isBench;
  }

private:
  LanguageControl *lgControl;
  // DocumentControl *docControl;
  PluginControl *pgControl;
  int interations;
  bool isBenchmark;
  QString profileDir;
  QString pluginDir;
  QString documentDir;

private slots:
  void initTestCase ();
  void initBenchmark_data ();
  void initTorture ();
  void initBenchmark ();
  void languageProfile ();
  //  void pluginProfile();
  //  void launchDocument();
  //  void launchDocument_data();
  //  void closeDocument();
  //  void closeDocument_data();
  //  void launchAndCloseDocument();
  //  void launchAndCloseDocument_data();
  void cleanupTestCase ();
};

#endif // TST_MODULEINIT_H
