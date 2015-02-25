/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "../include/WorkspaceModel.h"

namespace composer {
    namespace gui {

WorkspaceModel::WorkspaceModel(QObject *parent) :
        QSortFilterProxyModel(parent)
{
}

bool WorkspaceModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    if(sourceParent.isValid())
    {
        if (sourceParent == topIndex)
        {
            return !(sourceModel()->data(index0).toString().contains("blah"));
        }
    }
    return true;

}

}} //end namespace
