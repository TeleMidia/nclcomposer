#include "include/util/DocumentParserFactory.h"

DocumentParserFactory::DocumentParserFactory()
{

}

DocumentParserFactory::~DocumentParserFactory()
{

}

IPlugin* DocumentParserFactory::createPluginInstance() {
    qDebug() << "DocumentParserFactory::createPluginInstance()";
    return new DocumentParser();
}

void     DocumentParserFactory::releasePluginInstance(IPlugin *instance) {
    if (instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

QString  DocumentParserFactory::getPluginID(){
    return "composer.core.util.DocumentParser";
}
