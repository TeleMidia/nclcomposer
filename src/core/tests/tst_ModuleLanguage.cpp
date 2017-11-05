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

#include "tst_ModuleLanguage.h"

void
tst_ModuleLanguage::initTestCase ()
{
  QString baseDir = "C:";
  baseDir += QDir::separator () + QString ("Composer");
  profileDir = baseDir + QDir::separator () + "lib" + QDir::separator ()
               + "composer" + QDir::separator ();
  lgControl = LanguageControl::getInstance ();
}

void
tst_ModuleLanguage::cleanupTestCase ()
{
  LanguageControl::releaseInstance ();
  lgControl = nullptr;
}

void
tst_ModuleLanguage::languageLoadBenchmark_data ()
{
  QTest::addColumn<QString> ("fileName");

#ifdef Q_WS_MAC
  QTest::newRow ("NCL") << profileDir + "libNCLLanguageProfile.dylib";
#elif WIN32
  QTest::newRow ("NCL") << profileDir + "NCLLanguageProfile.dll";
#else
  QTest::newRow ("NCL") << profileDir + "libNCLLanguageProfile.so";
#endif
}

void
tst_ModuleLanguage::languageLoadBenchmark ()
{
  if (!isBenchmark)
    QSKIP ("This test is not a benchmark test", SkipSingle);

  QFETCH (QString, fileName);

  QBENCHMARK
  {
    lgControl->loadProfile (fileName);
    lgControl->removeProfile (NCL);
  }
}

void
tst_ModuleLanguage::languageProfile_data ()
{
  QTest::addColumn<QString> ("fileName");

#ifdef Q_WS_MAC
  QTest::newRow ("NCL") << profileDir + "libNCLLanguageProfile.dylib";
#elif WIN32
  QTest::newRow ("NCL") << profileDir + "NCLLanguageProfile.dll";
#else
  QTest::newRow ("NCL") << profileDir + "libNCLLanguageProfile.so";
#endif
}

void
tst_ModuleLanguage::languageProfile ()
{
  QFETCH (QString, fileName);

  QList<ILanguageProfile *> list;

  /* Try to load the same profile */
  QVERIFY (lgControl->loadProfile (fileName));
  list = lgControl->getLoadedProfiles ();
  QCOMPARE (list.size (), 1);

  /* remove the loaded profile */
  QVERIFY (lgControl->removeProfile (NCL));
  list = lgControl->getLoadedProfiles ();
  QCOMPARE (list.isEmpty (), true);

  /* reload the profile from fileName */
  QVERIFY (lgControl->loadProfile (fileName));
  list = lgControl->getLoadedProfiles ();
  QCOMPARE (list.size (), 1);

  /* Try to load a non existing profile */
  QVERIFY (!lgControl->loadProfile ("blah"));
  list = lgControl->getLoadedProfiles ();
  QCOMPARE (list.size (), 1);

  /* remove the loaded profile */
  QVERIFY (lgControl->removeProfile (NCL));
  list = lgControl->getLoadedProfiles ();
  QCOMPARE (list.isEmpty (), true);
}
