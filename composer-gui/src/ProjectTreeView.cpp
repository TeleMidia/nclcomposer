#include "../include/ProjectTreeView.h"

ProjectTreeView::ProjectTreeView(QWidget *parent) : QTreeView(parent)
{
}

void ProjectTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());

    if (index.isValid())
    {
            qDebug() << "CLICK: " << index.data().toString();
    }
}
