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

#include "ComposerCore_global.h"

#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QString>

#include <QLoggingCategory>

#include "util/ComposerSettings.h"

/*!
 \brief
*/
typedef enum { CPR, NCL, SMIL, HTML, NONE } LanguageType;

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief Groups useful functions.
 */
class COMPOSERCORESHARED_EXPORT Utilities
{
public:
  /*!
   * \brief getLanguageTypeByExtension
   * \param ext
   * \return
   */
  static LanguageType languageTypeByExtension (const QString &ext);

  /*!
   * \brief getExtensionForLanguageType
   * \param type
   * \return
   */
  static QString extensionForLanguageType (LanguageType type);

  /*!
   * \brief relativePath
   * \param absolutePath
   * \param relativeTo
   * \param bIsFile
   * \return
   */
  static QString relativePath (const QString &absolutePath,
                               const QString &relativeTo,
                               bool bIsFile = false);

  /*!
   * \brief getAbsolutePath
   * \param path
   * \param relativeTo
   * \return
   */
  static QString absolutePath (const QString &path, const QString &relativeTo);

  /*!
   * \brief getLastFileDialogPath
   * \return
   */
  static QString lastFileDialogPath ();

  /*!
   * \brief updateLastFileDialogPath
   * \param filepath
   */
  static void updateLastFileDialogPath (const QString &filepath);

  /*!
   * \brief splitParameters
   * \param params
   * \return
   */
  static QStringList splitParams (QString &params);

private:
  static QMap<QString, LanguageType> _types; /*!< TODO */
};

COMPOSERCORESHARED_EXPORT
Q_DECLARE_LOGGING_CATEGORY (CPR_CORE)
COMPOSERCORESHARED_EXPORT Q_DECLARE_LOGGING_CATEGORY (CPR_GUI)

// Assert macros.
#define CPR_ASSERT(cond)                                                      \
  ((!(cond)) ? qt_assert (#cond, __FILE__, __LINE__) : qt_noop ())

#define CPR_ASSERT_X(cond, where, what)                                       \
  ((!(cond)) ? qt_assert_x (where, what, __FILE__, __LINE__) : qt_noop ())

#define CPR_ASSERT_NON_NULL(var) CPR_ASSERT (var != nullptr)

#define CPR_ASSERT_NOT_REACHED()                                              \
  CPR_ASSERT_X (false, "", "Should not reach here.")

// Log macros.
#define cpr_debug(module, msg) qCDebug (module, msg)
#define cpr_warn(module, msg)
#define cpr_error(module, msg)

// Cast utilities.
#define cast(a, b) (dynamic_cast<a> ((b)))
#define instanceof(a, b) (cast (a, (b)) != nullptr)
#define not_null(a) (a != nullptr)

CPR_CORE_END_NAMESPACE

#endif // DEFINITIONS_H
