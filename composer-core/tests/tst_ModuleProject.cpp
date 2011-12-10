#include "tst_ModuleProject.h"

void tst_ModuleProject::initTestCase()
{
  pjControl = ProjectControl::getInstance();
  resourceDir = "resources";
  resourceDir = resourceDir+QDir::separator()+"documents"+QDir::separator();

  QString baseDir = "..";
  profileDir  = baseDir+QDir::separator()+"extensions"+QDir::separator();
  // +"composer"+QDir::separator();
  lgControl = LanguageControl::getInstance();

#ifdef Q_WS_MAC
  pluginDir = "/Library/Application Support/Composer/";
  //    pluginList << (pluginDir + "libncl_textual_plugin.dylib");
  //    pluginList << (pluginDir + "libdebug_console.dylib");
  //    pluginList << (pluginDir + "liboutline_view.dylib");
  //    pluginList << (pluginDir + "libproperties_view.dylib");
  //    pluginList << (pluginDir + "libQnly.dylib");
  // pluginList << (pluginDir + "libQnst.dylib");
#elif WIN32
  //TODO:
#else
  pluginDir = "/usr/local/lib/composer/extensions/";
  pluginList << ("");
//  pluginList << (pluginDir + "libdebug_console.so");
//  pluginList << (pluginDir + "libproperties_view.so");
//  pluginList << (pluginDir + "libQnly.so");
//  pluginList << (pluginDir + "libQnst.so");
//  pluginList << (pluginDir + "liboutline_view.so");
//  pluginList << (pluginDir + "libncl_textual_plugin.so");
#endif
  pgControl = PluginControl::getInstance();

//  connect(pgControl,
//         SIGNAL(addPluginWidgetToWindow(IPluginFactory*,IPlugin*,Project*,int)),
//         &showWidgets,
//         SLOT(showPluginWidget(IPluginFactory*, IPlugin*, Project*, int)));
}

void tst_ModuleProject::cleanupTestCase()
{
  ProjectControl::releaseInstance();
  pjControl = NULL;
  LanguageControl::releaseInstance();
  lgControl = NULL;
  PluginControl::releaseInstance();
  pgControl = NULL;

  QString projectDir = resourceDir + QDir::separator() + "projects";
  QDir docDir(projectDir);
  docDir.setFilter(QDir::Files | QDir::NoSymLinks);
  QFileInfoList list = docDir.entryInfoList();

  for (int i = 0; i != list.size(); i++)
  {
    QFileInfo fileInfo = list.at(i);
    cout << "Trying to remove " << fileInfo.absoluteFilePath().toStdString();
    QFile file(fileInfo.absoluteFilePath());
    file.remove();
  }
}

void tst_ModuleProject::init()
{
#ifdef Q_WS_MAC
  QString filename = profileDir+QDir::separator()+
      "NCL"+QDir::separator()+"libNCLLanguageProfile.dylib";
#elif WIN32
  //TODO:
#else
  QString filename = profileDir+QDir::separator()
      +"NCL"+QDir::separator()+"libNCLLanguageProfile.so";

  // cout << filename.toStdString();
#endif

  QVERIFY(lgControl->loadProfile(filename));

}

void tst_ModuleProject::cleanup()
{
  lgControl->removeProfile(NCL);
}

void tst_ModuleProject::importFromExistingNCL_data()
{
  QTest::addColumn<QString>("docFileName");
  QTest::addColumn<QString>("projectFileName");
  QTest::addColumn<int>("size");
  QTest::addColumn<int>("numPlugins");

  QDir docDir(resourceDir);
  docDir.setFilter(QDir::Files | QDir::NoSymLinks);
  docDir.setSorting(QDir::Size | QDir::Reversed);

  QFileInfoList list = docDir.entryInfoList();

  for (int k = 1; k <= pluginList.size(); k++)
  {
    for (int i = 0; i != list.size(); i++)
    {
      QFileInfo docFile = list.at(i);
      if (docFile.completeSuffix() != "ncl") continue;
      QString absDocFile = docFile.absoluteFilePath();
      QString absProjFile = docFile.absolutePath()+QDir::separator()+
          "projects"+QDir::separator()+
          docFile.baseName()+".cpr";
      int size = docFile.size();
      QString teste = "teste"+k;
      QTest::newRow(teste.toStdString().c_str())
          << absDocFile << absProjFile << size << k;
      //if(i==1) break;
    }
  }
}

