#include "ProjectTreeItem.h"

ProjectTreeItem::ProjectTreeItem(QString name, QString location,
                                 QTreeWidget *parent) :
        QTreeWidgetItem(parent, ProjectType)
{
    _name = name;
    _location = location;
    setIcon(0,QIcon(":/mainwindow/folderEmpty"));
    setText(0,_name);
    setToolTip(1,_location);
}
