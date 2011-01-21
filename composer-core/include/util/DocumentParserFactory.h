#ifndef DOCUMENTPARSERFACTORY_H
#define DOCUMENTPARSERFACTORY_H

#include "../extensions/IPluginFactory.h"
using namespace composer::core::extension::plugin;

#include "DocumentParser.h"
using namespace composer::core::util;

namespace composer {
namespace core {
namespace util {

class DocumentParserFactory : public IPluginFactory
{
    public:
        DocumentParserFactory();
        ~DocumentParserFactory();
        IPlugin* createPluginInstance();
        void     releasePluginInstance(IPlugin *instance);
        QString  getPluginID();
};


}
}
}
#endif // DOCUMENTPARSERFACTORY_H
