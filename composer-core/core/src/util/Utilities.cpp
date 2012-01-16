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

LanguageType Utilities::getLanguageTypeByExtension(QString ext)
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

} } } //end namespace

