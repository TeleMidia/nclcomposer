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

#include "tst_InsertNode.h"

#include <fstream>
#include <iostream>
using namespace std;

void
tst_InsertNode::initTestCase ()
{
  nNodes = 10000;

  pjControl = ProjectControl::getInstance ();
  resourceDir = "resources";
  resourceDir
      = resourceDir + QDir::separator () + "documents" + QDir::separator ();

  QString baseDir = "..";
  profileDir
      = baseDir + QDir::separator () + "extensions" + QDir::separator ();
  // +"composer"+QDir::separator();
  lgControl = LanguageControl::getInstance ();

  pluginList = TestUtil::getPluginList ();

  pgControl = PluginControl::getInstance ();

  connect (pgControl, SIGNAL (addPluginWidgetToWindow (
                          IPluginFactory *, IPlugin *, Project *, int)),
           &showWidgets, SLOT (showPluginWidget (IPluginFactory *, IPlugin *,
                                                 Project *, int)));
}

void
tst_InsertNode::cleanupTestCase ()
{
  ProjectControl::releaseInstance ();
  pjControl = nullptr;
  LanguageControl::releaseInstance ();
  lgControl = nullptr;
  PluginControl::releaseInstance ();
  pgControl = nullptr;

  /* Clean up the project directory */
  QString projectDir = resourceDir + QDir::separator () + "projects";
  QDir docDir (projectDir);
  docDir.setFilter (QDir::Files | QDir::NoSymLinks);
  QFileInfoList list = docDir.entryInfoList ();

  for (int i = 0; i != list.size (); i++)
  {
    QFileInfo fileInfo = list.at (i);
    cout << "Trying to remove " << fileInfo.absoluteFilePath ().toStdString ();
    QFile file (fileInfo.absoluteFilePath ());
    file.remove ();
  }
}

void
tst_InsertNode::init ()
{
#ifdef Q_WS_MAC
  QString filename = profileDir + QDir::separator () + "NCL"
                     + QDir::separator () + "libNCLLanguageProfile.dylib";
#elif WIN32
// TODO:
#else
  QString filename = profileDir + QDir::separator () + "NCL"
                     + QDir::separator () + "libNCLLanguageProfile.so";
#endif
  QVERIFY (lgControl->loadProfile (filename));
}

void
tst_InsertNode::insertNode_data ()
{
  QTest::addColumn<QString> ("projectFileName");
  QTest::addColumn<int> ("nNodes");
  QTest::addColumn<int> ("numPlugins");

  for (int i = 0; i < pluginList.size (); i++)
  {
    QString testName = "teste" + i;
    QString projectName = QDir::separator ();
    projectName += "tmp";
    projectName += QDir::separator ();
    projectName += "project";
    projectName += ".cpr";
    QTest::newRow (testName.toStdString ().c_str ()) << projectName << nNodes
                                                     << i;
  }
}

void
tst_InsertNode::insertNode ()
{
  QFETCH (QString, projectFileName);
  QFETCH (int, nNodes);
  QFETCH (int, numPlugins);

  for (int i = 0; i < numPlugins; i++)
  {
    QString pluginName = pluginList.at (i);
    pgControl->loadPlugin (pluginName);
  }

  QVERIFY (pjControl->launchProject (projectFileName));

  MessageControl *messageControl = pgControl->getMessageControl (
      pjControl->getOpenProject (projectFileName));

  Project *project
      = ProjectControl::getInstance ()->getOpenProject (projectFileName);

  QMap<QString, QString> attrs;
  messageControl->anonymousAddEntity ("ncl", project->getUniqueId (), attrs);
  Entity *ncl = project->getEntitiesbyType ("ncl").first ();
  messageControl->anonymousAddEntity ("head", ncl->getUniqueId (), attrs);
  Entity *head = project->getEntitiesbyType ("head").first ();
  messageControl->anonymousAddEntity ("regionBase", head->getUniqueId (),
                                      attrs);
  Entity *regionBase = project->getEntitiesbyType ("regionBase").first ();
  timeval begin, end;
  long long int interval;

  ofstream myfile;
  QString filename = "output/composer_";
  filename += QString::number (numPlugins);
  filename += "_plugins_1.txt";
  myfile.open (filename.toStdString ().c_str ());
  for (int i = 0; i < nNodes; i++)
  {
    attrs.insert ("id", "node");
    gettimeofday (&begin, nullptr);
    messageControl->anonymousAddEntity ("region", regionBase->getUniqueId (),
                                        attrs);

    QApplication::processEvents ();
    showWidgets.redraw ();
    gettimeofday (&end, nullptr);
    interval = TestUtil::timeval_subtract_micro (begin, end);
    myfile << ((double)interval) / 1000.0 << endl;
  }
  myfile.close ();

  //  ProjectControl::getInstance()->saveProject(projectFileName);
  //  QVERIFY(pgControl->releasePlugins(project));
  showWidgets.closeAllWidgets ();
  QVERIFY (pjControl->closeProject (project->getLocation ()));
}

