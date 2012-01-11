#include "qnstgraphicsactiondialog.h"

QnstGraphicsActionDialog::QnstGraphicsActionDialog(QWidget* parent)
    : QDialog(parent)
{
    form.setupUi(this);

    connect(form.cbConnector, SIGNAL(currentIndexChanged(QString)), SLOT(adjust(QString)));
}

QnstGraphicsActionDialog::~QnstGraphicsActionDialog()
{

}

void QnstGraphicsActionDialog::init(QMap<QString, QnstConncetor*> connectors, QMap<QString, QString> links)
{
    this->connectors = connectors;
    this->links = links;

    form.cbConnector->clear();

    foreach(QnstConncetor* connector, connectors.values()){
        form.cbConnector->addItem(connector->getName());
    }

    if (form.cbConnector->count() > 0){
        form.cbConnector->addItem("----------");
    }

    form.cbConnector->addItem("New...");

    form.cbLink->clear();

    if (form.cbConnector->currentText() != "----------"){
        foreach(QString link, links.keys(form.cbConnector->currentText())){
            form.cbLink->addItem(link);
        }
    }

    if (form.cbLink->count() > 0){
        form.cbLink->addItem("----------");
    }

    form.cbLink->addItem("New...");

    form.cbAction->setEnabled(true);
    form.cbAction->clear();

    QnstConncetor* conn = connectors[form.cbConnector->currentText()];

    if (conn != NULL){
        if (conn->getName() == form.cbConnector->currentText()){
            foreach(QString name, conn->getActions().values()){
                form.cbAction->addItem(name);
            }
        }
    }else{
        form.cbAction->addItem("start");
        form.cbAction->addItem("stop");
        form.cbAction->addItem("resume");
        form.cbAction->addItem("pause");
        form.cbAction->addItem("set");
    }
}


void QnstGraphicsActionDialog::adjust(QString connector)
{
    if (connector == "----------"){
        form.cbLink->setEnabled(false);
        form.cbLink->clear();

        form.cbAction->setEnabled(false);
        form.cbAction->clear();

    }else if (connector == "New..."){
        form.cbLink->setEnabled(true);
        form.cbLink->clear();

        form.cbLink->addItem("New...");

        form.cbAction->setEnabled(true);
        form.cbAction->clear();

        form.cbAction->addItem("start");
        form.cbAction->addItem("stop");
        form.cbAction->addItem("resume");
        form.cbAction->addItem("pause");
        form.cbAction->addItem("set");

    }else{
        form.cbLink->setEnabled(true);
        form.cbLink->clear();

        foreach(QString link, links.keys(connector)){
            form.cbLink->addItem(link);
        }

        if (form.cbLink->count() > 0){
            form.cbLink->addItem("----------");
        }

        form.cbLink->addItem("New...");

        QnstConncetor* conn = connectors[connector];

        if (conn != NULL){
            form.cbAction->setEnabled(true);
            form.cbAction->clear();

            if (conn->getName() == connector){
                foreach(QString name, conn->getActions().values()){
                    form.cbAction->addItem(name);
                }
            }
        }
    }
}
