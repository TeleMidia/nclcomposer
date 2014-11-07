#include "StructuralBindDialog.h"
#include "ui_StructuralBindDialog.h"

StructuralBindDialog::StructuralBindDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::StructuralBindDialog)
{
  ui->setupUi(this);

  ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

StructuralBindDialog::~StructuralBindDialog()
{

  delete ui;
}

void StructuralBindDialog::setType(BT type)
{
  ui->listWidget->clear();

  switch (type) {
    case CONDITION:
      ui->listWidget->addItem("onBegin");
      ui->listWidget->addItem("onEnd");
      ui->listWidget->addItem("onSelection");
      ui->listWidget->addItem("onResume");
      ui->listWidget->addItem("onPause");
      ui->listWidget->addItem("onBeginAttribution");
      ui->listWidget->addItem("onEndAttribution");
      ui->listWidget->addItem("onPauseAttribution");
      ui->listWidget->addItem("onResumeAttribution");
      break;

    case ACTION:
      ui->listWidget->addItem("Start");
      ui->listWidget->addItem("Stop");
      ui->listWidget->addItem("Resume");
      ui->listWidget->addItem("Pause");
      ui->listWidget->addItem("Set");
      break;

    default:
      break;
  }

  ui->listWidget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard).first()->setSelected(true);
}

QString StructuralBindDialog::getSelected()
{
  if (!ui->listWidget->selectedItems().isEmpty())
    return ui->listWidget->selectedItems().first()->text();
  else
    return "";
}
