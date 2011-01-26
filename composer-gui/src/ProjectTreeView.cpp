#include "../include/ProjectTreeView.h"

ProjectTreeView::ProjectTreeView(QWidget *parent) : QTreeView(parent)
{
}

void ProjectTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTreeView::mouseDoubleClickEvent(event);
    QModelIndex current = indexAt(event->pos());

    if (current.isValid())
    {
            qDebug() << "CLICK: " << current.data().toString();

            WorkspaceModel *wsModel = (WorkspaceModel*) model();
            QFileSystemModel *fsModel = (QFileSystemModel*)
                                                    wsModel->sourceModel();

            QString documentPath = fsModel->filePath
                                   (wsModel->mapToSource(current));

            if (! (fsModel->isDir(wsModel->mapToSource(current))) )
            {
                QModelIndex cParent = current.parent();
                while (cParent.isValid() && cParent != rootIndex())
                {
                    current = cParent;
                    cParent = current.parent();
                }
                qDebug() << "FileName: " << documentPath << "PROJECTID: "
                         << current.data().toString();
            }
    }
}

void ProjectTreeView::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeView::mouseReleaseEvent(event);
    if (event->button() == Qt::RightButton)
    {
        QMenu *contextMenu = new QMenu(this);
        QModelIndex current = indexAt(event->pos());
        if (current.isValid())
        {
            WorkspaceModel *wsModel = (WorkspaceModel*)model();
            QFileSystemModel *fsModel = (QFileSystemModel*)
                                                    wsModel->sourceModel();
            if (fsModel->isDir(wsModel->mapToSource(current)))
            {
                QAction *newDocumentAct = new QAction(
                                        QIcon(":/mainwindow/newDocument"),
                                        tr("New document"), contextMenu);
                QAction *newFolderAct = new QAction(
                                        QIcon(":/mainwindow/newFolder"),
                                        tr("New folder"), contextMenu);
                contextMenu->addAction(newDocumentAct);
                contextMenu->addAction(newFolderAct);
            } else {
                QAction *deleteDocument = new QAction(
                                        QIcon(":/mainwindow/deleteDoc"),
                                        tr("Delete Document"), contextMenu);
                QAction *launchDocument = new QAction(
                                        tr("Launch Document"), contextMenu);
                contextMenu->addAction(launchDocument);
                contextMenu->addAction(deleteDocument);
                qDebug() << "Launch Document " << current.data().toString();
            }
        } else {
            QAction *newProject = new QAction(
                                    QIcon(":/mainwindow/openProject"),
                                    tr("New Project"), contextMenu);
            contextMenu->addAction(newProject);
        }
        contextMenu->exec(event->globalPos());
    }
}
