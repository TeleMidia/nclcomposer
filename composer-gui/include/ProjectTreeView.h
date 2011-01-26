#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeView>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QFileSystemModel>

#include "WorkspaceModel.h"


class ProjectTreeView : public QTreeView
{
public:
    ProjectTreeView(QWidget *parent = 0);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // PROJECTTREEVIEW_H
