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

};

#endif
