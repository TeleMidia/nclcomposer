#include "../../include/gui/ProjectTreeWidget.h"

ProjectTreeWidget::ProjectTreeWidget(QWidget *parent) :
    QTreeWidget(parent) {

}

void ProjectTreeWidget::contextMenuEvent(QContextMenuEvent* event)
{
        item = itemAt(event->pos());
        /* Hit an item in the project tree */
        QMenu contextMenu(this);
        if (item) {
                QAction *newDocument = new QAction(tr("New Document ..."),
                                                   this);
                newDocument->setToolTip(tr("Create a new Document"));
                contextMenu.addAction(newDocument);
                QAction *addDocument = new QAction(tr("Add document ..."),
                                                   this);
                addDocument->setToolTip(tr("Add existing document to project"));
                contextMenu.addAction(addDocument);
                connect(newDocument, SIGNAL(triggered()), this,
                        SLOT(onNewDocument()));
                connect(addDocument,SIGNAL(triggered()),this,
                        SLOT(onAddDocument()));

            } else { /* it is not a project show new project option */
                QAction *newProject = new QAction(tr("New Project ..."),
                                                   this);
                newProject->setToolTip(tr("Create a new project"));
                contextMenu.addAction(newProject);
                connect(newProject,SIGNAL(triggered()),this,
                        SIGNAL(newProject()));

            }
            contextMenu.exec(event->globalPos());
}

void ProjectTreeWidget::onNewDocument() {
    emit newDocument(item->toolTip(1)); //pass the project location
}

void ProjectTreeWidget::onAddDocument() {
    emit addDocument(item->text(1)); //pass the projectId
}
