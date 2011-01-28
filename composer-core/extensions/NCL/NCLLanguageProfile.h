#ifndef NCLLANGUAGEPROFILE_H
#define NCLLANGUAGEPROFILE_H

#include "NCLLanguageProfile_global.h"

#include "NCLDocumentParser.h"

#include <core/extensions/ILanguageProfile.h>
using namespace composer::core::extension;


    class NCLLanguageProfile : public QObject,
                               public ILanguageProfile {
            Q_OBJECT
            Q_INTERFACES(ILanguageProfile);
    public:
        NCLLanguageProfile();
        ~NCLLanguageProfile();
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


#endif // NCLLANGUAGEPROFILE_H
