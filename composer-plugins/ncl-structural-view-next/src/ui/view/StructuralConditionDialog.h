#ifndef QNSTGRAPHICSCONDITIONDIALOG_H
#define QNSTGRAPHICSCONDITIONDIALOG_H

#include <QDialog>
#include <QDebug>

#include "StructuralLink.h"

//#include "ui_QnstGraphicsConditionForm.h"

class StructuralConditionDialog : public QDialog
{
public:
  StructuralConditionDialog(QWidget* _parent = 0);

  ~StructuralConditionDialog();

  void init();

  //void init(QMap<QString, QnstConnector*> connectors, QnstGraphicsLink* links);

public:
//  Ui::QnstGraphicsConditionForm form;

  StructuralLink* link;

  //QMap<QString, QnstConnector*> connectors;
};

#endif // QNSTGRAPHICSCONDITIONDIALOG_H
