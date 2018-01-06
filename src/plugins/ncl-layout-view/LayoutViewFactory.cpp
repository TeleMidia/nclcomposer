/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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

NCLLayoutViewFactory::NCLLayoutViewFactory (QObject *parent)
{
  setParent (parent);

  QStringList default_resolutions (
      { "640x480", "800x600", "1024x768",   // 4x3
        "854x480", "1280x720", "1920x1080", // 16x9
        "320x400" }                         // other
      );

  Preferences::instance ()->registerPreference (
      "cpr.layout.resolutions",
      new Preference ("resolutions", default_resolutions, "Layout"));
}
