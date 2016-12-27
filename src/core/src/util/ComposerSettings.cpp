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
#include <QDebug>

namespace composer {
    namespace core {
        namespace util {
GlobalSettings::GlobalSettings() :
  QSettings(QSettings::IniFormat, QSettings::UserScope, "telemidia", "nclcomposer")
{

}

void GlobalSettings::loadDefaults(const QString &dataPath)
{
  QSettings defaultSettings(dataPath, QSettings::IniFormat);

  QStringList keys = defaultSettings.allKeys();
  for (const QString &key : keys)
    setValue( key, defaultSettings.value(key) );
}

/*!
 * \brief Add the default paths to GlobalSettings.
 *
 * \fixme This function came from GUI, maybe some of the MACROS used above are
 * not working.
 */
void GlobalSettings::addPlatformDefaults()
{
  /* Defaults plugins paths */
  QStringList defaultPluginsPath;

#ifndef Q_OS_MAC
  // The first path will look for plug-ins is relative to the executable
  defaultPluginsPath << QApplication::applicationDirPath();
  defaultPluginsPath << QApplication::applicationDirPath() + "/plugins";
  defaultPluginsPath << QApplication::applicationDirPath() + "/../lib/nclcomposer/plugins";

  // Then, we will look for plug-ins in user's home.
  defaultPluginsPath << QDir::homePath() + QString("/nclcomposer/plugins");
#endif

  // After that we will look for plugins in the default system path (do we need that?)
#ifdef Q_OS_MAC
    defaultPluginsPath << QApplication::applicationDirPath() + "/../PlugIns/composer/";
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
#ifdef QT_NO_DEBUG_OUTPUT
    defaultConnBaseDir = QApplication::applicationDirPath() + "/../PlugIns/composer/";
#else
    defaultConnBaseDir = "/Library/Application Support/Composer/Data/";
#endif
#else
    defaultConnBaseDir = QApplication::applicationDirPath() + "/../etc/nclcomposer/";
#endif
  }

  this->setValue("default_conn_base",
                    defaultConnBaseDir + "defaultConnBase.ncl");
  this->endGroup();
  /*End Import Bases*/

  /* Stylesheet */
  this->beginGroup("theme");
  QString stylesheet =
    QApplication::applicationDirPath() + "/../etc/nclcomposer/style.qss";
  this->setValue("stylesheet", stylesheet);
  QString stylesheet_ini =
    QApplication::applicationDirPath() + "/../etc/nclcomposer/style.ini";
  this->setValue("stylesheet_ini", stylesheet_ini);
  this->endGroup();
  /* End Stylesheet */
}

ProjectSettings::ProjectSettings(const QString &project) :
  QSettings(project + ".ini", QSettings::IniFormat)
{

}

} } } // end namespace

