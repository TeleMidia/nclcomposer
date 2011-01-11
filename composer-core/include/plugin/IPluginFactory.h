#ifndef ILAYOUTPLUGIN_H
#define ILAYOUTPLUGIN_H

#include <QtPlugin>
#include <QString>
#include "IPlugin.h"
using namespace composer::core::plugin;

//!  A factory interface for build plugins instances.
/*!
  This extension point is used by the core to build multiple
  instances of the same plugin.
  The objective is to allow the plugin manipulates different NclDocuments
  at the same time.
  Each plugin is binded with a NclDocument instance at setup, this whole
  process is transparent for the plugin developer. The core is responsible
  to make this bind and also responsible for call a new instance
  when its suited.
*/

namespace composer {
namespace core {
namespace plugin {

    class IPluginFactory {
    public:
        IPluginFactory();
        virtual ~IPluginFactory() = 0;
        //! Through this call the core is able to create a new plugin instance
        //!
          /*!
            \return The new plugin instace.
          */
        virtual IPlugin* createPluginInstance()  = 0;
        //! When a NclDocument is closed by the user, or is no longer needed,
        //!    the core is going to use this call to release the plugin instance
        //!    that was associated with this NclDocument. This call free
        //!    the memory been used by plugins.
          /*!
            \param The new plugin instace.
          */
        virtual void            releasePluginInstance
                                           (IPlugin *) = 0;

        virtual QString getPluginID() = 0;

};

}
}
}
/**
  * This is a require for the QTPlugin system work.
  * Declaring the interface that is going to be used by external plugins.
  */
Q_DECLARE_INTERFACE(composer::core::plugin::IPluginFactory,
                   "br.telemidia.purcrio.composer.IPluginFactory")

#endif // ILAYOUTPLUGIN_H
