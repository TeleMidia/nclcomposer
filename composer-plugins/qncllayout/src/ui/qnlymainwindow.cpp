#include "qnlymainwindow.h"
#include "ui_qnlymainwindow.h"

#include <assert.h>
#include "ui/view/qnlyview.h"

#include "ui/view/qnlygraphicsregionbase.h"
#include "ui/view/qnlygraphicsregion.h"


QnlyMainWindow::QnlyMainWindow(QWidget *parent) :
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

QnlyMainWindow::~QnlyMainWindow()
{
  delete ui;
}

void QnlyMainWindow::setQnlyView(QnlyView *view)
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

void QnlyMainWindow::addRegion()
{
  QnlyGraphicsRegion *region = view->getSelectedRegion();
  if(region == NULL)
  {
    QnlyGraphicsRegionBase *regionBase = view->getSelectedRegionBase();
    if(regionBase != NULL)
    {
      regionBase->performRegion();
    }
  }
  else
    region->performRegion();
}

void QnlyMainWindow::removeSelectedRegion()
{
  QnlyGraphicsRegion *region = view->getSelectedRegion();
  if(region != NULL)
  {
    region->performDelete();
  }
}

void QnlyMainWindow::addRegionBaseToCombobox(const QString &uuid, const QMap<QString,QString> &properties)
{
  if(properties.count("id"))
    ui->regionBaseComboBox->addItem(properties["id"], uuid);
  else
    ui->regionBaseComboBox->addItem(tr("Unknown"), uuid);
}

void QnlyMainWindow::removeRegionBaseFromCombobox(const QString &uuid)
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

void QnlyMainWindow::comboBoxChangedCurrentRegionBase(int pos)
{
  QString uuid = ui->regionBaseComboBox->itemData(pos).toString();

  emit regionBaseSelectedFromComboBox(uuid);
}

void QnlyMainWindow::selectRegionBaseInComboBox(const QString &uuid)
{
  int index = ui->regionBaseComboBox->findData(uuid);
  if(index != -1)
    ui->regionBaseComboBox->setCurrentIndex(index);
}

void QnlyMainWindow::updateRegionBaseInComboBox(const QString &uuid,
                                                const QMap<QString,QString> &properties)
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

void QnlyMainWindow::on_actionAction_Export_triggered()
{
  view->snapshot();
}
