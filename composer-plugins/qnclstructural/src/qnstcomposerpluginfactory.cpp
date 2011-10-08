/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "qnstcomposerpluginfactory.h"

namespace composer {
    namespace plugin {
        namespace layout {

QnstComposerPluginFactory::QnstComposerPluginFactory(QObject* parent)
{
    setParent(parent);
}

QnstComposerPluginFactory::~QnstComposerPluginFactory()
{

}

IPlugin* QnstComposerPluginFactory::createPluginInstance()
{
    return new QnstComposerPlugin();
}

void QnstComposerPluginFactory::releasePluginInstance(IPlugin* plugin)
{
    delete(plugin);
}

QString QnstComposerPluginFactory::id() const
{
    return "br.puc-rio.telemidia.qnst";
}

QString QnstComposerPluginFactory::name() const
{
    return "Structural View";
}

Q_EXPORT_PLUGIN2(Qnst, QnstComposerPluginFactory)

} } } //end namespace
