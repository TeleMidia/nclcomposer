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
#include <util/Preferences.h>

NCLLayoutViewFactory::NCLLayoutViewFactory(QObject* parent)
{
  setParent(parent);

  Preferences::getInstance ()->registerPreference (
      "cpr.layout.resolutions",
      new Preference ("resolutions", "640x480,800x600,1024x768,854x480,1280x720,1920x1080,320x400", "Layout"));
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

