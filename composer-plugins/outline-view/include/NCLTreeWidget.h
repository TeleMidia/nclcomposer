#ifndef NCLTREEWIDGET_H
#define NCLTREEWIDGET_H

#include <map>
using namespace std;

#include <QDebug>
#include <qxml.h>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QTreeWidget>
#include <QShortcut>
#include <QKeyEvent>
#include <QWheelEvent>

#include "NCLParser.h"

class NCLTreeWidget: public QTreeWidget {
    Q_OBJECT

private:
    QAction *insertNodeAct;
    QAction *removeNodeAct;
    QAction *expandAllAct;
    QMenu   *elementMenu;

    QShortcut *shortcut_zoomout;
    QShortcut *shortcut_zoomin;

    void createActions();
    void createMenus();

    /* User events */
    void wheelEvent(QWheelEvent * event);
    void keyPressEvent(QKeyEvent *event);

public:
    NCLTreeWidget(QWidget *parent);
    virtual ~NCLTreeWidget();

    bool updateFromText(QString text);

    //! Add an element as child of father and return this element
    QTreeWidgetItem* addElement ( QTreeWidgetItem *father,
                                  int pos,
                                  QString tagname,
                                  QString id,
                                  QMap <QString, QString> &attrs,
                                  int line_in_text = -1,
                                  int column_in_text = -1);

    QTreeWidgetItem* getItemById(QString itemId);
    void removeItem(QString itemId);

public slots:
    void errorNotification( QString message,
                            QString filename,
                            int line,
                            int column,
                            int severity);

private slots:
    void userAddNewElement();
    void userRemoveElement();
    void DecreaseFont();
    void IncreaseFont();

signals:
    void elementAddedByUser(QString, QString, QMap<QString,QString>&, bool);
    void elementRemovedByUser (QString);
    void parserErrorNotify( QString message,
                            QString filename,
                            int line,
                            int column,
                            int severity);

};

#endif
