#include "NCLLanguageProfile.h"

#include "NCLStructure.h"
using namespace composer::language;

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

IDocumentParser* NCLLanguageProfile::createParser (Project *project)
{
    return new NCLDocumentParser (project);
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

map <QString, map <QString, char> *> *NCLLanguageProfile::getNesting()
{
    return NCLStructure::getInstance()->getNesting();
}

map <QString, bool> *NCLLanguageProfile::getAttributes (QString element)
{
    return NCLStructure::getInstance()->getAttributes(element);
}

map <QString, char> *NCLLanguageProfile::getChildren (QString tagname)
{
    return NCLStructure::getInstance()->getChildren(tagname);
}

vector <AttributeReferences *>
        NCLLanguageProfile::getReferences (QString element, QString attr)
{
    return NCLStructure::getInstance()->getReferences(element, attr);
}


Q_EXPORT_PLUGIN2(NCLLanguageProfile, NCLLanguageProfile)
