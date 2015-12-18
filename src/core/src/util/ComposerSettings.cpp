/* Copyright (c) 2011-2013 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "util/ComposerSettings.h"

#include <QApplication>
#include <QStringList>
#include <QDir>

namespace composer {
    namespace core {
        namespace util {
#ifdef Q_OS_MAC
GlobalSettings::GlobalSettings() :
  QSettings ("br.puc-rio.telemidia", "Composer")
#else
GlobalSettings::GlobalSettings() :
  QSettings(QSettings::IniFormat, QSettings::UserScope, "telemidia", "composer")
#endif
{

}

/*!
 * \brief Add the default paths to GlobalSettings.
 *
 * \fixme This function came from GUI, maybe some of the MACROS used above are
 * not working.
 */
void GlobalSettings::updateWithDefaults(const QString &dataPath)
{
  /* Defaults plugins paths */
  QStringList defaultPluginsPath;

  // The first path will look for plug-ins is relative to the executable
  defaultPluginsPath << QApplication::applicationDirPath() + "/extensions";

  // Then, we will look for plug-ins is at user's home.
  defaultPluginsPath << QDir::homePath() + QString("/composer/extensions");

  // After that we will look for plugins in the default system path
#ifdef Q_OS_MAC
  defaultPluginsPath << QApplication::applicationDirPath() + "/../../../extensions"
                     << "/Library/Application Support/Composer/Extensions"
                     << QApplication::applicationDirPath() +
                        "/../PlugIns/composer";
#elif defined(Q_OS_WIN32)

  defaultPluginsPath << "C:/Composer/extensions";
#else
  // PREFIX Should be defined by the qmake while compiling the source code.
#ifdef EXT_DEFAULT_PATH
  defaultPluginsPath << QString(EXT_DEFAULT_PATH)
                        + QString("/lib/composer/extensions");
#endif

#endif

  this->beginGroup("extensions");
  QStringList extensions_path = this->value("path").toStringList();
  extensions_path << defaultPluginsPath; //add default to extensions path
  extensions_path.removeDuplicates();
  this->setValue("path", extensions_path); //udpate with the new value
  this->endGroup();
  /* End defaults plugin path */

  /* Default language */
  this->beginGroup("languages");
  if(!this->contains("currentLanguage"))
    this->setValue("currentLanguage", "en");

  // Set the defaults supported languages
  if(!this->contains("supportedLanguages"))
  {
    QStringList list;
    list << "en_US" << "pt_BR" << "es_ES";
    this->setValue("supportedLanguages", list);
  }
  this->endGroup();
  /* End default language */

  /* Import Bases */
  QString defaultConnBaseDir;
  this->beginGroup("importBases");

  if(!this->contains("default_connector_base"))
  {
#ifdef Q_OS_MAC
#ifdef QT_NO_DEBUG
    defaultConnBaseDir = "../PlugIns/composer/";
#else
    defaultConnBaseDir = QApplication::applicationDirPath() + "/../../../data/";
#endif
#elif defined(Q_OS_WIN32)
    defaultConnBaseDir = QApplication::applicationDirPath() + "/data/";
#else
    // PREFIX Should be defined by the qmake while compiling the source code.
#ifdef EXT_DEFAULT_PATH
    defaultConnBaseDir = QString(EXT_DEFAULT_PATH)
        + QString("/share/composer/");
#endif
#endif
  }

  this->setValue("default_conn_base",
                    defaultConnBaseDir + "defaultConnBase.ncl");
  this->endGroup();
  /*End Import Bases*/

  /* Stylesheets */
  QStringList stylesheetsDirs =
      this->value("default_stylesheets_dirs").toStringList();
  stylesheetsDirs << QString(dataPath);

#if defined(Q_OS_MAC)
  stylesheetsDirs << QApplication::applicationDirPath() + "/../PlugIns/composer";
#else
  stylesheetsDirs << QApplication::applicationDirPath() + "/data";
#endif

  stylesheetsDirs.removeDuplicates();

  this->setValue("default_stylesheets_dirs", stylesheetsDirs);
  /* End Stylesheets */
}

ProjectSettings::ProjectSettings(const QString &project) :
  QSettings(project + ".ini", QSettings::IniFormat)
{

}



} } } // end namespace