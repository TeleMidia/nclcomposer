#ifndef QNSTGRAPHICSACTIONDIALOG_H
#define QNSTGRAPHICSACTIONDIALOG_H

#include <QDialog>

#include "qnstconncetor.h"
#include "qnstlink.h"

#include "ui_QnstGraphicsActionForm.h"

class QnstGraphicsActionDialog : public QDialog
{
public:
    QnstGraphicsActionDialog(QWidget* parent = 0);

    ~QnstGraphicsActionDialog();

    void init(QMap<QString, QnstConncetor*> connectors, QnstLink* link);

public:
    Ui::QnstGraphicsActionForm form;

    QnstLink* link;

    QMap<QString, QnstConncetor*> connectors;
};

#endif // QNSTGRAPHICSACTIONDIALOG_H
