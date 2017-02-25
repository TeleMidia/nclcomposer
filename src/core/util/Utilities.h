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

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "ComposerCoreControl_global.h"

#include <QString>
#include <QMap>
#include <QDir>
#include <QFileInfo>

#include <QLoggingCategory>

#include "util/ComposerSettings.h"

/*!
 \brief
*/
typedef enum
{
  CPR,
  NCL,
  SMIL,
  HTML,
  NONE
} LanguageType;

namespace composer {
  namespace core {
    namespace util {
/*!
 \brief Groups useful functions.
*/
class COMPOSERCORESHARED_EXPORT Utilities
{
public:
  /*!
   * \brief getLanguageTypeByExtension
   * \param ext
   * \return
   */
  static QString normalizeXMLID (const QString &);
  /*!
   * \brief getLanguageTypeByExtension
   * \param ext
   * \return
   */
  static LanguageType getLanguageTypeByExtension(const QString &ext);

  /*!
   * \brief getExtensionForLanguageType
   * \param type
   * \return
   */
  static QString getExtensionForLanguageType(LanguageType type);

  /*!
   * \brief relativePath
   * \param absolutePath
   * \param relativeTo
   * \param bIsFile
   * \return
   */
  static QString relativePath( const QString &absolutePath,
                               const QString &relativeTo,
                               bool bIsFile = false );

  /*!
   * \brief getAbsolutePath
   * \param path
   * \param relativeTo
   * \return
   */
  static QString absolutePath ( const QString &path,
                                const QString &relativeTo );

  /*!
   * \brief getLastFileDialogPath
   * \return
   */
  static QString getLastFileDialogPath();

  /*!
   * \brief updateLastFileDialogPath
   * \param filepath
   */
  static void updateLastFileDialogPath( const QString &filepath );

  /*!
   * \brief splitParameters
   * \param params
   * \return
   */
  static QStringList splitParams(QString &params);

private:
  static QMap<QString,LanguageType> types; /*!< TODO */
};

Q_DECLARE_LOGGING_CATEGORY(CPR_CORE)
Q_DECLARE_LOGGING_CATEGORY(CPR_GUI)

} } } //end namespace

#endif // DEFINITIONS_H
