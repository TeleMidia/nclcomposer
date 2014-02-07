#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <assert.h>
#include "View.h"

#include "RegionBase.h"
#include "Region.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QnlyMainWindow)
{
  ui->setupUi(this);

  QWidget *separator = new QWidget(this);
  separator->setSizePolicy(QSizePolicy::Expanding,
                           QSizePolicy::Expanding);

  ui->toolBar->addWidget(separator);
  ui->toolBar->addWidget(new QLabel(tr("Region Bases:"), this));
  ui->toolBar->addWidget(ui->regionBaseComboBox);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setQnlyView(View *view)
{
  assert(view != NULL);

  this->view = view;
  this->setCentralWidget(view);

  connect(ui->actionAdd_Region,
          SIGNAL(triggered()),
          this,
          SLOT(addRegion()));

  connect(ui->actionRemove_Region,
          SIGNAL(triggered()),
          this,
          SLOT(removeSelectedRegion()));

  connect(ui->actionShow_Hide_Grid,
               SIGNAL(toggled(bool)),
               view,
               SLOT(setGridVisible(bool)));

  connect(view,
          SIGNAL(gridVisibilityChanged(bool)),
          ui->actionShow_Hide_Grid,
          SLOT(setChecked(bool)));

  connect(ui->regionBaseComboBox,
          SIGNAL(activated(int)),
          this,
          SLOT(comboBoxChangedCurrentRegionBase(int)));
}

void MainWindow::addRegion()
{
  Region *region = view->getSelectedRegion();
  if(region == NULL)
  {
    RegionBase *regionBase = view->getSelectedRegionBase();
    if(regionBase != NULL)
    {
      regionBase->performRegion();
    }
  }
  else
    region->performRegion();
}

void MainWindow::removeSelectedRegion()
{
  Region *region = view->getSelectedRegion();
  if(region != NULL)
  {
    region->performDelete();
  }
}

void MainWindow::addRegionBaseToCombobox(
                const QString &uuid,
                const QMap<QString,QString> &properties )
{
  int comboCount = ui->regionBaseComboBox->count();

  if(properties.count("id"))
    ui->regionBaseComboBox->insertItem(comboCount - 2, properties["id"], uuid);
  else
    ui->regionBaseComboBox->insertItem(comboCount - 2, tr("Unknown"), uuid);
}

void MainWindow::removeRegionBaseFromCombobox(const QString &uuid)
{
  for(int i = 0; i < ui->regionBaseComboBox->count(); i++)
  {
    if(ui->regionBaseComboBox->itemData(i).toString() == uuid)
    {
      ui->regionBaseComboBox->removeItem(i);
      break;
    }
  }
}

void MainWindow::comboBoxChangedCurrentRegionBase(int pos)
{
  // \fixme Handle when the user clicks the separator
  // means that we want to create a new one
  if(pos == ui->regionBaseComboBox->count() - 1)
  {
    emit createNewRegionBase("test", QMap <QString, QString>());
  }
  else
  {
    QString uuid = ui->regionBaseComboBox->itemData(pos).toString();
    emit regionBaseSelectedFromComboBox(uuid);
  }
}

void MainWindow::selectRegionBaseInComboBox(const QString &uuid)
{
  int index = ui->regionBaseComboBox->findData(uuid);
  if(index != -1)
    ui->regionBaseComboBox->setCurrentIndex(index);
}

void MainWindow::updateRegionBaseInComboBox(
     const QString &uuid,
     const QMap<QString,QString> &properties )
{
  int index = ui->regionBaseComboBox->findData(uuid);
  if(index != -1)
  {
    QString newid = "Unknown";

    if(properties.count("id"))
      newid = properties["id"];

    ui->regionBaseComboBox->setItemText(index, newid);
  }
}

void MainWindow::on_actionAction_Export_triggered()
{
  view->snapshot();
}
