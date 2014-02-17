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
#include "LayoutViewFactory.h"

NCLLayoutViewFactory::NCLLayoutViewFactory(QObject* parent)
{
  setParent(parent);
}

NCLLayoutViewFactory::~NCLLayoutViewFactory()
{

}

IPlugin* NCLLayoutViewFactory::createPluginInstance()
{
  return new NCLLayoutViewPlugin();
}

void NCLLayoutViewFactory::releasePluginInstance(IPlugin* plugin)
{
  delete(plugin);
}

QString NCLLayoutViewFactory::id() const
{
  return "br.puc-rio.telemidia.qncllayout";
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(ncl-layout-view, NCLLayoutViewFactory)
#endif
