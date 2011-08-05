/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef PROJECTENTRYPAGE_H
#define PROJECTENTRYPAGE_H

#include <QWizardPage>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QString>
#include <QPushButton>
#include <QCheckBox>
#include <QDebug>

namespace composer {
    namespace gui {

class ProjectEntryPage : public QWizardPage
{
    Q_OBJECT
    public:
        ProjectEntryPage(QWidget *parent = 0);
        ~ProjectEntryPage();

    private:
        QLabel *nameLabel;
        QLineEdit *nameLineEdit;
        QLabel *locationLabel;
        QLineEdit *directoryLine;
        QPushButton *locatorButton;
        QString defaultLocation;
        QCheckBox *checkCpy;
        bool isOpen;

        void initGUI();

    private slots:
        void setProjectDirectory();
        void setOpenProject(int state);
};

}} //end namespace
#endif // PROJECTENTRYPAGE_H
