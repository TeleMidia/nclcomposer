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

#include "modules/LanguageControl.h"

#include <QApplication>

CPR_CORE_BEGIN_NAMESPACE

INIT_SINGLETON (LanguageControl)

LanguageControl::LanguageControl()
{

}

LanguageControl::~LanguageControl()
{
  QMap<LanguageType, ILanguageProfile*>::iterator it;

  for(it = _profiles.begin(); it != _profiles.end(); it++)
  {
    ILanguageProfile *pf = it.value();
    assert(pf != nullptr);
    delete pf;
    pf = nullptr;
  }
  _profiles.clear();
}

bool LanguageControl::removeProfile(LanguageType type)
{
  if (!_profiles.contains(type))
    return false;

  ILanguageProfile *lp = _profiles[type];
  assert(lp != nullptr);
  if (lp == nullptr)
    return false;

  delete lp;
  lp = nullptr;
  _profiles.remove(type);

  return true;
}

ILanguageProfile* LanguageControl::loadProfile(const QString &fileName)
{
  ILanguageProfile *lProfile = nullptr;
  QPluginLoader loader(fileName);

  QObject *profile = loader.instance();
  if (profile)
  {
    lProfile = qobject_cast<ILanguageProfile*> (profile);
    if (lProfile)
    {
      LanguageType type = lProfile->getLanguageType();
      if (_profiles.contains(type))
      {
        qCDebug(CPR_CORE)
            << "LanguageControl::loadProfiles Profile for language ("
            << Utilities::getExtensionForLanguageType(type) << ") already"
            << "exists";
      }
      else
      {
        _profiles.insert(type, lProfile);
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
  if (_profiles.contains(type))
    return _profiles[type];
  else
    return nullptr;
}

QList<ILanguageProfile*> LanguageControl::getLoadedProfiles()
{
  QMap<LanguageType, ILanguageProfile*>::iterator it;
  QList<ILanguageProfile*> list;

  for (it = _profiles.begin(); it != _profiles.end(); it++)
  {
    list.append(it.value());
  }
  return list;
}

CPR_CORE_END_NAMESPACE
