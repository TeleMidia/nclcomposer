/* Copyright (C) 2011-2018 PUC-Rio/TeleMídia Lab.

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

#include "util/Utilities.h"

#include <QDebug>
#include <QStringList>

CPR_CORE_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY (CPR_CORE, "cpr.core")
Q_LOGGING_CATEGORY (CPR_GUI, "cpr.gui")

static
QMap<QString, LanguageType> _types
    = { { "cpr", NCL }, { "ncl", NCL }, { "smil", SMIL }, { "html", HTML } };

LanguageType
Utilities::languageTypeByExtension (const QString &ext)
{
  if (!_types.contains (ext))
    return NONE;
  else
    return _types[ext];
}

QString
Utilities::extensionForLanguageType (LanguageType type)
{
  for (auto it = _types.begin (); it != _types.end (); ++it)
    if (type == it.value ())
      return it.key ();
  return "";
}

QString
Utilities::relativePath (const QString &absPath, const QString &relTo,
                         bool bIsFile /*= false*/)
{
  QString absolutePath = absPath;
  absolutePath.replace ("\\", "/"); // Force "/" instead of "\\".
  QString relativeTo = relTo;
  relativeTo = relativeTo.replace ("\\", "/");

  QStringList absoluteDirs = absolutePath.split ("/", QString::SkipEmptyParts);
  QStringList relativeDirs = relativeTo.split ("/", QString::SkipEmptyParts);

  // Get the shortest of the two paths.
  int length = absoluteDirs.count () < relativeDirs.count ()
                   ? absoluteDirs.count ()
                   : relativeDirs.count ();

  // Use to determine where in the loop we exited.
  int lastCommonRoot = -1;
  int index;

  // Find common root.
  for (index = 0; index < length; index++)
  {
    if (absoluteDirs[index] == relativeDirs[index])
      lastCommonRoot = index;
    else
      break;
  }

  // If we didn't find a common prefix then throw an exception.
  if (lastCommonRoot == -1)
    throw QString ("Paths do not have a common base");

  // Build up the relative path.
  QString relativePath;

  // Add on the ..
  for (index = lastCommonRoot + 1;
       index < absoluteDirs.count () - (bIsFile ? 1 : 0); index++)
  {
    if (absoluteDirs[index].length () > 0)
    {
      relativePath.append ("../");
      // relativePath.append(QDir::separator());
    }
  }

  // Add on the folders.
  for (index = lastCommonRoot + 1; index < relativeDirs.count () - 1;
       index++)
  {
    relativePath.append (relativeDirs[index]).append ("/");
  }

  relativePath.append (relativeDirs[relativeDirs.count () - 1]);

  return relativePath;
}

QString
Utilities::absolutePath (const QString &path, const QString &relativeTo)
{
  QFileInfo fInfo (path);
  if (fInfo.isAbsolute ())
    return path;
  else
    return relativeTo + "/" + path;
}

QString
Utilities::lastFileDialogPath ()
{
  GlobalSettings settings;
  QString lastFileDialogPath = QDir::homePath ();

  settings.beginGroup ("mainwindow"); // TODO: A better name to mainwindow.
  if (settings.contains ("lastFileDialogPath"))
    lastFileDialogPath = settings.value ("lastFileDialogPath").toString ();
  settings.endGroup ();

  qCDebug (CPR_CORE) << lastFileDialogPath;

  return lastFileDialogPath;
}

void
Utilities::updateLastFileDialogPath (const QString &filepath)
{
  QFileInfo fileInfo (filepath);

  GlobalSettings settings;
  settings.beginGroup ("mainwindow");
  settings.setValue ("lastFileDialogPath", fileInfo.absolutePath ());
  settings.endGroup ();
}

QStringList
Utilities::splitParams (QString &params)
{
  QStringList plist;
  QRegExp rx ("([-${}\\w\\\\:]+|\\\".*\\\")");

  int pos = 0;
  while ((pos = rx.indexIn (params, pos)) != -1)
  {
    plist << rx.cap (1);
    pos += rx.matchedLength ();
  }

  return plist;
}

CPR_CORE_END_NAMESPACE
