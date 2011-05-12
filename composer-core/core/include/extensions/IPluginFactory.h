#ifndef ILAYOUTPLUGIN_H
#define ILAYOUTPLUGIN_H

#include <QtPlugin>
#include <QString>
#include <QtGui/QIcon>

#include "../util/Utilities.h"
using namespace composer::core::util;

#include "IPlugin.h"
using namespace composer::core::extension::plugin;

namespace composer {
namespace core {
namespace extension {
namespace plugin {

    //!A Factory interface for build plugins instances.
    /*!
      This extension point is used by the core to build multiple
      instances of the same plugin.
      The objective is to allow the plugin manipulates different Documents
      at the same time.
      Each plugin is binded with a Document instance at setup, this whole
      process is transparent for the plugin developer. The core is responsible
      to make this bind and also responsible for call a new instance
      when its suited.
    */
    class IPluginFactory {
    public:

        virtual ~IPluginFactory() {}

        //! Through this call the core is able to create a new plugin instance
        /*!
          \return The new plugin instance.
        */
        virtual IPlugin* createPluginInstance()  = 0;

        //! When an NclDocument is closed by the user and is no longer needed,
        //! the core is going to use this call to release the plugin instance
        //! that was associated with this NclDocument. This call free
        //! the memory been used by plugins.
        /*!
          \param The plugin instance.
        */
        virtual void releasePluginInstance(IPlugin *) = 0;

        virtual QList<LanguageType> getSupportedLanguages() = 0;

        virtual QString id() const = 0;
        virtual QString name() const = 0;
        virtual QIcon icon() const = 0;

        virtual QWidget* getPreferencePageWidget() = 0;
        virtual void setDefaultValues() = 0;
        virtual void applyValues() = 0;

        /* Informações úteis que devem estar no plugin (em um XML?)
        QString name() const;
        QString version() const;
        QString compatVersion() const;
        QString vendor() const;
        QString copyright() const;
        QString license() const;
        QString description() const;
        QString url() const;
        QString category() const;
        */

};

}
}
}
}

/**
  * This is a require for the QTPlugin system work.
  * Declaring the interface that is going to be used by external plugins.
  */
Q_DECLARE_INTERFACE(IPluginFactory,
                    "br.telemidia.pucrio.composer.IPluginFactory")

#endif // ILAYOUTPLUGIN_H
