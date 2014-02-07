/*
 * Copyright 2011-2013 TeleMidia/PUC-Rio.
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

QString OutlineViewFactory::id() const
{
  return "br.puc-rio.telemidia.OutlineView";
}

QIcon OutlineViewFactory::icon() const
{
  return QIcon(":/images/ncl.png");
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(outline_view, OutlineViewFactory)
#endif