void
tst_InsertNode::insertNodeNPlugins_data ()
{
  insertNode_data ();
}

void
tst_InsertNode::insertNodeNPlugins ()
{
  QFETCH (QString, projectFileName);
  QFETCH (int, nNodes);
  QFETCH (int, numPlugins);

  QList<IPluginFactory *> factories;

  for (int i = 0; i < numPlugins; i++)
  {
    QString pluginName = pluginList.at (i);
    IPluginFactory *fact = pgControl->loadPlugin (pluginName);
    factories.push_back (fact);
  }

  QVERIFY (pjControl->launchProject (projectFileName));

  MessageControl *messageControl = pgControl->getMessageControl (
      pjControl->getOpenProject (projectFileName));
  Project *project
      = ProjectControl::getInstance ()->getOpenProject (projectFileName);

  int nInstances = 0;
  if (factories.size () > 1)
  {
    for (int i = 0; i < nInstances; i++)
    {
      cout << i << endl;
      IPluginFactory *pluginFactory = factories.at (1);
      PluginControl::getInstance ()->launchNewPlugin (pluginFactory, project);
    }
  }

  QMap<QString, QString> attrs;
  messageControl->anonymousAddEntity ("ncl", project->getUniqueId (), attrs);
  Entity *ncl = project->getEntitiesbyType ("ncl").first ();
  messageControl->anonymousAddEntity ("head", ncl->getUniqueId (), attrs);
  Entity *head = project->getEntitiesbyType ("head").first ();
  messageControl->anonymousAddEntity ("regionBase", head->getUniqueId (),
                                      attrs);
  Entity *regionBase = project->getEntitiesbyType ("regionBase").first ();

  messageControl->anonymousAddEntity ("body", ncl->getUniqueId (), attrs);
  Entity *body = project->getEntitiesbyType ("body").first ();
  timeval begin, end;
  long long int interval;

  ofstream myfile;
  QString filename = "output/composer_";
  filename += QString::number (numPlugins);
  filename += "_plugins.txt";
  myfile.open (filename.toStdString ().c_str ());
  for (int i = 0; i < nNodes; i++)
  {
    attrs.insert ("id", "node");
    gettimeofday (&begin, nullptr);
    messageControl->anonymousAddEntity ("region", regionBase->getUniqueId (),
                                        attrs, false, false);

    //    messageControl->anonymousAddEntity("media", body->getUniqueId(),
    //                                       attrs);
    messageControl->anonymousUpdateFromModel ();
    QApplication::processEvents ();
    showWidgets.redraw ();
    gettimeofday (&end, nullptr);
    interval = TestUtil::timeval_subtract_micro (begin, end);
    myfile << ((double)interval) / 1000.0 << endl;
  }
  myfile.close ();

  //  ProjectControl::getInstance()->saveProject(projectFileName);
  //  QVERIFY(pgControl->releasePlugins(project));
  showWidgets.closeAllWidgets ();
  QVERIFY (pjControl->closeProject (project->getLocation ()));
}

void
tst_InsertNode::updateNodes_data ()
{
  QTest::addColumn<QString> ("projectFileName");
  QTest::addColumn<int> ("numPlugins");
  QTest::addColumn<int> ("nNodes");
  QTest::addColumn<int> ("nUpdates");

  QVector<int> nNodes;
  nNodes.push_back (10);
  nNodes.push_back (100);
  nNodes.push_back (500);
  nNodes.push_back (1000);
  //  nNodes.push_back(5000);
  //  nNodes.push_back(10000);

  for (int i = 0; i < nNodes.size (); i++)
  {
    QString testName = "teste" + i;
    QString projectName = QDir::separator ();
    projectName += "tmp";
    projectName += QDir::separator ();
    projectName += "project";
    projectName += ".cpr";

    QTest::newRow (testName.toStdString ().c_str ())
        << projectName << pluginList.size () << nNodes[i] << 100;
  }
}

