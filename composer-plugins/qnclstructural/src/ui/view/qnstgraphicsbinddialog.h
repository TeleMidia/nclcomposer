#ifndef QNSTGRAPHICSBINDDIALOG_H
#define QNSTGRAPHICSBINDDIALOG_H

#include <QDialog>
#include "qnstconncetor.h"

#include <QStandardItemModel>
#include <QStandardItem>

#include "ui_QnstGraphicsBindForm.h"

class QnstGraphicsBindDialog : public QDialog
{
public:
    QnstGraphicsBindDialog(QWidget* parent = 0);

    ~QnstGraphicsBindDialog();

    void init(QMap<QString, QString> params);

    QMap<QString, QString> getProperties();

public:
    Ui::QnstGraphicsBindDialog form;
};

#endif // QNSTGRAPHICSBINDDIALOG_H
