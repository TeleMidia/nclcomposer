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

#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeView>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QFileSystemModel>

#include <core/modules/DocumentControl.h>
using namespace cpr::core;

#include "WorkspaceModel.h"

namespace composer {
    namespace gui {

/*!
 * \deprecated
 */
class ProjectTreeView : public QTreeView
{
    Q_OBJECT
public:
    ProjectTreeView(QWidget *parent = 0);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    void launchDocument(QString);

private slots:
    void launchSelectedDocument();
    //void newFolder();
};

}} //end namespace
#endif // PROJECTTREEVIEW_H