void tst_ModuleProject::importFromExistingNCL()
{
  QFETCH(QString, docFileName);
  QFETCH(QString, projectFileName);
  QFETCH(int, size);
  QFETCH(int, numPlugins);

  for (int i = 0; i < numPlugins ;i++)
  {
    QString pluginName = pluginList.at(i);
    pgControl->loadPlugin(pluginName);
  }

  timeval begin, end;
  // QVERIFY(pjControl->launchProject(projectFileName));
  gettimeofday(&begin,NULL);
  pjControl->importFromDocument(docFileName, projectFileName);
  gettimeofday(&end, NULL);
  long long int interval = timeval_subtract_micro(begin, end);
  cout << numPlugins << " " << size << " " << (double)interval/1000.0 << " ";
  gettimeofday(&begin,NULL);
  pjControl->saveProject(projectFileName);
  gettimeofday(&end,NULL);
  interval = timeval_subtract_micro(begin, end);
  cout << (double)interval / 1000.0 << endl;
  QVERIFY(pjControl->closeProject(projectFileName));
}

void tst_ModuleProject::importNCLForEachPlugin_data()
{
  QTest::addColumn<QString>("docFileName");
  QTest::addColumn<QString>("projectFileName");
  QTest::addColumn<int>("size");
  QTest::addColumn<int>("pluginPos");

  QDir docDir(resourceDir);
  docDir.setFilter(QDir::Files | QDir::NoSymLinks);
  docDir.setSorting(QDir::Size | QDir::Reversed);

  QFileInfoList list = docDir.entryInfoList();

  for (int k = 0; k <= pluginList.size(); k++)
  {
    for (int i = 0; i < list.size(); i++)
    {
      QFileInfo docFile = list.at(i);
      if (docFile.completeSuffix() != "ncl") continue;
      QString absDocFile = docFile.absoluteFilePath();
      QString absProjFile = docFile.absolutePath()+QDir::separator()+
          "projects"+QDir::separator()+
          docFile.baseName()+".cpr";
      int size = docFile.size();
      QString teste = "teste"+k;
      QTest::newRow(teste.toStdString().c_str())
          << absDocFile << absProjFile << size << k;
      //if(i==1) break;
    }
  }
}

void tst_ModuleProject::importNCLForEachPlugin()
{
  QFETCH(QString, docFileName);
  QFETCH(QString, projectFileName);
  QFETCH(int, size);
  QFETCH(int, pluginPos);

  QString pluginName = pluginList.at(pluginPos);
  pgControl->loadPlugin(pluginName);

  timeval begin, end;
  gettimeofday(&begin,NULL);
  pjControl->importFromDocument(docFileName, projectFileName);
  gettimeofday(&end, NULL);
  long long int interval = timeval_subtract_micro(begin, end);
  cout << pluginName.mid(pluginName.lastIndexOf("/")+1).toStdString().c_str()
       << " " << size << " " << (double)interval/1000.0 << " ";

  gettimeofday(&begin,NULL);
  pjControl->saveProject(projectFileName);
  gettimeofday(&end,NULL);
  interval = timeval_subtract_micro(begin, end);
  cout << (double)interval / 1000.0 << endl;
  QVERIFY(pjControl->closeProject(projectFileName));
}

void tst_ModuleProject::insertNode_data()
{
  QTest::addColumn<QString>("projectFileName");
  QTest::addColumn<int>("nNodes");
  QTest::addColumn<int>("numPlugins");

  int nNodes = 1000;

  for(int i = 0; i < pluginList.size(); i++)
  {
    QString testName = "teste" + i;
    QString projectName = QDir::separator();
    projectName += "tmp";
    projectName += QDir::separator();
    projectName += "project";
    projectName += ".cpr";
    QTest::newRow(testName.toStdString().c_str()) << projectName << nNodes << i;
  }
}

void tst_ModuleProject::insertNode()
{
  QFETCH(QString, projectFileName);
  QFETCH(int, nNodes);
  QFETCH(int, numPlugins);

  for (int i = 0; i < numPlugins ;i++)
  {
    QString pluginName = pluginList.at(i);
    pgControl->loadPlugin(pluginName);
  }

  QVERIFY(pjControl->launchProject(projectFileName));
  MessageControl *messageControl =
      pgControl->getMessageControl(projectFileName);
  Project *project = ProjectControl::getInstance()
      ->getOpenProject(projectFileName);

  QMap <QString, QString> attrs;
  messageControl->anonymousAddEntity("ncl", project->getUniqueId(), attrs);
  Entity *ncl = project->getEntitiesbyType("ncl").first();
  messageControl->anonymousAddEntity("body", ncl->getUniqueId(), attrs);
  Entity *body = project->getEntitiesbyType("body").first();
  timeval begin, end;
  long long int interval;
  for(int i = 0; i < nNodes; i++)
  {
    attrs.insert("id", "node");
    gettimeofday(&begin,NULL);
    messageControl->anonymousAddEntity("media", body->getParentUniqueId(),
                                       attrs);
    gettimeofday(&end, NULL);
    interval = timeval_subtract_micro(begin, end);
    cout << numPlugins << " " << i << " " << ((double)interval)/1000.0 << endl;
    showWidgets.redraw();
  }

//  ProjectControl::getInstance()->saveProject(projectFileName);
//  QVERIFY(pgControl->releasePlugins(project));
  QVERIFY(pjControl->closeProject(project->getLocation()));
}
