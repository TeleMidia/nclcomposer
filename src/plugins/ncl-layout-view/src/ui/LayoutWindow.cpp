#include "LayoutWindow.h"
#include "ui_LayoutWindow.h"

#include <assert.h>
#include <QRegExp>
#include "LayoutView.h"

#include "LayoutRegionBase.h"
#include "LayoutRegion.h"

LayoutWindow::LayoutWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QnlyMainWindow)
{
  ui->setupUi(this);

  ui->toolBar_regionBases->addWidget(new QLabel(tr("Region Bases:"), this));
  ui->toolBar_regionBases->addWidget(ui->regionBaseComboBox);
  ui->toolBar_regionBases->addWidget(new QLabel(tr("Resolution:"), this));
  ui->toolBar_regionBases->addWidget(ui->resolutionComboBox);
}

LayoutWindow::~LayoutWindow()
{
  delete ui;
}

void LayoutWindow::setQnlyView(LayoutView *view)
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

void LayoutWindow::addRegion()
{
  LayoutRegion *region = view->getSelectedRegion();
  if(region == NULL)
  {
    LayoutRegionBase *regionBase = view->getSelectedRegionBase();
    if(regionBase != NULL)
    {
      regionBase->performRegion();
    }
  }
  else
    region->performRegion();
}

void LayoutWindow::removeSelectedRegion()
{
  LayoutRegion *region = view->getSelectedRegion();
  if(region != NULL)
  {
    region->performDelete();
  }
}

void LayoutWindow::addRegionBaseToCombobox(
    const QString &uuid,
    const QMap<QString,QString> &properties )
{
  int comboCount = ui->regionBaseComboBox->count();

  if(properties.count("id"))
    ui->regionBaseComboBox->insertItem(comboCount - 2, QIcon(":/icon/regionbase"), properties["id"], uuid);
  else
    ui->regionBaseComboBox->insertItem(comboCount - 2, QIcon(":/icon/regionbase"), tr("Unknown"), uuid);
}

void LayoutWindow::removeRegionBaseFromCombobox(const QString &uuid)
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

void LayoutWindow::comboBoxChangedCurrentRegionBase(int pos)
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

void LayoutWindow::selectRegionBaseInComboBox(const QString &uuid)
{
  int index = ui->regionBaseComboBox->findData(uuid);
  if(index != -1)
    ui->regionBaseComboBox->setCurrentIndex(index);
}

void LayoutWindow::updateRegionBaseInComboBox(
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

void LayoutWindow::on_actionAction_Export_triggered()
{
  view->doSnapshot();
}

void LayoutWindow::on_actionCopy_region_triggered()
{
  view->performCopy();
}

void LayoutWindow::on_actionPaste_region_triggered()
{
  view->performPaste();
}

void LayoutWindow::on_resolutionComboBox_activated(const QString &arg1)
{
  int w,h; // arg1 is "w x h"
  LayoutRegionBase *currentRegionBase;
  QStringList list = arg1.split('x');
  w = list[0].toInt();
  h = list[1].toInt();
  currentRegionBase = view->getSelectedRegionBase();
  if(currentRegionBase != NULL)
  {
    currentRegionBase->changeResolution(w,h);
  }
}

void LayoutWindow::on_actionZoom_in_triggered()
{
    if(view->getSelectedRegionBase() != NULL)
    {
      LayoutCanvas* zoomObject = (LayoutCanvas*) view->getSelectedRegionBase()->parent();
      zoomObject->performZoomIn();
    }
}

void LayoutWindow::on_actionZoom_out_triggered()
{
  if(view->getSelectedRegionBase() != NULL)
  {
    LayoutCanvas* zoomObject = (LayoutCanvas*) view->getSelectedRegionBase()->parent();
    zoomObject->performZoomOut();
  }
}

void LayoutWindow::on_actionActionZoom_original_triggered()
{
  if(view->getSelectedRegionBase() != NULL)
  {
    LayoutCanvas* zoomObject = (LayoutCanvas*) view->getSelectedRegionBase()->parent();
    zoomObject->performZoomReset();
  }
}
