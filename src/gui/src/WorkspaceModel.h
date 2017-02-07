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

#ifndef WORKSPACEMODEL_H
#define WORKSPACEMODEL_H

#include <QSortFilterProxyModel>
#include <QDebug>

namespace composer {
    namespace gui {

/*!
 * \deprecated
 */
class WorkspaceModel : public QSortFilterProxyModel
{
private:
    QModelIndex topIndex;
public:
    WorkspaceModel(QObject *parent = 0);
    /* RootIndex from sourceModel() */
    inline void setTopIndex(QModelIndex index) { topIndex =index;}
    inline QModelIndex getTopIndex() {return topIndex;}

protected:
     bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

};

}} //end namespace

#endif // WORKSPACEMODEL_H