void
tst_InsertNode::updateNodes ()
{
  QFETCH (QString, projectFileName);
  QFETCH (int, numPlugins);
  QFETCH (int, nNodes);
  QFETCH (int, nUpdates);

  QList<IPluginFactory *> factories;

  for (int i = 0; i < numPlugins; i++)
  {
    QString pluginName = pluginList.at (i);
    IPluginFactory *fact = pgControl->loadPlugin (pluginName);
    factories.push_back (fact);
  }

  QVERIFY (pjControl->launchProject (projectFileName));

  MessageControl *messageControl = pgControl->getMessageControl (
      pjControl->getOpenProject (projectFileName));
  Project *project
      = ProjectControl::getInstance ()->getOpenProject (projectFileName);

  /* Create the original document */
  QMap<QString, QString> attrs;
  messageControl->anonymousAddEntity ("ncl", project->getUniqueId (), attrs);
  Entity *ncl = project->getEntitiesbyType ("ncl").first ();
  messageControl->anonymousAddEntity ("head", ncl->getUniqueId (), attrs);
  Entity *head = project->getEntitiesbyType ("head").first ();
  messageControl->anonymousAddEntity ("regionBase", head->getUniqueId (),
                                      attrs);
  Entity *regionBase = project->getEntitiesbyType ("regionBase").first ();

  messageControl->anonymousAddEntity ("body", ncl->getUniqueId (), attrs);
  Entity *body = project->getEntitiesbyType ("body").first ();
  timeval begin, end;
  long long int interval;

  for (int i = 0; i < nNodes; i++)
  {
    messageControl->anonymousAddEntity ("region", regionBase->getUniqueId (),
                                        attrs);
  }
  QApplication::processEvents ();
  showWidgets.redraw ();

  /* Update nUpdates times the node */
  Entity *region = project->getEntitiesbyType ("region").last ();
  ofstream myfile;
  QString filename = "output/composer_";
  filename += QString::number (numPlugins);
  filename += "_plugins_incremental_";
  filename += QString::number (nNodes);
  filename += "_nodes.txt";
  myfile.open (filename.toStdString ().c_str ());

  attrs.clear ();

  for (int i = 0; i < nUpdates; i++)
  {
    gettimeofday (&begin, nullptr);
    for (int j = 0; j < i; j++)
    {
      QString value = QString::number (j);
      attrs.insert ("top", value);
      messageControl->anonymousChangeEntity (region->getUniqueId (), attrs);
    }
    gettimeofday (&end, nullptr);
    interval = TestUtil::timeval_subtract_micro (begin, end);
    myfile << ((double)interval) / 1000.0 << endl;
  }
  QApplication::processEvents ();
  showWidgets.redraw ();
  myfile.close ();

  filename = "output/composer_";
  filename += QString::number (numPlugins);
  filename += "_plugins_updateFromModel_";
  filename += QString::number (nNodes);
  filename += "_nodes.txt";
  myfile.open (filename.toStdString ().c_str ());
  /* The same thing, but now update only in the end */

  for (int i = 0; i < nUpdates; i++)
  {
    gettimeofday (&begin, nullptr);
    for (int j = 0; j < i; j++)
    {
      QString value = QString::number (i);
      attrs.insert ("top", value);
      messageControl->anonymousChangeEntity (region->getUniqueId (), attrs,
                                             false, false);
    }
    messageControl->anonymousUpdateFromModel ();
    QApplication::processEvents ();
    showWidgets.redraw ();

    gettimeofday (&end, nullptr);
    interval = TestUtil::timeval_subtract_micro (begin, end);
    myfile << ((double)interval) / 1000.0 << endl;
  }

  myfile.close ();

  //  ProjectControl::getInstance()->saveProject(projectFileName);
  //  QVERIFY(pgControl->releasePlugins(project));
  showWidgets.closeAllWidgets ();
  QVERIFY (pjControl->closeProject (project->getLocation ()));
}
