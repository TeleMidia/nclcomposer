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

#include "util/Utilities.h"

#include <QStringList>
#include <QDebug>

QMap<QString,LanguageType> createMap() {
    QMap<QString,LanguageType> types;
    types["cpr"]      = NCL;
    types["ncl"]      = NCL;
    types["smil"]     = SMIL;
    types["html"]     = HTML;
    return types;
}

namespace composer {
  namespace core {
    namespace util {

Q_LOGGING_CATEGORY(CPR_CORE, "cpr.core")

QMap<QString,LanguageType> Utilities::types = createMap();

QString Utilities::normalizeXMLID(const QString &id)
{
  QString tmp = id.normalized(QString::NormalizationForm_KD);
  tmp.remove(QRegExp("[^a-zA-Z_-\\.\\s]"));
  if(tmp.at(0).isDigit())
    tmp = "_" + tmp;

  return tmp;
}

LanguageType Utilities::getLanguageTypeByExtension(const QString &ext)
{
  if (!types.contains(ext)) return NONE;
  else return types[ext];
}

QString Utilities::getExtensionForLanguageType(LanguageType type)
{
  QMap<QString,LanguageType>::iterator it;
  for (it = types.begin(); it != types.end(); it++)
    if(type == it.value())
      return it.key();
  return "";
}

QString Utilities::relativePath( const QString &absPath,
                                 const QString &relTo,
                                 bool bIsFile /*= false*/ )
{
  //force the "/" instead of "\\"
  QString absolutePath = absPath;
  absolutePath.replace("\\", "/");
  QString relativeTo = relTo;
  relativeTo = relativeTo.replace("\\", "/");

  QStringList absoluteDirectories = absolutePath.split("/",
                                                       QString::SkipEmptyParts);
  QStringList relativeDirectories = relativeTo.split("/",
                                                     QString::SkipEmptyParts);

  //Get the shortest of the two paths
  int length = absoluteDirectories.count() < relativeDirectories.count() ?
               absoluteDirectories.count() : relativeDirectories.count();

  //Use to determine where in the loop we exited
  int lastCommonRoot = -1;
  int index;

  //Find common root
  for (index = 0; index < length; index++)
  {
    if (absoluteDirectories[index] == relativeDirectories[index])
      lastCommonRoot = index;
    else
      break;
  }

  //If we didn't find a common prefix then throw
  if (lastCommonRoot == -1)
    throw QString("Paths do not have a common base");

  //Build up the relative path
  QString relativePath;

  //Add on the ..
  for (index = lastCommonRoot + 1;
       index < absoluteDirectories.count() - (bIsFile?1:0); index++)
  {
    if (absoluteDirectories[index].length() > 0)
    {
      relativePath.append("../");
//      relativePath.append(QDir::separator());
    }
  }

  //Add on the folders
  for (index = lastCommonRoot + 1; index < relativeDirectories.count() - 1;
       index++)
  {
    relativePath.append( relativeDirectories[index] ).append("/");
  }

  relativePath.append(relativeDirectories[relativeDirectories.count() - 1]);

  return relativePath;
}

QString Utilities::absolutePath( const QString &path,
                                 const QString &relativeTo )
{
  QFileInfo fInfo (path);
  if(fInfo.isAbsolute())
    return path;
  else
    return relativeTo + "/" + path;
}

QString Utilities::getLastFileDialogPath()
{
  GlobalSettings settings;
  QString lastFileDialogPath = QDir::homePath();

  settings.beginGroup("mainwindow"); // TODO: A better name to mainwindow.
  if(settings.contains("lastFileDialogPath"))
    lastFileDialogPath = settings.value("lastFileDialogPath").toString();
  settings.endGroup();

  qCDebug(CPR_CORE) << lastFileDialogPath;

  return lastFileDialogPath;
}

void Utilities::updateLastFileDialogPath(const QString &filepath)
{
  QFileInfo fileInfo(filepath);

  GlobalSettings settings;
  settings.beginGroup("mainwindow");
  settings.setValue("lastFileDialogPath", fileInfo.absolutePath());
  settings.endGroup();
}

QStringList Utilities::splitParams(QString &params)
{
  QStringList plist;
  QRegExp rx;
  rx.setPattern("([-${}\\w\\\\:]+|\\\".*\\\")");

  int pos = 0;
  while ((pos = rx.indexIn(params, pos)) != -1)
  {
    plist << rx.cap(1);
    pos += rx.matchedLength();
  }

  return plist;
}

} } } //end namespace

