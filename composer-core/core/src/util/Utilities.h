/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "ComposerCoreControl_global.h"

#include <QString>
#include <QMap>
#include <QDir>
#include <QFileInfo>

#include "util/ComposerSettings.h"

/*!
 \brief
*/
typedef enum {
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

private:
  static QMap<QString,LanguageType> types; /*!< TODO */
};

} } } //end namespace

#endif // DEFINITIONS_H
