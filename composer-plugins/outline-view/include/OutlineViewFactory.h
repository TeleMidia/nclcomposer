//END_LICENSE
#ifndef OUTLINEVIEWPLUGINFACTORY_H
#define OUTLINEVIEWPLUGINFACTORY_H

#include <core/extensions/IPluginFactory.h>
using namespace composer::core::extension::plugin;


#include "OutlineViewPlugin.h"

/*!
 \brief This class handles the creation and deletion of OutlineView objects.

*/
class OutlineViewFactory : public QObject, public IPluginFactory
{
    Q_OBJECT
    Q_INTERFACES(IPluginFactory)

    public:
         /*!
          \brief

         */
         OutlineViewFactory();

         /*!
          \brief Creates

          \return IPlugin *
         */
         IPlugin* createPluginInstance();

         /*!
          \brief Destroy an instance of OutileView class.

          \param
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
          \brief Returns the version of OutlineView implementation.

          \return QString the version number as string.
         */
         QString version() { return "1.0 (1.0)"; }
         /*!
          \brief Returns the core version that is compatible with this plugin.

          \return QString the core version that is compatible with this plugin
                    as a string.
         */
         QString compatVersion() {return "1.0";}
         /*!
          \brief Returns the vendor of OutlineView (i.e. Telemidia Lab).

          \return QString the name of the vendor of OutlineView.
         */
         QString vendor() {return "Telemidia Lab";}
         /*!
          \brief Returns the copyright of OutlineView.

          \return QString the copyright of OutlineView.
         */
         QString copyright() {return "Telemidia Lab";}
         /*!
           \brief Returns the license of OutlineView plugin (i.e. GPLv3).
           \todo The complete license description.

           \return QString the license of OutlineView.
         */
         QString license() {return "GPLv3";}
         QString description() {return "Unknown";}
         QString url() {return "Unknown";}
         QString category() {return "NCL";}

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

};

#endif // OUTLINEVIEWPLUGINFACTORY_H
