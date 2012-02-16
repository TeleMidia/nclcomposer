/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef DOCUMENTCONTROL_H
#define DOCUMENTCONTROL_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QTextStream>

#include <iostream>

#include "../util/ComposerCoreControl_global.h"

#include "PluginControl.h"
#include "LanguageControl.h"
using namespace composer::core;

#include "../model/Project.h"
using namespace composer::core::model;

#include "../util/Singleton.h"
#include "../util/Utilities.h"
using namespace composer::core::util;

namespace composer {
    namespace core {


/*!
 \brief Manages all the open Projects.
*/
class COMPOSERCORESHARED_EXPORT ProjectControl : public QObject
{
    Q_OBJECT
    SINGLETON(ProjectControl)

private:
    /*!
     \brief Constructor.
    */
    ProjectControl();
    /*!
     \brief Destructor.
    */
    ~ProjectControl();

    QMap<QString, Project*> openProjects; /*!< A map that keeps all the open
                                               projects. */

public:
    /*!
     \brief

     \param location
    */
    Project *getOpenProject(QString location);

 public slots:
    /*!
     \brief

     \param location
    */
    bool closeProject(QString location);
    /*!
     \brief

     \param location
    */
    void saveProject(QString location);
    /*!
     \brief

     \param location
    */
    void moveProject(QString location, QString dest, bool saveDesc = false);
    /*!
     \brief

     \param location
    */
    void saveTemporaryProject(QString location);
    /*!
     \brief

     \param location
    */
    bool launchProject(QString location);

    /*!
     \brief

     \param docLocation
     \param projLocation
    */
    void importFromDocument(QString docLocation, QString projLocation);

private slots:
    void projectIsDirty(bool isDirty);

signals:
    /*!
     \brief

     \param document
    */
    void startOpenProject(QString document);
    /*!
     \brief

     \param document
    */
    void endOpenProject(QString document);
    /*!
     \brief

     \param QString
    */
    void projectAlreadyOpen(QString);
    /*!
     \brief

     \param QString
    */
    void notifyError(QString);

    /*!
      \brief
      \param
     */
    void dirtyProject(QString, bool);
};

} } //end namespace


#endif // DOCUMENTCONTROL_H
