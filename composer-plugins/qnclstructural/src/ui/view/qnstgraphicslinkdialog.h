#ifndef QNSTGRAPHICSLINKDIALOG_H
#define QNSTGRAPHICSLINKDIALOG_H

#include <QDialog>

#include  "qnstconncetor.h"

#include "ui_QnstGraphicsLinkForm.h"

class QnstGraphicsLinkDialog : public QDialog
{
    Q_OBJECT

public:
    QnstGraphicsLinkDialog(QWidget* parent = 0);

    ~QnstGraphicsLinkDialog();

    void init(QMap<QString, QnstConncetor*> connectors);

protected slots:
    void adjustBinds(QString conn);

public:
    Ui::QnstGraphicsLinkForm form;

    QMap<QString, QnstConncetor*> connectors;
};

#endif // QNSTGRAPHICSLINKDIALOG_H
