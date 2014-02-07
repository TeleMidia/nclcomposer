#include "wizardchooser.h"
#include "ui_wizardchooser.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QDomDocument>

WizardChooser::WizardChooser(QWidget *parent, bool modal) :
    QDialog(parent),
    ui(new Ui::wizardchooser)
{
  ui->setupUi(this);
  setModal(modal);

  connect(ui->listWidget,
          SIGNAL(currentTextChanged(QString)),
          this,
          SLOT(changeDescView(QString)));
}

WizardChooser::~WizardChooser()
{
  delete ui;
}

int WizardChooser::exec(const QString &path)
{
  this->path = path;

  QFileInfo fInfo (path);
  if(fInfo.isDir())
  {
    QDir dir = fInfo.dir();
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString wizardName, dir.entryList())
    {
      QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
      item->setIcon(QIcon(":/images/empty-icon"));
      item->setText(wizardName);
      // ui->listWidget->addItem(wizardName);
    }
  }
  return QDialog::exec();
}

QString WizardChooser::getSelectedPath()
{
  QString selectedPath = "";
  if(ui->listWidget->selectedItems().size())
     selectedPath = ui->listWidget->selectedItems().at(0)->text();

  return path + selectedPath;
}

void WizardChooser::changeDescView(QString textItem)
{
  QString selectedPath = path  + textItem;
  selectedPath += selectedPath.mid(selectedPath.lastIndexOf("/")) + ".html";

  QFileInfo finfo(selectedPath);
  if(finfo.exists())
    ui->webView->load(selectedPath);
  else
    ui->webView->setHtml("<html><body>Description not found!!</body></html>");
}
