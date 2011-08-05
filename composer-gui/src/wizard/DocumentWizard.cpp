/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "wizard/DocumentWizard.h"

namespace composer {
    namespace gui {

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
    emit infoReceived(name, location, projectId, checked);

    QDialog::accept();
}

}} //end namespace
