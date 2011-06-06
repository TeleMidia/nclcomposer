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

};

} } } //end namespace

#endif // DEBUGCONSOLE_H
