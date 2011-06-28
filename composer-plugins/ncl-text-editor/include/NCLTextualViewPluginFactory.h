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

         QList<LanguageType> getSupportedLanguages();

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
         QString version() { return "1.0"; }
         /*!
          \brief Returns the core version that is compatible with this plugin.

          \return QString the core version that is compatible with this plugin
                    as a string.
         */
         QString compatVersion() {return "1.0";}
         /*!
          \brief Returns the vendor of TextualView (i.e. Telemidia Lab).

          \return QString the name of the vendor of OutlineView.
         */
         QString vendor() {return "Telemidia Lab";}
         /*!
          \brief Returns the copyright of TextualView.

          \return QString the copyright of TextualView.
         */
         QString copyright() {return "Telemidia Lab";}
         /*!
           \brief Returns the license of TextualView plugin (i.e. GPLv3).
           \todo The complete license description.

           \return QString the license of TextualView.
         */
         QString license() {return "GPLv3";}

         /*!
           \brief Returns a description of the textual view plugin

           \return QString the license of TextualView.
         */
         QString description() {return "Unknown";}
         QString url() {return "Unknown";}
         QString category() {return "NCL";}

private:
    QFrame *prefPageWidget;
    Ui::TextPluginPreferencesWidget *prefPageUi;

};

}}} //end namespace

#endif // NCLTEXTUALVIEWPLUGINFACTORY_H
