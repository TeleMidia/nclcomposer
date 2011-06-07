#include "PropertiesViewFactory.h"
namespace composer {
    namespace plugin {
        namespace property {

PropertiesViewFactory::PropertiesViewFactory()
{

}

PropertiesViewFactory::~PropertiesViewFactory()
{

}

IPlugin* PropertiesViewFactory::createPluginInstance()
{
    return new PropertiesViewPlugin();
}

void PropertiesViewFactory::releasePluginInstance(IPlugin *plugin)
{
    PropertiesViewPlugin *debug = qobject_cast<PropertiesViewPlugin*>(plugin);

    if (debug)
    {
        delete debug;
        debug = NULL;
    }

}

QList<LanguageType> PropertiesViewFactory::getSupportedLanguages()
{
    QList<LanguageType> lTypes;
    lTypes.append(NCL);
    return lTypes;
}

QString PropertiesViewFactory::id() const
{
    return "br.puc-rio.telemidia.PropertiesView";
}

QString PropertiesViewFactory::name() const
{
    return "PropertiesView";
}

QIcon PropertiesViewFactory::icon()  const
{
    return QIcon(":/images/icon.png");
}

QWidget* PropertiesViewFactory::getPreferencePageWidget()
{
    return NULL;
}

void PropertiesViewFactory::setDefaultValues()
{

}

void PropertiesViewFactory::applyValues()
{

}

Q_EXPORT_PLUGIN2(PropertiesView, PropertiesViewFactory)

} } } //end namespace



