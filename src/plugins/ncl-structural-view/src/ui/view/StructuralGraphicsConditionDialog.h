#ifndef QNSTGRAPHICSCONDITIONDIALOG_H
#define QNSTGRAPHICSCONDITIONDIALOG_H

#include <QDialog>
#include <QDebug>

#include "StructuralConnector.h"
#include "StructuralGraphicsLink.h"

#include "ui_QnstGraphicsConditionForm.h"

class StructuralGraphicsConditionDialog : public QDialog
{
public:
  StructuralGraphicsConditionDialog(QWidget* parent = 0);

  ~StructuralGraphicsConditionDialog();

  void init(QMap<QString, StructuralConnector*> connectors, StructuralGraphicsLink* links);

public:
  Ui::QnstGraphicsConditionForm form;

  StructuralGraphicsLink* link;

  QMap<QString, StructuralConnector*> connectors;
};

#endif // QNSTGRAPHICSCONDITIONDIALOG_H
