/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef PROJECTTREEITEMWIDGET_H
#define PROJECTTREEITEMWIDGET_H

#include <QTreeWidgetItem>

/*!
 * \deprecated
 */
class ProjectTreeItem : public QTreeWidgetItem
{
public:
    ProjectTreeItem(QString name, QString location, QTreeWidget *parent = 0);
    enum { ProjectType = QTreeWidgetItem::UserType+1 };
private:
    QString _name;
    QString _location;
};

#endif // PROJECTTREEITEMWIDGET_H
