#include "wizard/ProjectWizard.h"

namespace composer {
    namespace ui {

ProjectWizard::ProjectWizard(QWidget* parent) :
        QWizard(parent) {
      //setWizardStyle(MacStyle);
      setWindowTitle("Project Wizard!");
      entryPage = new ProjectEntryPage(this);
      addPage(entryPage);
}

ProjectWizard::~ProjectWizard() {
    //qDebug() << "Calling Project Wizard Destructor";
}

void ProjectWizard::init() {
    //entry->initGUI();
    this->show();
    this->exec();
}

void ProjectWizard::accept() {

    name        = field("name").toString();
    location    = field("location").toString();

    qDebug() << "ProjectWizard::accept(" << name << ", " << location << ")";
    emit infoReceived(name,location);

    QDialog::accept();
}

}} //end namespace
