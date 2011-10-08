/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
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
