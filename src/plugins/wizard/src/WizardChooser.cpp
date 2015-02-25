#include "WizardChooser.h"
#include "ui_wizardchooser.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QDomDocument>

WizardChooser::WizardChooser(QWidget *parent, bool modal) :
  QDialog(parent),
  _ui(new Ui::wizardchooser)
{
  _ui->setupUi(this);
  setModal(modal);

  connect(_ui->listWidget,
          SIGNAL(currentTextChanged(QString)),
          this,
          SLOT(changeDescView(QString)));
}

WizardChooser::~WizardChooser()
{
  delete _ui;
}

int WizardChooser::exec(const QString &path)
{
  _path = path;

  QFileInfo fInfo (path);
  if(fInfo.isDir())
  {
    QDir dir = fInfo.dir();
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QString wizardName, dir.entryList())
    {
      QListWidgetItem *item = new QListWidgetItem(_ui->listWidget);
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
  if(_ui->listWidget->selectedItems().size())
    selectedPath = _ui->listWidget->selectedItems().at(0)->text();

  return _path + selectedPath;
}

void WizardChooser::changeDescView(const QString &textItem)
{
  QString selectedPath = _path  + textItem;
  selectedPath += selectedPath.mid(selectedPath.lastIndexOf("/")) + ".html";

  QFileInfo finfo(selectedPath);
  if(finfo.exists())
    _ui->webView->load(selectedPath);
  else
    _ui->webView->setHtml("<html><body>Description not found!!</body></html>");
}
