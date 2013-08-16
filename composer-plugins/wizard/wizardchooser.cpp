#include "wizardchooser.h"
#include "ui_wizardchooser.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>

wizardchooser::wizardchooser(QWidget *parent, bool modal) :
    QDialog(parent),
    ui(new Ui::wizardchooser)
{
  ui->setupUi(this);
  setModal(modal);
}

wizardchooser::~wizardchooser()
{
  delete ui;
}

int wizardchooser::exec(const QString &path)
{
  this->path = path;

  QFileInfo fInfo (path);
  if(fInfo.isDir())
  {
    QDir dir = fInfo.dir();
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString wizardName, dir.entryList())
    {
      ui->listWidget->addItem(wizardName);
    }
  }
  return QDialog::exec();
}

QString wizardchooser::getSelectedPath()
{
  QString selectedPath = ui->listWidget->selectedItems().at(0)->text();

  return path + selectedPath;
}
