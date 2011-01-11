#include "../../../include/gui/wizard/MainWizard.h"

MainWizard::MainWizard(QWidget* parent) :
        QWizard(parent) {
    intro = new IntroPage(this);
    addPage(intro);
    entry = new EntryPage(this);
    addPage(entry);
}

void MainWizard::init() {
    entry->initGUI();
    this->show();
    this->exec();
}

MainWizard::~MainWizard() {
    //qDebug() << "Calling Project Wizard Destructor";
}

void MainWizard::accept() {
    name        = field("name").toString();
    location    = field("location").toString();
    //TODO - melhorar pegando o checkbox ... e juntando em um só sinal
    qDebug() << "MainWizard::accept " << location+QDir::separator()+name;
    if (projectId != "") {
        emit infoReceived(name, location+QDir::separator()+name, projectId,true);
    } else {
        emit infoReceived(name, location+QDir::separator()+name);
    }
    QDialog::accept();
}
