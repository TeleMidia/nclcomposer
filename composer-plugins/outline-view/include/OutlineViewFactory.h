#ifndef OUTLINEVIEWPLUGINFACTORY_H
#define OUTLINEVIEWPLUGINFACTORY_H

#include <core/extensions/IPluginFactory.h>
using namespace composer::core::extension::plugin;


#include "OutlineViewPlugin.h"

class OutlineViewFactory : public QObject,
                                                      public IPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(IPluginFactory)

    public:
         OutlineViewFactory();

         IPlugin* createPluginInstance();

         void releasePluginInstance(IPlugin *);

         QList<LanguageType> getSupportLanguages();

         QString getPluginID();

         QString getPluginName();

         QIcon getPluginIcon();

         QWidget* getPreferencePageWidget();

         void setDefaultValues();

         void applyValues();

};

#endif // OUTLINEVIEWPLUGINFACTORY_H
