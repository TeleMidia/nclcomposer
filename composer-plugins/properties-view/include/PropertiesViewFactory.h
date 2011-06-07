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

};

} } } //end namespace

#endif // DEBUGCONSOLE_H
