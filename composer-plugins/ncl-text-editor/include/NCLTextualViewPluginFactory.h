#ifndef NCLTEXTUALVIEWPLUGINFACTORY_H
#define NCLTEXTUALVIEWPLUGINFACTORY_H

#include <core/extensions/IPluginFactory.h>
using namespace composer::core::extension::plugin;


#include "NCLTextualViewPlugin_global.h"
#include "NCLTextualViewPlugin.h"

namespace Ui {
    class TextPluginPreferencesWidget;
}

class NCLTextualViewPluginFactory : public QObject,
                                                      public IPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(IPluginFactory)
    public:
         NCLTextualViewPluginFactory();

         IPlugin* createPluginInstance();

         void releasePluginInstance(IPlugin *);

         QList<LanguageType> getSupportedLanguages();

         QString getPluginID();

         QString getPluginName();

         QIcon getPluginIcon();

         QWidget* getPreferencePageWidget();

         void setDefaultValues();

         void applyValues();

private:
    QFrame *prefPageWidget;
    Ui::TextPluginPreferencesWidget *prefPageUi;

};

#endif // NCLTEXTUALVIEWPLUGINFACTORY_H
