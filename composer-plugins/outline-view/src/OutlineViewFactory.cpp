/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
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

Q_EXPORT_PLUGIN2(outline_view, OutlineViewFactory)

}}} //end namespace
