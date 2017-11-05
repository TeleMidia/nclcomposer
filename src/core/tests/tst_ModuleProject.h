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

#ifndef TST_MODULEDOCUMENT_H
#define TST_MODULEDOCUMENT_H

#include <iostream>
#include <sys/time.h>
using namespace std;

#include <QMainWindow>
#include <QObject>
#include <QStringList>
#include <QTest>

#include <modules/LanguageControl.h>
#include <modules/MessageControl.h>
#include <modules/PluginControl.h>
#include <modules/ProjectControl.h>

using namespace composer::core;

class ShowWidgets : public QObject
{
  Q_OBJECT

public:
  ShowWidgets () {}

public slots:
  void
  showPluginWidget (IPluginFactory *factory, IPlugin *plugin, Project *project,
                    int i)
  {
    (void *)factory;
    (void *)project;
    (void *)i;

    plugin->getWidget ()->show ();
    widgets.push_back (plugin->getWidget ());
    QTest::qWaitForWindowShown (plugin->getWidget ());
  }

  void
  redraw ()
  {
    for (int i = 0; i < widgets.size (); i++)
      widgets.at (i)->repaint ();
  }

  void
  closeAllWidgets ()
  {
    widgets.clear ();
    QApplication::closeAllWindows ();
  }

private:
  QList<QWidget *> widgets;
};

class tst_ModuleProject : public QObject
{
  Q_OBJECT

public:
  void
  setBenchmark (bool isBench)
  {
    isBenchmark = isBench;
  }

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

  void importNCLForEachPlugin ();
  void importNCLForEachPlugin_data ();

private slots:
  void initTestCase ();
  void init ();
  void cleanup ();
  void cleanupTestCase ();

  void importFromExistingNCL_data ();
  void importFromExistingNCL ();
};

#endif // TST_MODULEDOCUMENT_H
