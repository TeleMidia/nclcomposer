#include "NCLTextualViewPluginFactory.h"
#include "ui_TextualPluginPreferencesWidget.h"

NCLTextualViewPluginFactory::NCLTextualViewPluginFactory()
{
    prefPageWidget = new QFrame();
    prefPageUi = new Ui::TextPluginPreferencesWidget();
    prefPageUi->setupUi(prefPageWidget);
}

IPlugin* NCLTextualViewPluginFactory::createPluginInstance()
{
    return new NCLTextualViewPlugin();
}

void NCLTextualViewPluginFactory::releasePluginInstance(IPlugin *plugin)
{
    NCLTextualViewPlugin *textualView =
            qobject_cast<NCLTextualViewPlugin*>(plugin);

    if (textualView)
    {
        delete textualView;
        textualView = NULL;
    }
}

QList<LanguageType> NCLTextualViewPluginFactory::getSupportLanguages()
{
    QList<LanguageType> lTypes;
    lTypes.append(NCL);
    return lTypes;
}

QString NCLTextualViewPluginFactory::getPluginID()
{
    return "br.puc-rio.telemidia.NCLTextualView";
}

QString NCLTextualViewPluginFactory::getPluginName()
{
    return "NCL Textual View";
}

QIcon NCLTextualViewPluginFactory::getPluginIcon()
{
    return QIcon(":/images/ncl.png");
}

QWidget* NCLTextualViewPluginFactory::getPreferencePageWidget()
{
    return prefPageWidget;
}

void NCLTextualViewPluginFactory::setDefaultValues()
{

}

void NCLTextualViewPluginFactory::applyValues()
{

}

Q_EXPORT_PLUGIN2(ncl_textual_plugin,NCLTextualViewPluginFactory)
