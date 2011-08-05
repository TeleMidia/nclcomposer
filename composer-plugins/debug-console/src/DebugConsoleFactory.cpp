/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "DebugConsoleFactory.h"

namespace composer {
    namespace plugin {
        namespace debug {

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

QString DebugConsoleFactory::id() const
{
    return "br.puc-rio.telemidia.DebugConsole";
}

QString DebugConsoleFactory::name() const
{
    return "DebugConsole";
}

QIcon DebugConsoleFactory::icon()  const
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

} } } //end namespace
