#ifndef QNSTGRAPHICSBINDDIALOG_H
#define QNSTGRAPHICSBINDDIALOG_H

#include <QDialog>
#include "StructuralConnector.h"

#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QCheckBox>

#include "ui_QnstGraphicsBindForm.h"

class StructuralGraphicsBindDialog : public QDialog
{
public:
  StructuralGraphicsBindDialog(QWidget* parent = 0);

  ~StructuralGraphicsBindDialog();

  void init(QMap<QString, QString> params);

  QMap<QString, QString> getProperties();

public:
  Ui::QnstGraphicsBindDialog form;
};

#endif // QNSTGRAPHICSBINDDIALOG_H
