/* Copyright (c) 2011-2013 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef ComposerSettings_H
#define ComposerSettings_H

#include "ComposerCoreControl_global.h"
#include <QSettings>

namespace composer {
    namespace core {
        namespace util {

class COMPOSERCORESHARED_EXPORT GlobalSettings: public QSettings
{
public:
  GlobalSettings();

  void updateWithDefaults(const QString &dataPath);
};

class COMPOSERCORESHARED_EXPORT ProjectSettings: public QSettings
{
public:
  ProjectSettings(const QString &project);
};

} } } // end namespace

#endif // ComposerSettings_H
