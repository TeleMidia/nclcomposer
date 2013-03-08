/* Copyright (c) 2011-2013 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "util/ComposerSettings.h"

namespace composer {
    namespace core {
        namespace util {
#ifdef Q_WS_MAC
GlobalSettings::GlobalSettings() : QSettings ("br.puc-rio.telemidia", "Composer")
#else
GlobalSettings::GlobalSettings() : QSettings(QSettings::IniFormat, QSettings::UserScope, "telemidia", "composer")
#endif
{

}

ProjectSettings::ProjectSettings(const QString &project) : QSettings(project + ".ini", QSettings::IniFormat)
{

}

} } } // end namespace
