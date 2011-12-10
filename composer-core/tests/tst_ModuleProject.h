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

class ShowWidgets : public QObject {
  Q_OBJECT
  QList <QWidget *> widgets;

public:
  ShowWidgets(){};

public slots:
  void showPluginWidget(IPluginFactory* factory,
                        IPlugin* plugin, Project *project, int i)
  {
    plugin->getWidget()->show();
    widgets.push_back(plugin->getWidget());
    QTest::qWaitForWindowShown(plugin->getWidget());
  };

  void redraw() {
    for(int i = 0; i < widgets.size(); i++)
      widgets.at(i)->repaint();
  }

};

class tst_ModuleProject : public QObject
{
  Q_OBJECT
  ShowWidgets showWidgets;

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

private slots:
  void initTestCase();
  void init();
  void cleanup();
  void importNCLForEachPlugin();
  void importNCLForEachPlugin_data();

  //    void loadPluginDir();
  //    void pluginName_data();
  //    void pluginName();
  //    void pluginLoadBenchmark_data();
  //    void pluginLoadBenchmark();

  void cleanupTestCase();

private:
  void importFromExistingNCL_data();
  void importFromExistingNCL();
  void insertNode_data();
  void insertNode();

  /* This function doesn't seem correct, using timeval_subtract insteads */
  timeval diff(timeval start, timeval end)
  {
    timeval temp;

    if ((end.tv_usec-start.tv_usec)<0) {
      temp.tv_sec = end.tv_sec-start.tv_sec-1;
      temp.tv_usec = 1000000+end.tv_usec-start.tv_usec;
    } else {
      temp.tv_sec = end.tv_sec-start.tv_sec;
      temp.tv_usec = end.tv_usec-start.tv_usec;
    }
    return temp;
  }


  long long int timeval_subtract_micro (struct timeval start,
                                        struct timeval stop)
  {
    long long int result = ((long long int)(stop.tv_sec - start.tv_sec))*1000000;
    result += stop.tv_usec - start.tv_usec;

    return result;
  }
};

#endif // TST_MODULEDOCUMENT_H
