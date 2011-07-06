#ifndef PROJECTTREEITEMWIDGET_H
#define PROJECTTREEITEMWIDGET_H

#include <QTreeWidgetItem>

/*!
 * \deprecated
 */
class ProjectTreeItem : public QTreeWidgetItem
{
public:
    ProjectTreeItem(QString name, QString location, QTreeWidget *parent = 0);
    enum { ProjectType = QTreeWidgetItem::UserType+1 };
private:
    QString _name;
    QString _location;
};

#endif // PROJECTTREEITEMWIDGET_H
