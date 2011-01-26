#include "../include/WorkspaceModel.h"

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
