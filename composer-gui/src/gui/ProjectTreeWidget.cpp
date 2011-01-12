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
                QAction *addDocument = new QAction(tr("Add document ..."),
                                                   this);
                addDocument->setToolTip(tr("Add a document to project"));
                contextMenu.addAction(addDocument);
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

void ProjectTreeWidget::onAddDocument() {
    emit addDocument(item->text(1)); //pass the projectId
}
