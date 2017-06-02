/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef DOCUMENTCONTROL_H
#define DOCUMENTCONTROL_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QTextStream>

#include <iostream>

#include "util/ComposerCore_global.h"

#include "PluginControl.h"
#include "LanguageControl.h"
#include "model/Project.h"
#include "util/Singleton.h"
#include "util/Utilities.h"
using namespace cpr::core;

CPR_CORE_BEGIN_NAMESPACE

/*!
 \brief Manages all the open Projects.
*/
class COMPOSERCORESHARED_EXPORT ProjectControl : public QObject
{
  Q_OBJECT
  SINGLETON(ProjectControl)

public:
  /*!
   * \brief getOpenProject
   *
   * \param location
   * \return
   */
  Project *getOpenProject(const QString &location);

public slots:
  /*!
   * \brief closeProject
   * \param location
   * \return
   */
  bool closeProject(const QString &location);

  /*!
   * \brief saveProject
   * \param location
   */
  void saveProject(const QString &location);

  /*!
   * \brief moveProject
   *
   * \param location
   * \param dest
   * \param saveDesc
   */
  void moveProject(const QString &location, const QString &dest, bool saveDesc = false);

  /*!
   * \brief saveTemporaryProject
   *
   * \param location
   */
  void saveTemporaryProject(const QString &location);

  /*!
   * \brief launchProject
   *
   * \param location
   * \return
   */
  bool launchProject(const QString &location);


  /*!
   * \brief importFromDocument
   *
   * \param docLocation
   * \param projLocation
   */
  void importFromDocument(const QString &docLocation, const QString &projLocation);

signals:
  /*!
   * \brief startOpenProject
   *
   * \param document
   */
  void startOpenProject(const QString &document);

  /*!
   * \brief endOpenProject
   *
   * \param document
   */
  void endOpenProject(const QString &document);

  /*!
   * \brief projectAlreadyOpen
   */
  void projectAlreadyOpen(const QString&);

  /*!
   * \brief notifyError
   *
   * \param strError
   */
  void notifyError(const QString &strError);

  /*!
   * \brief dirtyProject
   */
  void dirtyProject(const QString&, bool);

private:
  /*!
   * \brief Constructor.
   */
  ProjectControl();

  /*!
   * \brief Destructor.
   */
  ~ProjectControl();

  QMap<QString, Project*> openProjects; /*!< A map that keeps all the open
                                             projects. */

private slots:
  void projectIsDirty(bool isDirty);

};

CPR_CORE_END_NAMESPACE

#endif // DOCUMENTCONTROL_H
