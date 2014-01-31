/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
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

QMap<QString,LanguageType> Utilities::types = createMap();

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

  qDebug() << lastFileDialogPath;

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

} } } //end namespace

