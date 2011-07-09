#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include "PropertiesView_global.h"
#include "PropertiesViewPlugin.h"

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;

namespace composer {
    namespace plugin {
        namespace property {

/*!
 \brief Handles the creation and deletion of PropertyView objects.

*/
class PropertiesViewFactory : public QObject,
                            public IPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(IPluginFactory)

    public:
         /*!
          \brief Constructor.
         */
         PropertiesViewFactory();

         /*!
          \brief Destructor.
         */
         ~PropertiesViewFactory();

         /*!
          \brief Creates an PropertiesViewPlugin instance.

          \return IPlugin* One NEW instance of PropertiesViewPlugin.
         */
         IPlugin* createPluginInstance();

         /*!
          \brief Destroy an instance of PropertiesViewPlugin.

          \param IPlugin* the instance that must be destroyed.
         */
         void releasePluginInstance(IPlugin *);

         /*!
          \brief

          \return QList<LanguageType>
         */
         QList<LanguageType> getSupportedLanguages();

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
          \brief

          \return QWidget *
         */
         QWidget* getPreferencePageWidget();

         /*!
          \brief

         */
         void setDefaultValues();

         /*!
          \brief

         */
         void applyValues();

         /*!
          \brief Returns the version of Property View implementation.

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
          \brief Returns the vendor of Property View (i.e. Telemidia Lab).

          \return QString the name of the vendor of OutlineView.
         */
         QString vendor() {return "Telemidia Lab";}
         /*!
          \brief Returns the copyright of Property View.

          \return QString the copyright of Property View.
         */
         QString copyright() {return "Telemidia/PUC-Rio";}
         /*!
           \brief Returns the license of Property View plugin (i.e. GPLv3).
           \todo The complete license description.

           \return QString the license of Property View.
         */
         QString license() {return "GPLv3";}

         /*!
           \brief Returns a description of the Property View

           \return QString the description of Property View.
         */
         QString description() {return "Property View shows the attributes of \
                                a current selected entity and enables the user \
                                to change its values.";}

         QString url() {return "http://composer.telemidia.puc-rio.br/debug";}

         QString category() {return "NCL";}

};

} } } //end namespace

#endif // DEBUGCONSOLE_H
