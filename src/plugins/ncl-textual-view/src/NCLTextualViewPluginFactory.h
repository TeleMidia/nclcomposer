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
#ifndef NCLTEXTUALVIEWPLUGINFACTORY_H
#define NCLTEXTUALVIEWPLUGINFACTORY_H

#include "NCLTextualViewPlugin_global.h"

#include <extensions/IPluginFactory.h>
using namespace cpr::core;

#include "NCLTextualViewPlugin.h"
#include "NCLTextualViewPlugin_global.h"

namespace Ui
{
class TextPluginPreferencesWidget;
}

/*!
  \brief Handles the creation and deletion of NCLTextualView objects.
 */
class NCLTextualViewPluginFactory : public QObject, public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES (IPluginFactory)
  Q_PLUGIN_METADATA (IID IPluginFactory_iid FILE "ncl-textual-view.json")

public:
  NCLTextualViewPluginFactory ();

  IPlugin *createPluginInstance ();

  void releasePluginInstance (IPlugin *);

  QString id () const;

  QIcon icon () const;

  QWidget *getPreferencePageWidget ();

  void setDefaultValues ();

  void applyValues ();

private:
  QFrame *_prefPageWidget;
  Ui::TextPluginPreferencesWidget *_prefPageUi;
};

#endif // NCLTEXTUALVIEWPLUGINFACTORY_H
