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
