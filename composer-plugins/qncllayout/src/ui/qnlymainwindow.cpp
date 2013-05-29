#include "qnlymainwindow.h"
#include "ui_qnlymainwindow.h"

#include <assert.h>
#include "ui/view/qnlyview.h"

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

  connect(ui->actionShow_Hide_Grid,
               SIGNAL(toggled(bool)),
               view,
               SLOT(setGridVisible(bool)));

  connect(view,
          SIGNAL(gridVisibilityChanged(bool)),
          ui->actionShow_Hide_Grid,
          SLOT(setChecked(bool)));
}
