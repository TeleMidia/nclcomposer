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
          SLOT(removeRegion()));

  connect(ui->actionShow_Hide_Grid,
               SIGNAL(toggled(bool)),
               view,
               SLOT(setGridVisible(bool)));

  connect(view,
          SIGNAL(gridVisibilityChanged(bool)),
          ui->actionShow_Hide_Grid,
          SLOT(setChecked(bool)));
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

void QnlyMainWindow::removeRegion()
{
  QnlyGraphicsRegion *region = view->getSelectedRegion();
  if(region != NULL)
  {
    region->performDelete();
  }
}
