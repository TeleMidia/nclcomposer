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

#include "tst_InsertNode.h"
#include "tst_ModuleInit.h"
#include "tst_ModuleLanguage.h"
#include "tst_ModulePlugin.h"
#include "tst_ModuleProject.h"

int
run_ModuleInit ()
{
  tst_ModuleInit testModuleInit;
  testModuleInit.setInterations (10240);
  testModuleInit.setBenchmark (true);
  QStringList args;
  args.append ("-v2");
  return QTest::qExec (&testModuleInit, args);
}

int
run_ModuleLanguage ()
{
  tst_ModuleLanguage testModuleLanguage;
  testModuleLanguage.setBenchmark (true);
  QStringList args;
  args.append ("-v2");
  return QTest::qExec (&testModuleLanguage, args);
}

int
run_ModuleProject ()
{
  // run_ModuleInit();
  run_ModuleLanguage ();
}

int
run_InsertNode ()
{
  tst_InsertNode testInsertNode;
  QStringList args;
  args.append ("-v2");
  return QTest::qExec (&testInsertNode, args);
}

// QTEST_MAIN(tst_ModuleProject)
// QTEST_MAIN(tst_ModulePlugin)
QTEST_MAIN (tst_InsertNode)
