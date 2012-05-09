#include "include/NewProjectWizard.h"
#include "ui_NewProjectWizard.h"

#include <QFileDialog>
#include <QDir>

NewProjectWizard::NewProjectWizard(QWidget *parent) :
  QWizard(parent),
  ui(new Ui::NewProjectWizard)
{
  ui->setupUi(this);

  connect(ui->pushButton_Browser, SIGNAL(pressed()),
          this, SLOT(browseDir()));

  ui->lineEdit_CreateIn->setText(QDir::homePath());
}

NewProjectWizard::~NewProjectWizard()
{
  delete ui;
}

void NewProjectWizard::browseDir()
{
  QString path = QFileDialog::getExistingDirectory(
        this,
        tr("New Project Location"),
        ui->lineEdit_CreateIn->text());

  if(!path.isNull() && !path.isEmpty())
    ui->lineEdit_CreateIn->setText(path);
}

QString NewProjectWizard::getProjectFullPath()
{
  QString dir = ui->lineEdit_CreateIn->text();

  return dir + "/" + ui->lineEdit_Name->text();
}

bool NewProjectWizard::shouldCopyDefaultConnBase()
{
  return ui->checkBox_ImportConnectorBase->isChecked();
}
