#ifndef ILAYOUTPLUGIN_H
#define ILAYOUTPLUGIN_H

#include <QtPlugin>
#include <QString>

#include "../../util/Utilities.h"
using namespace composer::core::util;

#include "IPlugin.h"
using namespace composer::core::extension::plugin;

//!  A factory interface for build plugins instances.
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

namespace composer {
namespace core {
namespace extension {
namespace plugin {

    class IPluginFactory {
    public:

        virtual ~IPluginFactory() {}

        //! Through this call the core is able to create a new plugin instance
        //!
          /*!
            \return The new plugin instance.
          */
        virtual IPlugin* createPluginInstance()  = 0;

        //! When a NclDocument is closed by the user, or is no longer needed,
        //!    the core is going to use this call to release the plugin instance
        //!    that was associated with this NclDocument. This call free
        //!    the memory been used by plugins.
          /*!
            \param The plugin instance.
          */
        virtual void            releasePluginInstance
                                           (IPlugin *) = 0;

        virtual QList<LanguageType> getSupportLanguages() = 0;

        virtual QString getPluginID() = 0;


};

}
}
}
}

/**
  * This is a require for the QTPlugin system work.
  * Declaring the interface that is going to be used by external plugins.
  */
Q_DECLARE_INTERFACE(composer::core::extension::plugin::IPluginFactory,
                   "br.telemidia.purcrio.composer.IPluginFactory")

#endif // ILAYOUTPLUGIN_H
