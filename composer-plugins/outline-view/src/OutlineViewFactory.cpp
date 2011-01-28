#include "OutlineViewFactory.h"

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

QList<LanguageType> OutlineViewFactory::getSupportLanguages()
{
    QList<LanguageType> lTypes;
    lTypes.append(NCL);
    return lTypes;
}

QString OutlineViewFactory::getPluginID()
{
    return "br.puc-rio.telemidia.OutlineView";
}

QString OutlineViewFactory::getPluginName()
{
    return "Outline View";
}

QIcon OutlineViewFactory::getPluginIcon()
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
