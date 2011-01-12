#ifndef PLUGINCONTROL_H
#define PLUGINCONTROL_H

#include <QString>
#include <QObject>
#include <QDir>
#include <QPluginLoader>
#include <QObject>
#include <QPair>
#include <QMultiHash>
#include <QList>
#include <QHashIterator>
#include <QMetaEnum>
#include <QMetaObject>

#include <model/ncm/NclDocument.h>
using namespace ncm;

#include "../util/Singleton.h"
using namespace composer::core::util;

#include "MessageControl.h"
#include "../plugin/IPluginFactory.h"
#include "../plugin/IPlugin.h"
using namespace composer::core::plugin;

namespace composer {
namespace core {
namespace module {

    class PluginControl : public QObject , public Singleton<PluginControl> {
        Q_OBJECT

        friend class Singleton<PluginControl>;
        private:
            PluginControl();
            ~PluginControl();
            QHash<QString,IPluginFactory*> pluginFactories;
            QMultiHash<QString,IPlugin*> pluginInstances;

            void connectRegion(IPlugin *);
            void connectRegionBase(IPlugin *);

        public:
            void loadPlugins(QString pluginsDirPath);
            void launchNewPlugin(IPlugin *);
        public slots:
            void onNewDocument(NclDocument *nclDoc);

    };
}
}
}
#endif // PLUGINCONTROL_H
