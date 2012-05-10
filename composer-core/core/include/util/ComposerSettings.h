/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef COMPOSERSETTINGS_H
#define COMPOSERSETTINGS_H

#include "ComposerCoreControl_global.h"
#include <QSettings>

namespace composer {
    namespace core {
        namespace util {

class COMPOSERCORESHARED_EXPORT ComposerSettings: public QSettings
{
public:
  ComposerSettings();
};

} } } // end namespace

#endif // COMPOSERSETTINGS_H
