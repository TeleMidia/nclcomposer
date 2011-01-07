#ifndef NCLTREEWIDGET_H
#define NCLTREEWIDGET_H

#include <QDebug>
#include <qxml.h>
#include <QHeaderView>
#include <QTreeWidget>

#include "NCLParser.h"

class NCLTreeWidget: public QTreeWidget {
    Q_OBJECT

public:
    NCLTreeWidget(QWidget *parent);
    virtual ~NCLTreeWidget();

    bool updateFromText(QString text);

    //! Add an element as child of father and return this element
    QTreeWidgetItem* addElement ( QTreeWidgetItem *father,
                                  int pos,
                                  QString tagname,
                                  QString id = "",
                                  int line_in_text = -1,
                                  int column_in_text = -1);

};

#endif
