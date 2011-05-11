#include "layoutpluginfactory.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

LayoutPluginFactory::LayoutPluginFactory()
{

}

LayoutPluginFactory::~LayoutPluginFactory()
{

}

IPlugin* LayoutPluginFactory::createPluginInstance()
{
    return new LayoutPlugin();
}

void LayoutPluginFactory::releasePluginInstance(IPlugin* plugin)
{
    if (plugin != NULL){
        delete(plugin);
    }
}

QList<LanguageType> LayoutPluginFactory::getSupportedLanguages()
{
    QList<LanguageType> lTypes;
    lTypes.append(NCL);
    return lTypes;
}

QString LayoutPluginFactory::getPluginID()
{
    return "br.puc-rio.telemidia.nclplugin-layout";
}

QString LayoutPluginFactory::getPluginName()
{
    return "NCL Layout View";
}

QIcon LayoutPluginFactory::getPluginIcon()
{
    return QIcon(":/images/layout");
}

QWidget* LayoutPluginFactory::getPreferencePageWidget()
{
    return new QWidget();
}

void LayoutPluginFactory::setDefaultValues()
{

}

void LayoutPluginFactory::applyValues()
{

}

Q_EXPORT_PLUGIN2(nclplugin-layout,LayoutPluginFactory);

}
}
}
}
}
