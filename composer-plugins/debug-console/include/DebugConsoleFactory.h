#ifndef DEBUGCONSOLE_H
#define DEBUGCONSOLE_H

#include "DebugConsole_global.h"
#include "DebugConsolePlugin.h"

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;

namespace composer {
    namespace plugin {
        namespace debug {

class DebugConsoleFactory : public QObject,
                            public IPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(IPluginFactory)
    public:
         DebugConsoleFactory();

         ~DebugConsoleFactory();

         IPlugin* createPluginInstance();

         void releasePluginInstance(IPlugin *);

         QList<LanguageType> getSupportedLanguages();

         QString id() const;

         QString name() const;

         QIcon icon() const;

         QWidget* getPreferencePageWidget();

         void setDefaultValues();

         void applyValues();

         /*!
          \brief Returns the version of Debug Console View implementation.

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
          \brief Returns the vendor of Debug Console View (i.e. Telemidia Lab).

          \return QString the name of the vendor of OutlineView.
         */
         QString vendor() {return "Telemidia Lab";}
         /*!
          \brief Returns the copyright of Debug Console View.

          \return QString the copyright of Debug Console View.
         */
         QString copyright() {return "Telemidia/PUC-Rio";}
         /*!
           \brief Returns the license of Debug Console plugin (i.e. GPLv3).
           \todo The complete license description.

           \return QString the license of Debug Console.
         */
         QString license() {return "GPLv3";}

         /*!
           \brief Returns a description of the Debug Console View

           \return QString the description of Debug Console View.
         */
         QString description() {return "Debug Console View prints all the \
                                   messages send by composer-core to plugins.";}

         QString url() {return "http://composer.telemidia.puc-rio.br/debug";}

         QString category() {return "Debug";}

};

} } } //end namespace

#endif // DEBUGCONSOLE_H
