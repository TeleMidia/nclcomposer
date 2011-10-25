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
#ifndef NCLTEXTUALVIEWPLUGINFACTORY_H
#define NCLTEXTUALVIEWPLUGINFACTORY_H

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;


#include "NCLTextualViewPlugin_global.h"
#include "NCLTextualViewPlugin.h"

namespace Ui {
    class TextPluginPreferencesWidget;
}

namespace composer {
    namespace plugin {
        namespace textual {
/*!
  \brief Handles the creation and deletion of NCLTextualView objects.
 */
class NCLTextualViewPluginFactory : public QObject,
                                    public IPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(IPluginFactory)

    public:
         NCLTextualViewPluginFactory();

         IPlugin* createPluginInstance();

         void releasePluginInstance(IPlugin *);

         QString id() const;

         QString name() const;

         QIcon icon() const;

         QWidget* getPreferencePageWidget();

         void setDefaultValues();

         void applyValues();

         /*!
          \brief Returns the version of Textual View implementation.

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
          \brief Returns the vendor of TextualView (i.e. Telemidia Lab).

          \return QString the name of the vendor of OutlineView.
         */
         QString vendor() {return "Telemidia Lab";}
         /*!
          \brief Returns the copyright of TextualView.

          \return QString the copyright of TextualView.
         */
         QString copyright() {return "Telemidia/PUC-Rio";}
         /*!
           \brief Returns the license of TextualView plugin (i.e. GPLv3).
           \todo The complete license description.

           \return QString the license of TextualView.
         */
         QString license() {return "LGPL";}

         /*!
           \brief Returns a description of the textual view plugin

           \return QString the description of TextualView.
         */
         QString description() {return "NCL Textual Plugins enable the user to\
                                    edit the application directly through its \
                                    source code. Additionally, this plugin \
                                    brings features like code coloring and \
                                    autocomplete.";}

         QString url() {return "http://composer.telemidia.puc-rio.br/textual";}

         QString category() {return "NCL";}

private:
    QFrame *prefPageWidget;
    Ui::TextPluginPreferencesWidget *prefPageUi;

};

}}} //end namespace

#endif // NCLTEXTUALVIEWPLUGINFACTORY_H
