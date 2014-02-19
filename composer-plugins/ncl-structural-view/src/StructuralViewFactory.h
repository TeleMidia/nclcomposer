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
#ifndef QNSTCOMPOSERPLUGINFACTORY_H
#define QNSTCOMPOSERPLUGINFACTORY_H

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;

#include "StructuralViewPlugin.h"

//
// ATTENTION: This code needs a refactoring.
//

class StructuralViewFactory : public QObject, public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES(IPluginFactory)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "ncl-structural-view.json")
#endif

public:
  StructuralViewFactory(QObject* parent = 0);

  ~StructuralViewFactory();

  IPlugin* createPluginInstance();

  void releasePluginInstance(IPlugin *);

  QString id() const;

#if QT_VERSION < 0x050000
  /*!
   * \brief Returns the name of the structural view plug-in.
   *
   * \return QString the name of the plug-in.
   */
  QString name() { return "Structural View"; }

  /*!
     \brief Returns the version of Layout View implementation.

     \return QString the version number as string.
    */
  QString version() { return NCLCOMPOSER_PLUGINS_VERSION; }
  /*!
     \brief Returns the core version that is compatible with this plugin.

     \return QString the core version that is compatible with this plugin
               as a string.
    */
  QString compatVersion() {return "0.1";}
  /*!
     \brief Returns the vendor of Layout View (i.e. Telemidia Lab).

     \return QString the name of the vendor of Layout View.
    */
  QString vendor() {return "Telemidia Lab";}
  /*!
     \brief Returns the copyright of Layout View.

     \return QString the copyright of Layout View.
    */
  QString copyright() {return "Telemidia/PUC-Rio";}
  /*!
      \brief Returns the license of Layout View plugin (i.e. GPLv3).
      \todo The complete license description.

      \return QString the license of Layout View.
    */
  QString license() {return "GPLv3";}

  /*!
      \brief Returns a description of the Layout View

      \return QString the description of Layout View.
    */
  QString description() {return tr("Structural View enable the user to edit "
                                   "the logical structure of a NCL document "
                                   "visually.");}

  QString url() {return "http://composer.telemidia.puc-rio.br/structural";}

  QString category() {return "NCL";}
#endif
};

#endif // QNSTCOMPOSERPLUGINFACTORY_H
