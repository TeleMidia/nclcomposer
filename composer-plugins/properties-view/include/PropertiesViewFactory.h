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
#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include "PropertiesView_global.h"
#include "PropertiesViewPlugin.h"

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;

/*!
 \brief Handles the creation and deletion of PropertyView objects.

*/
class PropertiesViewFactory : public QObject,
    public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES(IPluginFactory)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "properties_view.json")
#endif

public:
  /*!
   * \brief Constructor.
   */
  PropertiesViewFactory();

  /*!
   * \brief Destructor.
   */
  ~PropertiesViewFactory();

  /*!
   * \brief Creates an PropertiesViewPlugin instance.
   *
   * \return IPlugin* One NEW instance of PropertiesViewPlugin.
   */
  IPlugin* createPluginInstance();

  /*!
   * \brief Destroy an instance of PropertiesViewPlugin.
   *
   * \param IPlugin* the instance that must be destroyed.
   */
  void releasePluginInstance(IPlugin *);

  /*!
   * \brief
   *
   * \return QString
   */
  QString id() const;

  /*!
   * \brief
   *
   * \return QIcon
   */
  QIcon icon() const;

#if QT_VERSION < 0x050000
  /*!
   * \brief Returns the name of properties view plug-in.
   *
   * \return QString the name of the plug-in.
   */
  QString name() { return "Properties View"; }

  /*!
   * \brief Returns the version of Property View implementation.
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
   * \brief Returns the vendor of Property View (i.e. Telemidia Lab).
   *
   * \return QString the name of the vendor of OutlineView.
   */
  QString vendor() {return "Telemidia Lab";}

  /*!
   * \brief Returns the copyright of Property View.
   *
   * \return QString the copyright of Property View.
   */
  QString copyright() {return "Telemidia/PUC-Rio";}

  /*!
   * \brief Returns the license of Property View plugin (i.e. GPLv3).
   *
   * \todo The complete license description.
   * \return QString the license of Property View.
   */
  QString license() {return "LGPL";}

  /*!
   * \brief Returns a description of the Property View
   *
   * \return QString the description of Property View.
   */
  QString description() {return tr("Property View shows the attributes "
                                   "of a current selected entity and "
                                   "enables the user to change its "
                                   "values.");}

  /*!
   * \brief url
   * \return
   */
  QString url() {return "http://composer.telemidia.puc-rio.br/property";}

  /*!
   * \brief category
   * \return
   */
  QString category() {return "NCL";}
#endif

};

#endif // DEBUGCONSOLE_H
