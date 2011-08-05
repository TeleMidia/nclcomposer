/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef CPMODELPROFILE_H
#define CPMODELPROFILE_H

#include <core/extensions/ILanguageProfile.h>
using namespace composer::core::extension;

class cpModelProfile : public QObject,
                       public ILanguageProfile
{
    Q_OBJECT
    Q_INTERFACES(ILanguageProfile);
public:
    cpModelProfile();
    ~cpModelProfile();
    LanguageType
            getLanguageType();
    QList<QString>
            getOutputDocumentTypes();
    IDocumentParser*
            createDocumentParser(Document *doc);
    void releaseDocumentParser
            (IDocumentParser *parser);
    QString getProfileName();
};

#endif // CPMODELPROFILE_H
