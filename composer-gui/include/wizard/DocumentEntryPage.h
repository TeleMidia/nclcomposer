/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef DOCUMENTENTRYPAGE_H
#define DOCUMENTENTRYPAGE_H

#include <QWizardPage>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QString>
#include <QPushButton>
#include <QGroupBox>

namespace composer {
    namespace gui {

class DocumentEntryPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DocumentEntryPage(QWidget *parent = 0);
    inline bool isCopy() { return copy;}
private:
    void initGUI();
    QLabel *nameLabel;
    QLineEdit *nameLineEdit;
    QLabel *locationLabel;
    QLineEdit *directoryLine;
    QPushButton *locatorButton;
    QGroupBox *copyBox;
    QString defaultLocation;
    bool copy;
signals:

public slots:

private slots:
    void setDocumentLocation();
    inline void setCopy(bool copy) { this->copy = copy; }

};

}} //end namespace

#endif // DOCUMENTENTRYPAGE_H
