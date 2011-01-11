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

#include "MessageControl.h"
#include "../plugin/IPluginFactory.h"
#include "../plugin/IPlugin.h"
using namespace composer::core::plugin;

namespace composer {
namespace core {
namespace module {
    class PluginControl : public QObject{
        Q_OBJECT
        private:
            QHash<QString,IPluginFactory*> pluginFactories;
            QMultiHash<QString,IPlugin*> pluginInstances;
            MessageControl *messageControl;
            void connectRegion(IPlugin *);
            void connectRegionBase(IPlugin *);
        public:
            PluginControl();
            ~PluginControl();
            void loadPlugins(QString pluginsDirPath);
            void launchNewPlugin(IPlugin *);
        public slots:
            void onNewDocument(NclDocument *nclDoc);

    };
}
}
}
#endif // PLUGINCONTROL_H
