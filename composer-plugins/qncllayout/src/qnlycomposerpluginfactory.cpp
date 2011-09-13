/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
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
