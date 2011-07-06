#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeView>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QFileSystemModel>

#include <core/modules/DocumentControl.h>
using namespace composer::core;

#include "WorkspaceModel.h"

namespace composer {
    namespace gui {

/*!
 * \deprecated
 */
class ProjectTreeView : public QTreeView
{
    Q_OBJECT
public:
    ProjectTreeView(QWidget *parent = 0);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    void launchDocument(QString);

private slots:
    void launchSelectedDocument();
    //void newFolder();
};

}} //end namespace
#endif // PROJECTTREEVIEW_H
