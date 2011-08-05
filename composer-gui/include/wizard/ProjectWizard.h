/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef PROJECTWIZARD_H
#define PROJECTWIZARD_H

#include <QWizard>
#include <QString>
#include <QVariant>
#include <QDebug>

#include "ProjectEntryPage.h"

namespace composer {
    namespace gui {

/*!
 \brief A Wizard to guide the user to create a new document.
*/
class ProjectWizard : public QWizard
{
    Q_OBJECT
public:
    /*!
     \brief Constructor.

     \param parent The Parent of this Wizard.
    */
    ProjectWizard(QWidget* parent=0);
    /*!
     \brief Destrutor.
    */
    ~ProjectWizard();
    /*!
     \brief
    */
    void init();
    /*!
     \brief
    */
    void accept();

private:
    ProjectEntryPage *entryPage; /*!< TODO */
    QString name; /*!< TODO */
    QString location; /*!< TODO */

signals:
    /*!
     \brief

     \param QString
     \param QString
    */
    void infoReceived(QString,QString);
};

}} //end namespace
#endif // PROJECTWIZARD_H
