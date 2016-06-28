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

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;

#include "StructuralViewPlugin.h"

class StructuralViewFactory : public QObject, public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES(IPluginFactory)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "ncl-structural-view-next.json")
#endif

public:
  StructuralViewFactory(QObject* parent = 0);
  virtual ~StructuralViewFactory();

  IPlugin* createPluginInstance();
  void releasePluginInstance(IPlugin *);
  QString id() const;

#if QT_VERSION < 0x050000
  QString name() { return "NCL Structural View Next"; }
  QString version() { return NCLCOMPOSER_PLUGINS_VERSION; }
  QString compatVersion() {return "0.2";}
  QString vendor() {return "Telemidia Lab";}
  QString copyright() {return "Telemidia/PUC-Rio";}
  QString license() {return "LGPL";}
  QString description() {return tr("NCL Structural View enable the user to edit the logical structure of a NCL document visually.");}
  QString url() {return "http://composer.telemidia.puc-rio.br/structural";}
  QString category() {return "NCL";}
#endif
};

#endif // QNSTCOMPOSERPLUGINFACTORY_H
