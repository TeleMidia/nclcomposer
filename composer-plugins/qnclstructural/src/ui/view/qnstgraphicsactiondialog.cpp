#include "qnstgraphicsactiondialog.h"

QnstGraphicsActionDialog::QnstGraphicsActionDialog(QWidget* parent)
  : QDialog(parent)
{
  form.setupUi(this);
}

QnstGraphicsActionDialog::~QnstGraphicsActionDialog()
{

}

void QnstGraphicsActionDialog::init(QMap<QString, QnstConnector*> connectors,
                                    QnstGraphicsLink* link)
{
  this->connectors = connectors;
  this->link = link;

  form.cbConnector->clear();
  form.cbConnector->setEnabled(false);

  if (link != NULL)
    form.cbConnector->addItem(link->getxConnector());
  else
    form.cbConnector->addItem("New...");

  form.cbLink->clear();
  form.cbLink->setEnabled(false);

  if (link != NULL)
    form.cbLink->addItem(link->getnstId());
  else
    form.cbLink->addItem("New...");

  form.cbAction->clear();
  form.cbAction->setEnabled(true);

  QnstConnector* conn;

  if (link != NULL)
    conn = connectors[link->getxConnector()];
  else
    conn = NULL;

  if (conn != NULL)
  {
    if (conn->getName() == form.cbConnector->currentText())
    {
      foreach(QString name, conn->getActions().values())
      {
        form.cbAction->addItem(name);
      }
    }
  }
  else
  {
    form.cbAction->addItem("start");
    form.cbAction->addItem("stop");
    form.cbAction->addItem("resume");
    form.cbAction->addItem("pause");
    form.cbAction->addItem("set");
  }
}
