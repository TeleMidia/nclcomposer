#include "NCLLanguageProfile.h"


NCLLanguageProfile::NCLLanguageProfile()
{
}

NCLLanguageProfile::~NCLLanguageProfile()
{

}

LanguageType NCLLanguageProfile::getLanguageType()
{
    return NCL;
}

QList<QString> NCLLanguageProfile::getOutputDocumentTypes()
{
    QList<QString> list;
    list.append("ncl");
    return list;
}

IDocumentParser* NCLLanguageProfile::createDocumentParser (Document *doc)
{
    return new NCLDocumentParser (doc);
}

void NCLLanguageProfile::releaseDocumentParser (IDocumentParser *parser)
{
    if (parser) {
        delete parser;
        parser = NULL;
    }
}

QString NCLLanguageProfile::getProfileName()
{
    return "Nested Context Language Profile";
}


Q_EXPORT_PLUGIN2(NCLLanguageProfile, NCLLanguageProfile)
