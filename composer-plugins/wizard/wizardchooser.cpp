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

  connect(ui->listWidget,
          SIGNAL(currentTextChanged(QString)),
          this,
          SLOT(changeDescView(QString)));
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
  QString selectedPath = "";
  if(ui->listWidget->selectedItems().size())
     selectedPath = ui->listWidget->selectedItems().at(0)->text();

  return path + selectedPath;
}

void wizardchooser::changeDescView(QString textItem)
{
  qDebug() << textItem;
  QString selectedPath = path  + textItem;
  selectedPath += selectedPath.mid(selectedPath.lastIndexOf("/")) + ".html";

  QFileInfo finfo(selectedPath);
  if(finfo.exists())
    ui->webView->load(selectedPath);
  else
    ui->webView->setHtml("<html><body>Description not found!!</body></html>");
}
