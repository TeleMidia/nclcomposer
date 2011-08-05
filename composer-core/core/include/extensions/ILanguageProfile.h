/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef ILANGUAGE_H
#define ILANGUAGE_H

#include <QtPlugin>
#include <QList>
#include <QString>
#include <QDebug>

#include "IDocumentParser.h"
using namespace composer::extension;

#include "../util/Utilities.h"
using namespace composer::core::util;

#include "../model/Project.h"
using namespace composer::core::model;

#include "../util/AttributeReferences.h"
using namespace composer::util;

namespace composer {
    namespace extension {
/**!
  * This class must be implemented by anyone that want to support
  * a new language in Composer.
  */
class ILanguageProfile {
public:
    virtual ~ILanguageProfile() {}

    virtual LanguageType
            getLanguageType()  = 0;

    virtual QString
            getProfileName() = 0;

    virtual QList<QString>
            getOutputDocumentTypes() = 0;

    virtual IDocumentParser*
            createParser(Project *project) = 0;

    virtual void releaseDocumentParser
            (IDocumentParser *parser) = 0;

    //FUNCTIONS RELATED TO LANGUAGE STRUCTURE
    virtual map <QString, map <QString, char> *>
            *getNesting() = 0;

    virtual map <QString, bool>
            *getAttributes (QString element) = 0;

    virtual map <QString, char>
            *getChildren (QString tagname) = 0;

    virtual vector <AttributeReferences *>
            getReferences (QString element, QString attr) = 0;
};
} } //end namespace


/**
  * This is require for the QTPlugin system work.
  * Declaring the interface that is going to be used by external plugins.
  */
Q_DECLARE_INTERFACE(ILanguageProfile,
                    "br.telemidia.purcrio.composer.ILanguage")
#endif // ILANGUAGE_H
