#include "qnstgraphicslinkdialog.h"

#include <QDesktopWidget>
#include <QApplication>

QnstGraphicsLinkDialog::QnstGraphicsLinkDialog(QWidget* parent)
    : QDialog(parent), firstTime(true)
{
    form.setupUi(this);

    connect(form.cbConnector, SIGNAL(currentIndexChanged(QString)), SLOT(adjustBinds(QString)));
}

QnstGraphicsLinkDialog::~QnstGraphicsLinkDialog()
{

}

void QnstGraphicsLinkDialog::init(QMap<QString, QnstConncetor*> connectors)
{
    this->connectors = connectors;

    form.cbConnector->clear();

    form.cbCondition->setEnabled(false);
    form.cbCondition->clear();

    form.cbAction->setEnabled(false);
    form.cbAction->clear();

    foreach(QnstConncetor* conn, connectors.values()){
        form.cbConnector->addItem(conn->getName());
    }

    if (form.cbConnector->count() > 0){
        form.cbConnector->addItem("----------");
    }

    form.cbConnector->addItem("New...");

    // Center the Dialog if this is the first time that we are openning QnstGraphicsLinkDialog
    if(firstTime) {
        updateGeometry();
        QDesktopWidget *desktop = QApplication::desktop();

        int screenWidth = desktop->width();
        int screenHeight = desktop->height();

        int x = (screenWidth - this->width()) / 2;
        int y = (screenHeight - this->height()) / 2;

        this->move(x, y);
        firstTime = false;
    }
}

void QnstGraphicsLinkDialog::adjustBinds(QString conn)
{
    if (conn == "" || conn == "----------"){
        form.cbCondition->setEnabled(false);
        form.cbCondition->clear();

        form.cbAction->setEnabled(false);
        form.cbAction->clear();

    }else if (conn == "New..."){
        form.cbCondition->setEnabled(true);
        form.cbCondition->clear();

        form.cbCondition->addItem("onBegin");
        form.cbCondition->addItem("onEnd");
        form.cbCondition->addItem("onSelection");
        form.cbCondition->addItem("onResume");
        form.cbCondition->addItem("onPause");

        form.cbAction->setEnabled(true);
        form.cbAction->clear();

        form.cbAction->addItem("start");
        form.cbAction->addItem("stop");
        form.cbAction->addItem("resume");
        form.cbAction->addItem("pause");
        form.cbAction->addItem("set");
    }else{
        QnstConncetor* oconn = connectors[conn];

        form.cbCondition->setEnabled(true);
        form.cbCondition->clear();

        if (oconn->getName() == conn){
            foreach(QString cond, oconn->getConditions().values()){
                form.cbCondition->addItem(cond);
            }
        }

        form.cbAction->setEnabled(true);
        form.cbAction->clear();

        if (oconn->getName() == conn){
            foreach(QString act, oconn->getActions().values()){
                form.cbAction->addItem(act);
            }
        }
    }
}
