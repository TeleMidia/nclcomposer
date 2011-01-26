#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeView>
#include <QMouseEvent>
#include <QDebug>


class ProjectTreeView : public QTreeView
{
public:
    ProjectTreeView(QWidget *parent = 0);
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // PROJECTTREEVIEW_H
