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
// END_LICENSE
#ifndef OUTLINEVIEWPLUGINFACTORY_H
#define OUTLINEVIEWPLUGINFACTORY_H

#include <extensions/IPluginFactory.h>
using namespace cpr::core;

#include "OutlineViewPlugin.h"

/*!
 \brief Handles the creation and deletion of OutlineView objects.
*/
class OutlineViewFactory : public QObject, public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES (IPluginFactory)
  Q_PLUGIN_METADATA (IID IPluginFactory_iid FILE "outline-view.json")

public:
  /*!
   * \brief Constructor.
   */
  OutlineViewFactory ();

  /*!
   * \brief Creates an OutlineViewPlugin instance.
   *
   * \return IPlugin* A NEW instance of OutlineViewPlugin.
   */
  IPlugin *createPluginInstance ();

  /*!
   * \brief Destroy an instance of OutlineViewPlugin.
   *
   * \param IPlugin* the instance that must be destroyed.
   */
  void releasePluginInstance (IPlugin *);

  /*!
   * \brief
   * \return QString
   */
  QString id () const;

  /*!
   * \brief
   * \return QIcon
   */
  QIcon icon () const;
};

#endif // OUTLINEVIEWPLUGINFACTORY_H
