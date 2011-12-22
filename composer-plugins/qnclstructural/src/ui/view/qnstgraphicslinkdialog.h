#ifndef QNSTGRAPHICSLINKDIALOG_H
#define QNSTGRAPHICSLINKDIALOG_H

#include <QDialog>

#include "ui_QnstGraphicsLinkForm.h"

class QnstGraphicsLinkDialog : public QDialog
{
    Q_OBJECT

public:
    QnstGraphicsLinkDialog(QWidget* parent = 0);

    ~QnstGraphicsLinkDialog();

protected slots:
    void adjustBinds(QString conn);

public:
    Ui::QnstGraphicsLinkForm form;
};

#endif // QNSTGRAPHICSLINKDIALOG_H
