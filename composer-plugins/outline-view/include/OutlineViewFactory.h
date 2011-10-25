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
//END_LICENSE
#ifndef OUTLINEVIEWPLUGINFACTORY_H
#define OUTLINEVIEWPLUGINFACTORY_H

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;


#include "OutlineViewPlugin.h"

namespace composer {
    namespace plugin {
        namespace outline {

/*!
 \brief Handles the creation and deletion of OutlineView objects.

*/
class OutlineViewFactory : public QObject, public IPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(IPluginFactory)

    public:
         /*!
          \brief Constructor.

         */
         OutlineViewFactory();

         /*!
          \brief Creates an OutlineViewPlugin instance.

          \return IPlugin* One NEW instance of OutlineViewPlugin.
         */
         IPlugin* createPluginInstance();

         /*!
          \brief Destroy an instance of OutlineViewPlugin.

          \param IPlugin* the instance that must be destroyed.
         */
         void releasePluginInstance(IPlugin *);

         /*!
          \brief

          \return QString
         */
         QString id() const;
         /*!
          \brief

          \return QString
         */
         QString name() const;
         /*!
          \brief

          \return QIcon
         */
         QIcon icon() const;
         /*!
          \brief Returns the version of Outline View implementation.

          \return QString the version number as string.
         */
         QString version() { return "0.1"; }
         /*!
          \brief Returns the core version that is compatible with this plugin.

          \return QString the core version that is compatible with this plugin
                    as a string.
         */
         QString compatVersion() {return "0.1";}
         /*!
          \brief Returns the vendor of Outline View (i.e. Telemidia Lab).

          \return QString the name of the vendor of OutlineView.
         */
         QString vendor() {return "Telemidia Lab";}
         /*!
          \brief Returns the copyright of Outline View.

          \return QString the copyright of Outline View.
         */
         QString copyright() {return "Telemidia/PUC-Rio";}
         /*!
           \brief Returns the license of Outline View plugin (i.e. GPLv3).
           \todo The complete license description.

           \return QString the license of Outline View.
         */
         QString license() {return "LGPL";}

         /*!
           \brief Returns a description of the Outline View

           \return QString the description of Outline View.
         */
         QString description() {return "Outline View enables the user navigates\
                                in the project as a tree.";}

         QString url() {return "http://composer.telemidia.puc-rio.br/outline";}

         QString category() {return "NCL";}
};

}}} //end namespace

#endif // OUTLINEVIEWPLUGINFACTORY_H
