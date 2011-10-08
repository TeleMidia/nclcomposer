/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
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

Q_EXPORT_PLUGIN2(PropertiesView, PropertiesViewFactory)

} } } //end namespace



