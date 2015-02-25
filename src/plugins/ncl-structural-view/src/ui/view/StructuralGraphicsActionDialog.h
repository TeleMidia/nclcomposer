#ifndef QNSTGRAPHICSACTIONDIALOG_H
#define QNSTGRAPHICSACTIONDIALOG_H

#include <QDialog>

#include "StructuralConnector.h"
#include "StructuralGraphicsLink.h"

#include "ui_QnstGraphicsActionForm.h"

class StructuralGraphicsActionDialog : public QDialog
{
public:
  StructuralGraphicsActionDialog(QWidget* parent = 0);

  ~StructuralGraphicsActionDialog();

  void init(QMap<QString, StructuralConnector*> connectors, StructuralGraphicsLink* link);

public:
  Ui::QnstGraphicsActionForm form;

  StructuralGraphicsLink* link;

  QMap<QString, StructuralConnector*> connectors;
};

#endif // QNSTGRAPHICSACTIONDIALOG_H
