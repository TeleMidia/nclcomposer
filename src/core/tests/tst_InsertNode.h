/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public
 License along with Foobar.  If not, see <http://www.gnu.org/licenses/>. */

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
