#ifndef TST_INSERTNODE_H
#define TST_INSERTNODE_H

#include <QObject>
#include <QTest>
#include <QStringList>

/* Composer includes */
#include <modules/PluginControl.h>
#include <modules/ProjectControl.h>
#include <modules/MessageControl.h>
#include <modules/LanguageControl.h>

#include "tst_ModuleProject.h"
#include "tst_Utils.h"

class tst_InsertNode : public QObject
{
  Q_OBJECT

private:
  ProjectControl *pjControl;
  LanguageControl *lgControl;
  PluginControl *pgControl;
  QString resourceDir;
  QStringList pluginList;
  QString profileDir;
  QString pluginDir;
  bool isBenchmark;

  int nNodes;

  ShowWidgets showWidgets;

  void insertNode_data();
  void insertNode();
  void insertNodeNPlugins_data();
  void insertNodeNPlugins();

private slots:
  void initTestCase();
  void cleanupTestCase();
  void init();

  void updateNodes_data();
  void updateNodes();
};

#endif // TST_INSERTNODE_H
