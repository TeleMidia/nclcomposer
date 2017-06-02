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

#ifndef LANGUAGECONTROL_H
#define LANGUAGECONTROL_H

#include <QObject>
#include <QMap>
#include <QPluginLoader>
#include <QDir>

#include "util/ComposerCore_global.h"

#include "extensions/ILanguageProfile.h"
#include "util/Singleton.h"
#include "util/Utilities.h"
using namespace cpr::core;

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \brief Manages the installed language profiles (though ILanguageProfile
 *  interface).
 */
class COMPOSERCORESHARED_EXPORT LanguageControl : public QObject
{
  Q_OBJECT
  SINGLETON(LanguageControl) // Constructor wil be private

public:
  /*!
   * \brief Load all the profiles from the default extensions paths.
   */
  void loadProfiles();
  /*!
   * \brief Load all the profiles from a specific directory.
   *
   * \param profilesDirPath the directory path from where the profiles must be
   * loaded.
   */
  void loadProfiles(const QString &profilesDirPath);

  /*!
   * \brief Load a language profile from a specific fileName.
   *
   * \param fileName th
   */
  ILanguageProfile* loadProfile(const QString &fileName);

  /*!
   * \brief
   * \param type
   */
  ILanguageProfile* getProfileFromType(LanguageType type);

  /*!
   * \brief
   * \param type
   */
  bool removeProfile(LanguageType type);

  /*!
   * \brief
   */
  QList<ILanguageProfile*> getLoadedProfiles();

signals:
  /*!
   * \brief
   *
   * \param QString
   */
  void notifyError(QString);

private:
  QMap<LanguageType, ILanguageProfile*> profiles; /*!< TODO */

  /*!
   * \brief Constructor (it is private because this class is a Singleton).
   */
  LanguageControl();

  /*!
   * \brief Destructor (it is private because this class is a Singleton).
   */
  ~LanguageControl();
};

CPR_CORE_END_NAMESPACE

#endif // LANGUAGECONTROL_H
