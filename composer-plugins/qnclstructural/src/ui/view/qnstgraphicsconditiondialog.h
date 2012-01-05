#ifndef QNSTGRAPHICSCONDITIONDIALOG_H
#define QNSTGRAPHICSCONDITIONDIALOG_H

#include <QDialog>
#include <QDebug>

#include "qnstconncetor.h"

#include "ui_QnstGraphicsConditionForm.h"

class QnstGraphicsConditionDialog : public QDialog
{
    Q_OBJECT

public:
    QnstGraphicsConditionDialog(QWidget* parent = 0);

    ~QnstGraphicsConditionDialog();

    void init(QMap<QString, QnstConncetor*> connectors, QMap<QString, QString> links);

protected slots:
    void adjust(QString connector);

public:
    Ui::QnstGraphicsConditionForm form;

    QMap<QString, QString> links;

    QMap<QString, QnstConncetor*> connectors;
};

#endif // QNSTGRAPHICSCONDITIONDIALOG_H
