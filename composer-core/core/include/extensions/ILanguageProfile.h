#ifndef ILANGUAGE_H
#define ILANGUAGE_H

#include <QtPlugin>
#include <QList>
#include <QString>
#include <QDebug>

#include "IDocumentParser.h"
using namespace composer::core::extension;

#include "../util/Utilities.h"
using namespace composer::core::util;

#include "../model/Document.h"
using namespace composer::core::model;

namespace composer {
    namespace core{
            namespace extension {
                    class ILanguageProfile {
                        public:
                            virtual ~ILanguageProfile() {}
                            virtual LanguageType
                                    getLanguageType()  = 0;
                            virtual QString getProfileName() = 0;
                            virtual QList<QString>
                                    getOutputDocumentTypes() = 0;
                            virtual IDocumentParser*
                                    createDocumentParser(Document *doc) = 0;
                            virtual void releaseDocumentParser
                                    (IDocumentParser *parser) = 0;
                    };
                }
        }
}


/**
  * This is a require for the QTPlugin system work.
  * Declaring the interface that is going to be used by external plugins.
  */
Q_DECLARE_INTERFACE(ILanguageProfile,
                   "br.telemidia.purcrio.composer.ILanguage")

#endif // ILANGUAGE_H
