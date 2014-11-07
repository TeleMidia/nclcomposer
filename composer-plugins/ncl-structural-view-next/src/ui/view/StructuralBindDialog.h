#ifndef STRUCTURALBINDDIALOG_H
#define STRUCTURALBINDDIALOG_H

#include <QDialog>

namespace Ui {
  class StructuralBindDialog;
}

class StructuralBindDialog : public QDialog
{
    Q_OBJECT

  public:
    enum BT{
      ACTION,
      CONDITION
    };

    explicit StructuralBindDialog(QWidget *parent = 0);
    ~StructuralBindDialog();

    void setType(BT type);

    QString getSelected();

  private:
    Ui::StructuralBindDialog *ui;
};

#endif // STRUCTURALBINDDIALOG_H
