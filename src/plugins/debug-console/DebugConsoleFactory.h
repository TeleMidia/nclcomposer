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
#ifndef DEBUGCONSOLE_H
#define DEBUGCONSOLE_H

#include "DebugConsolePlugin.h"
#include "DebugConsole_global.h"

#include <extensions/IPluginFactory.h>
using namespace cpr::core;

/*!
 * \brief Handles the creation and deletion of DebugConsolePlugin objects.
 */
class DebugConsoleFactory : public QObject, public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES (IPluginFactory)
  Q_PLUGIN_METADATA (IID IPluginFactory_iid FILE "debug-console.json")

public:
  DebugConsoleFactory ();
  ~DebugConsoleFactory ();

  IPlugin *createPluginInstance ();
  void releasePluginInstance (IPlugin *);
  QList<LanguageType> getSupportedLanguages ();

  QString id () const;
  QIcon icon () const;
};

#endif // DEBUGCONSOLE_H
