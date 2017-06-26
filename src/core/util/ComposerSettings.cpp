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

#include "util/ComposerSettings.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QStringList>

CPR_CORE_BEGIN_NAMESPACE

GlobalSettings::GlobalSettings ()
    : QSettings (QSettings::IniFormat, QSettings::UserScope, "telemidia",
                 "nclcomposer")
{
}

void
GlobalSettings::loadDefaults (const QString &dataPath)
{
  QSettings defaultSettings (dataPath, QSettings::IniFormat);

  QStringList keys = defaultSettings.allKeys ();
  for (const QString &key : keys)
    setValue (key, defaultSettings.value (key));
}

/*!
 * \brief Add the default paths to GlobalSettings.
 *
 * \fixme This function came from GUI, maybe some of the MACROS used above are
 * not working.
 */
void
GlobalSettings::addPlatformDefaults ()
{
  /* Defaults plugins paths */
  QStringList defaultPluginsPath;

#ifndef Q_OS_MAC
  // The first path will look for plug-ins is relative to the executable
  defaultPluginsPath << QApplication::applicationDirPath ();
  defaultPluginsPath << QApplication::applicationDirPath () + "/plugins";
  defaultPluginsPath << QApplication::applicationDirPath ()
                            + "/../lib/nclcomposer/plugins";

  // Then, we will look for plug-ins in user's home.
  defaultPluginsPath << QDir::homePath () + QString ("/nclcomposer/plugins");
#endif

// After that we will look for plugins in the default system path
#ifdef Q_OS_MAC
  defaultPluginsPath << QApplication::applicationDirPath ()
                            + "/../PlugIns/composer/";
#endif

  this->beginGroup ("extensions");
  QStringList extensions_path = this->value ("path").toStringList ();
  extensions_path << defaultPluginsPath; // add default to extensions path
  extensions_path.removeDuplicates ();
  this->setValue ("path", extensions_path); // udpate with the new value
  this->endGroup ();
  /* End defaults plugin path */

  /* Default language */
  this->beginGroup ("languages");
  if (!this->contains ("currentLanguage"))
    this->setValue ("currentLanguage", "en");

  // Set the defaults supported languages
  if (!this->contains ("supportedLanguages"))
  {
    QStringList list;
    list << "en_US"
         << "pt_BR"
         << "es_ES";
    this->setValue ("supportedLanguages", list);
  }
  this->endGroup ();
  /* End default language */

  /* Import Bases */
  QString defaultConnBaseDir;
  this->beginGroup ("importBases");

  if (!this->contains ("default_connector_base"))
  {
#ifdef Q_OS_MAC
#ifdef QT_NO_DEBUG_OUTPUT
    defaultConnBaseDir
        = QApplication::applicationDirPath () + "/../PlugIns/composer/";
#else
    defaultConnBaseDir = "/Library/Application Support/Composer/Data/";
#endif
#else
    defaultConnBaseDir
        = QApplication::applicationDirPath () + "/../share/nclcomposer/";
#endif
  }

  this->setValue ("default_conn_base",
                  defaultConnBaseDir + "defaultConnBase.ncl");
  this->endGroup ();
  /*End Import Bases*/

  /* Stylesheet */
  this->beginGroup ("theme");
  if (!this->contains ("stylesheet"))
  {
    this->setValue ("stylesheet", QApplication::applicationDirPath ()
                                      + "/../share/nclcomposer/style.qss");
  }

  if (!this->contains ("stylesheet_ini"))
  {
    this->setValue ("stylesheet_ini", QApplication::applicationDirPath ()
                                          + "/../share/nclcomposer/style.ini");
  }
  this->endGroup ();
  /* End Stylesheet */
}

QStringList
GlobalSettings::getExtensionsPaths ()
{
  beginGroup ("extensions");
  QStringList extPaths = value ("path").toStringList ();
  endGroup ();

  extPaths.removeDuplicates (); // Remove duplicate paths
  return extPaths;
}

ProjectSettings::ProjectSettings (const QString &project)
    : QSettings (project + ".ini", QSettings::IniFormat)
{
}

CPR_CORE_END_NAMESPACE
