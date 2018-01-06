/*
 * Copyright 2011-2018 TeleMidia/PUC-Rio.
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

#include "NCLExport_global.h"
#include "NCLExportPlugin.h"

#include <extensions/IPluginFactory.h>
using namespace cpr::core;

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

};

#endif // NCLEXPORT_H
