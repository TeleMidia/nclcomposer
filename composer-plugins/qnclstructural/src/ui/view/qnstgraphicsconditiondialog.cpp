#include "qnstgraphicsconditiondialog.h"

QnstGraphicsConditionDialog::QnstGraphicsConditionDialog(QWidget* parent)
    : QDialog(parent)
{
    form.setupUi(this);

    connect(form.cbConnector, SIGNAL(currentIndexChanged(QString)), SLOT(adjust(QString)));
}

QnstGraphicsConditionDialog::~QnstGraphicsConditionDialog()
{

}

void QnstGraphicsConditionDialog::init(QMap<QString, QnstConncetor*> connectors, QMap<QString, QString> links)
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

    form.cbCondition->setEnabled(true);
    form.cbCondition->clear();

    QnstConncetor* conn = connectors[form.cbConnector->currentText()];

    if (conn != NULL){
        if (conn->getName() == form.cbConnector->currentText()){
            foreach(QString name, conn->getConditions().values()){
                form.cbCondition->addItem(name);
            }
        }
    }else{
        form.cbCondition->addItem("onBegin");
        form.cbCondition->addItem("onEnd");
        form.cbCondition->addItem("onSelection");
        form.cbCondition->addItem("onResume");
        form.cbCondition->addItem("onPause");
    }
}


void QnstGraphicsConditionDialog::adjust(QString connector)
{
    if (connector == "----------"){
        form.cbLink->setEnabled(false);
        form.cbLink->clear();

        form.cbCondition->setEnabled(false);
        form.cbCondition->clear();

    }else if (connector == "New..."){
        form.cbLink->setEnabled(true);
        form.cbLink->clear();

        form.cbLink->addItem("New...");

        form.cbCondition->setEnabled(true);
        form.cbCondition->clear();

        form.cbCondition->addItem("onBegin");
        form.cbCondition->addItem("onEnd");
        form.cbCondition->addItem("onSelection");
        form.cbCondition->addItem("onResume");
        form.cbCondition->addItem("onPause");

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
            form.cbCondition->setEnabled(true);
            form.cbCondition->clear();

            if (conn->getName() == connector){
                foreach(QString name, conn->getConditions().values()){
                    form.cbCondition->addItem(name);
                }
            }
        }
    }
}
