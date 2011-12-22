#include "qnstgraphicslinkdialog.h"

QnstGraphicsLinkDialog::QnstGraphicsLinkDialog(QWidget* parent)
    : QDialog(parent)
{
    form.setupUi(this);

    connect(form.cbConnector, SIGNAL(currentIndexChanged(QString)), SLOT(adjustBinds(QString)));
}

QnstGraphicsLinkDialog::~QnstGraphicsLinkDialog()
{

}

void QnstGraphicsLinkDialog::adjustBinds(QString conn)
{
    if (conn == "" || conn == "----------"){
        form.cbCondition->setEnabled(false);
        form.cbCondition->clear();

        form.cbAction->setEnabled(false);
        form.cbAction->clear();

    }else if (conn == "Personalized"){
        form.cbCondition->setEnabled(true);

        form.cbCondition->addItem("onBegin");
        form.cbCondition->addItem("onEnd");
        form.cbCondition->addItem("onSelection");
        form.cbCondition->addItem("onResume");
        form.cbCondition->addItem("onPause");

        form.cbAction->setEnabled(true);

        form.cbAction->addItem("Start");
        form.cbAction->addItem("Stop");
        form.cbAction->addItem("Resume");
        form.cbAction->addItem("Pause");
        form.cbAction->addItem("Set");
    }
}
