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

}

IDocumentParser* NCLLanguageProfile::createDocumentParser(Document *doc)
{
    return new NCLDocumentParser(doc);
}

void NCLLanguageProfile::releaseDocumentParser (IDocumentParser *parser)
{

}

QString NCLLanguageProfile::getProfileName()
{
    return "Nested Context Language Profile";
}


Q_EXPORT_PLUGIN2(NCLLanguageProfile,NCLLanguageProfile)
