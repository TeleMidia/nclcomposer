/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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
#include "NCLProblemsView.h"

NCLProblemsView::NCLProblemsView (QWidget *parent)
    : QDockWidget ("Problems", parent)
{
  _mainWindow = new QMainWindow (parent);

  createViewActions ();
  createToolBar ();
  createTreeWidget ();
}

void
NCLProblemsView::createViewActions ()
{
  _clearProblemsAct = new QAction (QIcon (":/images/clear-icon.png"),
                                   tr ("&Clear Problems"), this);

  _clearProblemsAct->setStatusTip (tr ("Clear All"));
  connect (_clearProblemsAct, SIGNAL (triggered ()), this,
           SLOT (clearProblems ()));
}

void
NCLProblemsView::createToolBar ()
{
  QToolBar *file = _mainWindow->addToolBar (tr ("File"));
  file->addAction (_clearProblemsAct);
  file->setIconSize (QSize (16, 16));
}

void
NCLProblemsView::createTreeWidget ()
{
  QStringList labels;
  labels << QObject::tr ("Description") << QObject::tr ("file")
         << QObject::tr ("line");

  _problemsList = new QTreeWidget (this);
  _problemsList->setSortingEnabled (1);
  _problemsList->setStyleSheet (/*"background-color:*/ "font-size: 11px;");
  _problemsList->setHeaderLabels (labels);

  QTreeWidgetItem *lst1 = new QTreeWidgetItem (_problemsList);
  lst1->setIcon (0, QIcon (":/images/error-icon-16.png"));
  lst1->setText (0, "Error!");
  lst1->setText (1, "0");

  setFeatures (QDockWidget::DockWidgetMovable
               | QDockWidget::DockWidgetFloatable);

  _mainWindow->setCentralWidget (_problemsList);

  setWidget (_mainWindow);
}

void
NCLProblemsView::clearProblems ()
{
  _problemsList->clear ();
}

// TODO: ERROR OR WARNING ??
void
NCLProblemsView::addProblem (QString message, QString file, int line,
                             int column, int severity /*ERROR OR WARNING?*/)
{
  Q_UNUSED (column)

  QTreeWidgetItem *error = new QTreeWidgetItem (_problemsList);
  if (severity == 0)
    error->setIcon (0, QIcon (":/images/error-icon-16.png"));
  else
    error->setIcon (0, QIcon (":/images/warning-big.png"));

  error->setText (0, message);
  error->setText (1, file);
  error->setText (2, QString::number (line));
}
