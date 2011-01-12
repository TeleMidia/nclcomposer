#ifndef PROJECTTREEWIDGET_H
#define PROJECTTREEWIDGET_H

#include <QTreeWidget>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>

class ProjectTreeWidget : public QTreeWidget
{
    Q_OBJECT
private:
    void contextMenuEvent(QContextMenuEvent* event);
    QTreeWidgetItem* item;
public:
    explicit ProjectTreeWidget(QWidget *parent = 0);

signals:
    void newDocument(QString projectLocation);
    void addDocument(QString projectLocation);
    void newProject();

private slots:
    void onAddDocument();
public slots:


};

#endif // PROJECTTREEWIDGET_H
