#include "../include/ProjectTreeView.h"

namespace composer {
    namespace gui {

ProjectTreeView::ProjectTreeView(QWidget *parent) : QTreeView(parent)
{
}

void ProjectTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTreeView::mouseDoubleClickEvent(event);
    launchSelectedDocument();
}

void ProjectTreeView::launchSelectedDocument()
{
    QModelIndex current = currentIndex();
    if (current.isValid())
    {
            WorkspaceModel *wsModel = (WorkspaceModel*) model();
            QFileSystemModel *fsModel = (QFileSystemModel*)
                                                    wsModel->sourceModel();
            if (! (fsModel->isDir(wsModel->mapToSource(current))) )
            {
                QString documentPath = fsModel->filePath
                                           (wsModel->mapToSource(current));
                QModelIndex cParent = current.parent();
                while (cParent.isValid() && cParent != rootIndex())
                {
                    current = cParent;
                    cParent = current.parent();
                }
                QString projectId = current.data().toString();
                emit launchDocument(projectId, documentPath);
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
                QAction *launchSelectedDocument = new QAction(
                                        tr("Launch Document"), contextMenu);
                connect(launchSelectedDocument,SIGNAL(triggered()),
                                       SLOT(launchSelectedDocument()));

                contextMenu->addAction(launchSelectedDocument);
                contextMenu->addAction(deleteDocument);
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

void ProjectTreeView::keyReleaseEvent(QKeyEvent *event)
{
    QTreeView::keyReleaseEvent(event);

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        launchSelectedDocument();
}

}} //end namespace
