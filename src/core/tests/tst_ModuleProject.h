#ifndef TST_MODULEDOCUMENT_H
#define TST_MODULEDOCUMENT_H

#include <iostream>
#include <sys/time.h>
using namespace std;

#include <QObject>
#include <QTest>
#include <QStringList>
#include <QMainWindow>

#include <modules/PluginControl.h>
#include <modules/ProjectControl.h>
#include <modules/MessageControl.h>
#include <modules/LanguageControl.h>

using namespace composer::core;

class ShowWidgets : public QObject
{
  Q_OBJECT

public:
  ShowWidgets(){}

public slots:
  void showPluginWidget(IPluginFactory* factory,
                        IPlugin* plugin, Project *project, int i)
  {
    (void *) factory;
    (void *) project;
    (void *) i;

    plugin->getWidget()->show();
    widgets.push_back(plugin->getWidget());
    QTest::qWaitForWindowShown(plugin->getWidget());
  }

  void redraw() {
    for(int i = 0; i < widgets.size(); i++)
      widgets.at(i)->repaint();
  }

  void closeAllWidgets()
  {
    widgets.clear();
    QApplication::closeAllWindows();
  }

private:
  QList <QWidget *> widgets;
};

class tst_ModuleProject : public QObject
{
  Q_OBJECT

public:
  void setBenchmark(bool isBench) { isBenchmark = isBench; }

private:
  ProjectControl *pjControl;
  LanguageControl *lgControl;
  PluginControl *pgControl;
  QString resourceDir;
  QStringList pluginList;
  QString profileDir;
  QString pluginDir;
  bool isBenchmark;

  ShowWidgets showWidgets;

  void importNCLForEachPlugin();
  void importNCLForEachPlugin_data();

private slots:
  void initTestCase();
  void init();
  void cleanup();
  void cleanupTestCase();

  void importFromExistingNCL_data();
  void importFromExistingNCL();
};

#endif // TST_MODULEDOCUMENT_H
