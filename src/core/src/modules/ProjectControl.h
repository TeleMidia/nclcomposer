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

} } //end namespace


#endif // DOCUMENTCONTROL_H
