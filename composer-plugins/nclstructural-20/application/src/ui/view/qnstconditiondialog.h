#ifndef QNSTGRAPHICSCONDITIONDIALOG_H
#define QNSTGRAPHICSCONDITIONDIALOG_H

#include <QDialog>
#include <QDebug>

#include "qnstlink.h"

#include "ui_QnstGraphicsConditionForm.h"

class QnstConditionDialog : public QDialog
{
public:
  QnstConditionDialog(QWidget* parent = 0);

  ~QnstConditionDialog();

  void init();

  //void init(QMap<QString, QnstConnector*> connectors, QnstGraphicsLink* links);

public:
  Ui::QnstGraphicsConditionForm form;

  QnstLink* link;

  //QMap<QString, QnstConnector*> connectors;
};

#endif // QNSTGRAPHICSCONDITIONDIALOG_H
