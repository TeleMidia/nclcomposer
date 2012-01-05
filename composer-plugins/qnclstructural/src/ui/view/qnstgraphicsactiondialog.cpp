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
            foreach(QString name, conn->getActions()){
                form.cbAction->addItem(name);
            }
        }
    }else{
        form.cbAction->addItem("Start");
        form.cbAction->addItem("Stop");
        form.cbAction->addItem("Resume");
        form.cbAction->addItem("Pause");
        form.cbAction->addItem("Set");
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

        form.cbAction->addItem("Start");
        form.cbAction->addItem("Stop");
        form.cbAction->addItem("Resume");
        form.cbAction->addItem("Pause");
        form.cbAction->addItem("Set");

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
                foreach(QString name, conn->getActions()){
                    form.cbAction->addItem(name);
                }
            }
        }
    }
}
