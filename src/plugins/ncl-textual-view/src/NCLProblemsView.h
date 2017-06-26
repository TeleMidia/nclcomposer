/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef NCLPROBLEMSVIEW_H
#define NCLPROBLEMSVIEW_H

#include <QAction>
#include <QToolBar>

#include <QDockWidget>
#include <QMainWindow>
#include <QTreeWidget>

class NCLProblemsView : public QDockWidget
{
  Q_OBJECT

public:
  explicit NCLProblemsView (QWidget *parent = 0);

private:
  void createViewActions ();
  void createToolBar ();
  void createTreeWidget ();
  void createProblemsList ();

  QMainWindow *_mainWindow;
  QTreeWidget *_problemsList;
  QAction *_clearProblemsAct;

private slots:
  void clearProblems ();

  void addProblem (QString message, QString file, int line, int column = 0,
                   int severity = 0 /*ERROR OR WARNING?*/);
};

#endif
