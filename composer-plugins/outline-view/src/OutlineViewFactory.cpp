#include "OutlineViewFactory.h"

namespace composer {
    namespace plugin {
        namespace outline {

OutlineViewFactory::OutlineViewFactory()
{
}

IPlugin* OutlineViewFactory::createPluginInstance()
{
    return new OutlineViewPlugin();
}

void OutlineViewFactory::releasePluginInstance(IPlugin *plugin)
{
    OutlineViewFactory *outlineView = qobject_cast<OutlineViewFactory*>(plugin);

    if (outlineView)
    {
        delete outlineView;
        outlineView = NULL;
    }
}

QList<LanguageType> OutlineViewFactory::getSupportedLanguages()
{
    QList<LanguageType> lTypes;
    lTypes.append(NCL);
    return lTypes;
}

QString OutlineViewFactory::id() const
{
    return "br.puc-rio.telemidia.OutlineView";
}

QString OutlineViewFactory::name() const
{
    return "Outline View";
}

QIcon OutlineViewFactory::icon() const
{
    return QIcon(":/images/ncl.png");
}

QWidget* OutlineViewFactory::getPreferencePageWidget()
{
    return NULL;
}

void OutlineViewFactory::setDefaultValues()
{

}

void OutlineViewFactory::applyValues()
{

}

Q_EXPORT_PLUGIN2(outline_view, OutlineViewFactory)

}}} //end namespace
