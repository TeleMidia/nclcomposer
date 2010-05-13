#ifndef ILAYOUTPLUGIN_H
#define ILAYOUTPLUGIN_H

#include <QtPlugin>
#include <QString>
#include "IPluginMessage.h"
using namespace plugin;

namespace plugin {

    class IPluginFactory {
    public:
        IPluginFactory();
        virtual ~IPluginFactory() = 0;
        virtual IPluginMessage* createPluginInstance()  = 0;
        virtual void            releasePluginInstance
                                           (IPluginMessage *) = 0;

};

}

Q_DECLARE_INTERFACE(plugin::IPluginFactory,
                   "br.telemidia.purcrio.composer.IPluginFactory")

#endif // ILAYOUTPLUGIN_H
