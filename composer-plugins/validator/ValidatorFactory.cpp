#include "ValidatorFactory.h"

namespace composer{
    namespace plugin{
        namespace validator{

ValidatorFactory::ValidatorFactory(QWidget *parent)
    : QObject(parent)
{
}

ValidatorFactory::~ValidatorFactory()
{
}

IPlugin * ValidatorFactory::createPluginInstance()
{
    return new ValidatorPlugin();
}

void ValidatorFactory::releasePluginInstance(IPlugin *plugin)
{
    ValidatorPlugin *validator = qobject_cast <ValidatorPlugin *> (plugin);

    if (validator){
        delete validator;
        validator = NULL;
    }
}


QList <LanguageType> ValidatorFactory::getSupportedLanguages()
{
    QList <LanguageType> list;
    list.append(NCL);
    return list;
}


Q_EXPORT_PLUGIN2(ValidatorFactory, ValidatorFactory)

        }
    }
}
