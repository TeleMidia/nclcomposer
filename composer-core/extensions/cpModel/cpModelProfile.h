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
