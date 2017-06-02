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

#ifndef ComposerSettings_H
#define ComposerSettings_H

#include "ComposerCore_global.h"
#include <QSettings>

CPR_CORE_BEGIN_NAMESPACE

class COMPOSERCORESHARED_EXPORT GlobalSettings: public QSettings
{
public:
  GlobalSettings();

  void loadDefaults(const QString &dataPath);
  void addPlatformDefaults();
  QStringList getExtensionsPaths();
};

class COMPOSERCORESHARED_EXPORT ProjectSettings: public QSettings
{
public:
  ProjectSettings(const QString &project);
};

CPR_CORE_END_NAMESPACE

#endif // ComposerSettings_H
