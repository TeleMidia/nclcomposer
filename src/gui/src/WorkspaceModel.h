/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
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
