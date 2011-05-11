#ifndef DEBUGCONSOLE_H
#define DEBUGCONSOLE_H

#include "DebugConsole_global.h"
#include "DebugConsolePlugin.h"

#include <core/extensions/IPluginFactory.h>
using namespace composer::core::extension::plugin;

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

         QString getPluginID();

         QString getPluginName();

         QIcon getPluginIcon();

         QWidget* getPreferencePageWidget();

         void setDefaultValues();

         void applyValues();

};

#endif // DEBUGCONSOLE_H
