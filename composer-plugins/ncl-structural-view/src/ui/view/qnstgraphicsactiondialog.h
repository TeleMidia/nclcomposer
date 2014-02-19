#ifndef QNSTGRAPHICSACTIONDIALOG_H
#define QNSTGRAPHICSACTIONDIALOG_H

#include <QDialog>

#include "qnstconnector.h"
#include "qnstgraphicslink.h"

#include "ui_QnstGraphicsActionForm.h"

class QnstGraphicsActionDialog : public QDialog
{
public:
  QnstGraphicsActionDialog(QWidget* parent = 0);

  ~QnstGraphicsActionDialog();

  void init(QMap<QString, QnstConnector*> connectors, QnstGraphicsLink* link);

public:
  Ui::QnstGraphicsActionForm form;

  QnstGraphicsLink* link;

  QMap<QString, QnstConnector*> connectors;
};

#endif // QNSTGRAPHICSACTIONDIALOG_H
