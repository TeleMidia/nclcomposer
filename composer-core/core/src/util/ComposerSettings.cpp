/* Copyright (c) 2011 Telemidia/PUC-Rio.
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
ComposerSettings::ComposerSettings() : QSettings ("br.puc-rio.telemidia", "Composer")
#else
ComposerSettings::ComposerSettings() : QSettings(QSettings::IniFormat, QSettings::UserScope, "telemidia", "composer")
#endif

{

}

} } } // end namespace
