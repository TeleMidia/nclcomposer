/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "ProjectTreeItem.h"

ProjectTreeItem::ProjectTreeItem(QString name, QString location,
                                 QTreeWidget *parent) :
        QTreeWidgetItem(parent, ProjectType)
{
    _name = name;
    _location = location;
    setIcon(0,QIcon(":/mainwindow/folderEmpty"));
    setText(0,_name);
    setToolTip(1,_location);
}
