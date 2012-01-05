#ifndef QNSTGRAPHICSACTIONDIALOG_H
#define QNSTGRAPHICSACTIONDIALOG_H

#include <QDialog>

#include "qnstconncetor.h"

#include "ui_QnstGraphicsActionForm.h"

class QnstGraphicsActionDialog : public QDialog
{
    Q_OBJECT

public:
    QnstGraphicsActionDialog(QWidget* parent = 0);

    ~QnstGraphicsActionDialog();

    void init(QMap<QString, QnstConncetor*> connectors, QMap<QString, QString> links);

protected slots:
    void adjust(QString connector);

public:
    Ui::QnstGraphicsActionForm form;

    QMap<QString, QString> links;

    QMap<QString, QnstConncetor*> connectors;
};

#endif // QNSTGRAPHICSACTIONDIALOG_H
