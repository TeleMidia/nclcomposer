#ifndef OUTLINEVIEWPLUGINFACTORY_H
#define OUTLINEVIEWPLUGINFACTORY_H

#include <core/extensions/IPluginFactory.h>
using namespace composer::core::extension::plugin;


#include "OutlineViewPlugin.h"

class OutlineViewFactory : public QObject, public IPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(IPluginFactory)

    public:
         OutlineViewFactory();

         IPlugin* createPluginInstance();

         void releasePluginInstance(IPlugin *);

         QList<LanguageType> getSupportedLanguages();

         QString id() const;
         QString name() const;
         QIcon icon() const;

         QString version() { return "1.0 (1.0)"; }
         QString compatVersion() {return "1.0";}
         QString vendor() {return "Telemidia Lab";}
         QString copyright() {return "Telemidia Lab";}
         QString license() {return "GPLv3";}
         QString description() {return "Unknown";}
         QString url() {return "Unknown";}
         QString category() {return "NCL";}

         QWidget* getPreferencePageWidget();
         void setDefaultValues();
         void applyValues();

};

#endif // OUTLINEVIEWPLUGINFACTORY_H
