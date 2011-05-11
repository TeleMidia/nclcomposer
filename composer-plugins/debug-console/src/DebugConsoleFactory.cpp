#include "DebugConsoleFactory.h"


DebugConsoleFactory::DebugConsoleFactory()
{

}

DebugConsoleFactory::~DebugConsoleFactory()
{

}

IPlugin* DebugConsoleFactory::createPluginInstance()
{
    return new DebugConsolePlugin();
}

void DebugConsoleFactory::releasePluginInstance(IPlugin *plugin)
{
    DebugConsolePlugin *debug = qobject_cast<DebugConsolePlugin*>(plugin);

    if (debug)
    {
        delete debug;
        debug = NULL;
    }

}

QList<LanguageType> DebugConsoleFactory::getSupportedLanguages()
{
    QList<LanguageType> lTypes;
    lTypes.append(NCL);
    return lTypes;
}

QString DebugConsoleFactory::getPluginID()
{
    return "br.puc-rio.telemidia.DebugConsole";
}

QString DebugConsoleFactory::getPluginName()
{
    return "DebugConsole";
}

QIcon DebugConsoleFactory::getPluginIcon()
{
    return QIcon(":/images/icon.png");
}

QWidget* DebugConsoleFactory::getPreferencePageWidget()
{
    return NULL;
}

void DebugConsoleFactory::setDefaultValues()
{

}

void DebugConsoleFactory::applyValues()
{

}

Q_EXPORT_PLUGIN2(DebugConsole,DebugConsoleFactory)
