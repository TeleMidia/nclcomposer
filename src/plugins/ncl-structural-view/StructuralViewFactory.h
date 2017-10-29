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
#ifndef QNSTCOMPOSERPLUGINFACTORY_H
#define QNSTCOMPOSERPLUGINFACTORY_H

#include <extensions/IPluginFactory.h>
using namespace cpr::core;

#include "StructuralViewPlugin.h"

class StructuralViewFactory : public QObject,
    public IPluginFactoryTpl <StructuralViewPlugin>
{
  Q_OBJECT
  Q_INTERFACES (IPluginFactory)
  Q_PLUGIN_METADATA (IID IPluginFactory_iid FILE "ncl-structural-view.json")

public:
  StructuralViewFactory (QObject *parent = 0) : QObject (parent) {}
  virtual ~StructuralViewFactory () {}

  QString id () const { return "br.puc-rio.telemidia.composer.structural"; }
};

#endif // QNSTCOMPOSERPLUGINFACTORY_H
