#include "wizard/DocumentWizard.h"


DocumentWizard::DocumentWizard(QWidget* parent) :
        QWizard(parent) {
      setWizardStyle(MacStyle);
      setWindowTitle("Document Wizard!");
      entryPage = new DocumentEntryPage(this);
      addPage(entryPage);
}

DocumentWizard::~DocumentWizard() {
    //qDebug() << "Calling Project Wizard Destructor";
}

void DocumentWizard::init() {
    //entry->initGUI();
    this->show();
    this->exec();
}

void DocumentWizard::accept() {

    QString name        = field("name").toString();
    QString location    = field("location").toString();
    bool checked        = entryPage->isCopy();

    qDebug() << "DocumentWizard::accept(" << name << ", " << location << ")";
    emit infoReceived(name,location,projectId,checked);

    QDialog::accept();
}
