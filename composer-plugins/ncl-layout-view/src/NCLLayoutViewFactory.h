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
#ifndef QNLYCOMPOSERPLUGINFACTORY_H
#define QNLYCOMPOSERPLUGINFACTORY_H

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;

#include "NCLLayoutViewPlugin.h"

class NCLLayoutViewFactory : public QObject, public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES(IPluginFactory)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "ncl-layout-view.json")
#endif

public:
  explicit NCLLayoutViewFactory(QObject* parent = 0);
  virtual ~NCLLayoutViewFactory();

  IPlugin* createPluginInstance();

  void releasePluginInstance(IPlugin *);

  QString id() const;

#if QT_VERSION < 0x050000
  /*!
   * \brief Returns the name of layout view plug-in.
   *
   * \return QString the name of the plug-in.
   */
  QString name() { return "Layout View"; }

  /*!
   * \brief Returns the version of Layout View implementation.
   *
   * \return QString the version number as string.
   */
  QString version() { return NCLCOMPOSER_PLUGINS_VERSION; }

  /*!
   * \brief Returns the core version that is compatible with this plugin.
   *
   * \return QString the core version that is compatible with this plugin
   *          as a string.
   */
  QString compatVersion() {return "0.1";}

  /*!
   * \brief Returns the vendor of Layout View (i.e. Telemidia Lab).
   *
   * \return QString the name of the vendor of Layout View.
   */
  QString vendor() {return "Telemidia Lab";}

  /*!
   * \brief Returns the copyright of Layout View.
   *
   * \return QString the copyright of Layout View.
   */
  QString copyright() {return "Telemidia/PUC-Rio";}

  /*!
   * \brief Returns the license of Layout View plugin (i.e. GPLv3).
   *
   * \todo The complete license description.
   * \return QString the license of Layout View.
   */
  QString license() {return "LGPL";}

  /*!
   * \brief Returns a description of the Layout View
   *
   * \return QString the description of Layout View.
   */
  QString description() {return tr("Layout View enable the user to edit the "
                                   "NCL regions visually.");}

  /*!
   * \brief url
   * \return
   */
  QString url() {return "http://composer.telemidia.puc-rio.br/layout";}

  /*!
   * \brief category
   * \return
   */
  QString category() {return "NCL";}
#endif
};

#endif // QNLYCOMPOSERPLUGINFACTORY_H
