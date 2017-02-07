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
#ifndef NCLEXPORT_H
#define NCLEXPORT_H

#include "nclExport_global.h"
#include "nclExportPlugin.h"

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;

/*!
 * \brief Handles the creation and deletion of nclExportPlugin objects.
 */
class nclExportFactory : public QObject,
    public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES(IPluginFactory)
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "nclexport.json")

public:
  nclExportFactory();

  ~nclExportFactory();

  IPlugin* createPluginInstance();

  void releasePluginInstance(IPlugin *);


  QList<LanguageType> getSupportedLanguages();

  QString id() const ;

  QIcon icon() const;

  QWidget* getPreferencePageWidget();

#if QT_VERSION < 0x050000
  /*!
   * \brief Returns the name of the plug-in.
   *
   * \return QString the name of the plug-in.
   */
  QString name() { return tr("NCL export"); }

  /*!
   * \brief Returns the version of Debug Console View implementation.
   *
   * \return QString the version number as string.
   */
  QString version() { return NCLCOMPOSER_PLUGINS_VERSION; }

  /*!
   * \brief Returns the core version that is compatible with this plugin.
   *
   * \return QString the core version that is compatible with this plugin
   * as a string.
   */
  QString compatVersion() {return "0.1";}

  /*!
   * \brief Returns the vendor of Debug Console View (i.e. Telemidia Lab).
   *
   * \return QString the name of the vendor of OutlineView.
   */
  QString vendor() {return "Telemidia Lab";}

  /*!
   * \brief Returns the copyright of Debug Console View.
   *
   * \return QString the copyright of Debug Console View.
   */
  QString copyright() {return "Telemidia/PUC-Rio";}

  /*!
   * \brief Returns the license of Debug Console plugin (i.e. LGPL).
   * \todo The complete license description.
   * \return QString the license of Debug Console.
   */
  QString license() {return "LGPL";}

  /*!
   * \brief Returns a description of the Debug Console View
   *
   * \return QString the description of Debug Console View.
   */
  QString description() {return tr("Export NCL - export what is in the "
                                   "project to ncl file.");}

  /*!
   * \brief Returns the URL to find more information about the plugin.
   *
   * \return QString the description of Debug Console View.
   */
  QString url() {return "http://composer.telemidia.puc-rio.br/debug";}

  /*!
   * \brief Returns the plugin category.
   *
   * \return QString the description of Debug Console View.
   */
  QString category() {return tr("General");}
#endif

};

#endif // NCLEXPORT_H
