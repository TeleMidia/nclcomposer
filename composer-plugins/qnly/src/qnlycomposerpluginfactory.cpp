#include "qnlycomposerpluginfactory.h"

namespace composer {
    namespace plugin {
        namespace layout {

QnlyComposerPluginFactory::QnlyComposerPluginFactory(QObject* parent)
{
    setParent(parent);
}

QnlyComposerPluginFactory::~QnlyComposerPluginFactory()
{

}

IPlugin* QnlyComposerPluginFactory::createPluginInstance()
{
    return new QnlyComposerPlugin();
}

void QnlyComposerPluginFactory::releasePluginInstance(IPlugin* plugin)
{
    delete(plugin);
}

QList<LanguageType> QnlyComposerPluginFactory::getSupportedLanguages()
{
    QList<LanguageType> lTypes;
    lTypes.append(NCL);
    return lTypes;
}

QString QnlyComposerPluginFactory::id() const
{
    return "br.puc-rio.telemidia.qncllayout";
}

QString QnlyComposerPluginFactory::name() const
{
    return "Layout View";
}

QIcon QnlyComposerPluginFactory::icon() const
{
    return QIcon();
}

QWidget* QnlyComposerPluginFactory::getPreferencePageWidget()
{
    return NULL;
}

void QnlyComposerPluginFactory::setDefaultValues()
{

}

void QnlyComposerPluginFactory::applyValues()
{

}

Q_EXPORT_PLUGIN2(QnclLayout, QnlyComposerPluginFactory)

} } } //end namespace
