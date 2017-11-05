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

#include "tst_ModuleInit.h"

void
tst_ModuleInit::initTestCase ()
{
  QString baseDir = "C:";
  baseDir += QDir::separator () + QString ("Composer");
  profileDir = baseDir + QDir::separator () + "lib" + QDir::separator ()
               + "composer" + QDir::separator ();
  pluginDir = baseDir + QDir::separator () + "plugins" + QDir::separator ();
}

void
tst_ModuleInit::initBenchmark_data ()
{
  QTest::addColumn<bool> ("instanciate");

  QTest::newRow ("init") << true;
  QTest::newRow ("release") << false;
}

void
tst_ModuleInit::initBenchmark ()
{
  if (!isBenchmark)
    QSKIP ("This test is not a benchmark test", SkipSingle);

  QFETCH (bool, instanciate);

  if (instanciate)
  {
    QBENCHMARK
    {
      // docControl = DocumentControl::getInstance();
      // QVERIFY(docControl);
      lgControl = LanguageControl::getInstance ();
      QVERIFY (lgControl);
      pgControl = PluginControl::getInstance ();
      QVERIFY (pgControl);
    }
    // DocumentControl::releaseInstance();
    LanguageControl::releaseInstance ();
    PluginControl::releaseInstance ();
  }
  else
  {
    // docControl = DocumentControl::getInstance();
    // QVERIFY(docControl);
    lgControl = LanguageControl::getInstance ();
    QVERIFY (lgControl);
    pgControl = PluginControl::getInstance ();
    QVERIFY (pgControl);

    QBENCHMARK
    {
      // DocumentControl::releaseInstance();
      LanguageControl::releaseInstance ();
      PluginControl::releaseInstance ();
    }
  }
}

void
tst_ModuleInit::initTorture ()
{

  for (int i = 0; i < interations; i++)
  {
    // docControl = DocumentControl::getInstance();
    // QVERIFY(docControl);
    lgControl = LanguageControl::getInstance ();
    QVERIFY (lgControl);
    pgControl = PluginControl::getInstance ();
    QVERIFY (pgControl);

    // DocumentControl::releaseInstance();
    LanguageControl::releaseInstance ();
    PluginControl::releaseInstance ();
  }
}

void
tst_ModuleInit::languageProfile ()
{
  QList<ILanguageProfile *> list;

  /* Try to load the same profile */
  QVERIFY (lgControl->loadProfile (profileDir + "NCLLanguageProfile.so"));
  list = lgControl->getLoadedProfiles ();
  QCOMPARE (list.size (), 1);

  //    /* remove the loaded profile */
  //    QVERIFY(lgControl->removeProfile(NCL));
  //    list = lgControl->getLoadedProfiles();
  //    QCOMPARE(list.isEmpty(),true);

  //    /* reload the profile from fileName */
  //    QVERIFY(lgControl->loadProfile
  //            (profileDir+"libNCLLanguageProfile.dylib"));
  //    list = lgControl->getLoadedProfiles();
  //    QCOMPARE(list.size(),1);

  //    /* Try to load a non existing profile */
  //    QVERIFY(!lgControl->loadProfile("blah"));
  //    list = lgControl->getLoadedProfiles();
  //    QCOMPARE(list.size(),1);
}

// void tst_ModuleInit::pluginProfile()
//{
//    QList<IPluginFactory*> list;

//    pgControl->loadPlugins(pluginDir);
//    list = pgControl->getLoadedPlugins();
//    QCOMPARE(list.isEmpty(),false);

//    /* Load the same plugin */
//    QVERIFY(pgControl->loadPlugin(pluginDir+"libdebug_console.dylib"));
//    list = pgControl->getLoadedPlugins();
//    QCOMPARE(list.size(),1);

//    /* load a non existent plugin */
//    QVERIFY(!pgControl->loadPlugin("blah"));
//    list = pgControl->getLoadedPlugins();
//    QCOMPARE(list.size(),1);

//}

// void tst_ModuleInit::launchDocument_data()
//{
//    QTest::addColumn<QString>("fileName");

//    QDir docDir(documentDir);
//    docDir.setFilter(QDir::Files | QDir::NoSymLinks);
//    docDir.setSorting(QDir::Size | QDir::Reversed);

//    QFileInfoList list = docDir.entryInfoList();

//    for (int i = 0; i != list.size(); i++)
//    {
//        QFileInfo file = list.at(i);
//        QString absFile = file.absoluteFilePath();
//        int size = file.size();
//        qDebug() << absFile << size;
//        QTest::newRow("openDoc") << absFile;
//    }
//}

// void tst_ModuleInit::closeDocument_data()
//{
//    QTest::addColumn<QString>("fileName");

//    QDir docDir(documentDir);
//    docDir.setFilter(QDir::Files | QDir::NoSymLinks);
//    docDir.setSorting(QDir::Size | QDir::Reversed);

//    QFileInfoList list = docDir.entryInfoList();

//    for (int i = 0; i != list.size(); i++)
//    {
//        QFileInfo file = list.at(i);
//        QString absFile = file.absoluteFilePath();
//        int size = file.size();
//        qDebug() << absFile << size;
//        QTest::newRow("closeDoc") << absFile;
//    }
//}

// void tst_ModuleInit::launchAndCloseDocument_data()
//{
//    QTest::addColumn<QString>("fileName");
//    QTest::addColumn<bool>("openDoc");

//    QDir docDir(documentDir);
//    docDir.setFilter(QDir::Files | QDir::NoSymLinks);
//    docDir.setSorting(QDir::Size | QDir::Reversed);

//    QFileInfoList list = docDir.entryInfoList();

//    for (int i = 0; i != list.size(); i++)
//    {
//        QFileInfo file = list.at(i);
//        QString absFile = file.absoluteFilePath();
//        int size = file.size();
//        qDebug() << absFile << size;
//        QTest::newRow("openDoc") << absFile << true;
//        QTest::newRow("closeDoc") << absFile << false;
//        if (i == 1) break;
//    }
//}

// void tst_ModuleInit::launchDocument()
//{

//    QFETCH(QString, fileName);

//    QBENCHMARK
//    {
//        docControl->launchDocument("testing",fileName);
//    }
//}

// void tst_ModuleInit::closeDocument()
//{
//    QFETCH(QString, fileName);

//    QBENCHMARK
//    {
//        docControl->closeDocument(fileName);
//    }
//}

// void tst_ModuleInit::launchAndCloseDocument()
//{
//    QFETCH(QString, fileName);
//    QFETCH(bool, openDoc);

//    if (openDoc)
//    {
//        QBENCHMARK
//        {
//            docControl->launchDocument("testing",fileName);
//        }
//    } else {
//        QBENCHMARK
//        {
//            docControl->closeDocument(fileName);
//        }
//    }
//}

void
tst_ModuleInit::cleanupTestCase ()
{
}
