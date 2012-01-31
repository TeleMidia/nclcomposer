#ifndef QNSTGRAPHICSCONDITIONDIALOG_H
#define QNSTGRAPHICSCONDITIONDIALOG_H

#include <QDialog>
#include <QDebug>

#include "qnstconncetor.h"
#include "qnstlink.h"

#include "ui_QnstGraphicsConditionForm.h"

class QnstGraphicsConditionDialog : public QDialog
{
public:
    QnstGraphicsConditionDialog(QWidget* parent = 0);

    ~QnstGraphicsConditionDialog();

    void init(QMap<QString, QnstConncetor*> connectors, QnstLink* links);

public:
    Ui::QnstGraphicsConditionForm form;

    QnstLink* link;

    QMap<QString, QnstConncetor*> connectors;
};

#endif // QNSTGRAPHICSCONDITIONDIALOG_H
