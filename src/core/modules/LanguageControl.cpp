/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "modules/LanguageControl.h"

#include <QApplication>

namespace composer {
  namespace core {

INIT_SINGLETON (LanguageControl)

LanguageControl::LanguageControl()
{

}

LanguageControl::~LanguageControl()
{
  QMap<LanguageType, ILanguageProfile*>::iterator it;

  for(it = profiles.begin(); it != profiles.end(); it++)
  {
    ILanguageProfile *pf = it.value();
    assert(pf != NULL);
    delete pf;
    pf = NULL;
  }
  profiles.clear();
}

bool LanguageControl::removeProfile(LanguageType type)
{
  if (!profiles.contains(type))
    return false;

  ILanguageProfile *lp = profiles[type];
  assert(lp != NULL);
  if (lp == NULL)
    return false;

  delete lp;
  lp = NULL;
  profiles.remove(type);

  return true;
}

ILanguageProfile* LanguageControl::loadProfile(const QString &fileName)
{
  ILanguageProfile *lProfile = NULL;
  QPluginLoader loader(fileName);

  QObject *profile = loader.instance();
  if (profile)
  {
    lProfile = qobject_cast<ILanguageProfile*> (profile);
    if (lProfile)
    {
      LanguageType type = lProfile->getLanguageType();
      if (profiles.contains(type))
      {
        qCDebug(CPR_CORE)
            << "LanguageControl::loadProfiles Profile for language ("
            << Utilities::getExtensionForLanguageType(type) << ") already"
            << "exists";
      }
      else
      {
        profiles.insert(type, lProfile);
        qCDebug(CPR_CORE) << fileName << " loaded --- "
                            << "languageProfile = " << lProfile;
      }
    }
    else
    {
      qCDebug(CPR_CORE) <<  fileName << "is not a ILanguageProfile.";
    }
  }
  else
    qCDebug(CPR_CORE) << "Failed to load languageProfile (" << fileName <<")"
                        << loader.errorString();

  return lProfile;
}

void LanguageControl::loadProfiles()
{
  GlobalSettings settings;
  QStringList extPaths = settings.getExtensionsPaths();

  // add all the paths to LibraryPath, i.e., plugins are allowed to install
  // dll dependencies in the extensions path.
  for(const QString &extDir: extPaths)
    QApplication::addLibraryPath(extDir + "/");

  // foreach path where extensions can be installed, try to load language profiles.
  for(const QString &extDir: extPaths)
    LanguageControl::getInstance()->loadProfiles(extDir);
}

void LanguageControl::loadProfiles(const QString &profilesDirPath)
{
  QDir profileDir = QDir(profilesDirPath);
  qCDebug(CPR_CORE) << "Trying to load language profiles from "
                    << profilesDirPath;
  if(!profileDir.exists())
  {
    qCDebug(CPR_CORE) << profilesDirPath << " directory does not exist!";
    return;
  }

  QStringList filter;
  filter.append("*.so");
  filter.append("*.dylib");
  filter.append("*.dll");
  profileDir.setNameFilters(filter);

  for ( const QString& fileName:
            profileDir.entryList(QDir::Files | QDir::NoSymLinks) )
  {
    loadProfile(profileDir.absoluteFilePath(fileName));
  }
}

ILanguageProfile* LanguageControl::getProfileFromType(LanguageType type)
{
  if (profiles.contains(type))
    return profiles[type];
  else
    return NULL;
}

QList<ILanguageProfile*> LanguageControl::getLoadedProfiles()
{
  QMap<LanguageType, ILanguageProfile*>::iterator it;
  QList<ILanguageProfile*> list;

  for (it = profiles.begin(); it != profiles.end(); it++)
  {
    list.append(it.value());
  }
  return list;
}

} } //end namespace


