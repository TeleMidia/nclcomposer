/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>. 
 */
#include "PropertiesViewFactory.h"

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



