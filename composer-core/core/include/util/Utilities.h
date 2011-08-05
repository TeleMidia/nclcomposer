/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QString>
#include <QMap>

typedef enum {
      CPR,
      NCL,
      SMIL,
      HTML,
      NONE
} LanguageType;

namespace composer {
    namespace core {
        namespace util {

class Utilities
{

private:
    static QMap<QString,LanguageType> types;

public:
    static LanguageType getLanguageTypeByExtension(QString ext);
    static QString getExtensionForLanguageType(LanguageType type);

};

} } } //end namespace

#endif // DEFINITIONS_H
