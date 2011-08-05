/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "wizard/ProjectWizard.h"

namespace composer {
    namespace gui {

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

} } //end namespace
