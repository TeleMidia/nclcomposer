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
#include "../plugin/IPluginMessage.h"
using namespace composer::core::plugin;

namespace composer {
namespace core {
namespace module {
    class PluginControl : public QObject{
        Q_OBJECT
        private:
            QHash<QString,IPluginFactory*> pluginFactories;
            QMultiHash<QString,IPluginMessage*> pluginInstances;
            MessageControl *messageControl;
            void connectRegion(IPluginMessage *);
            void connectRegionBase(IPluginMessage *);
        public:
            PluginControl(MessageControl *message);
            ~PluginControl();
            void loadPlugins(QString pluginsDirPath);
            void launchNewPlugin(IPluginMessage *);
        public slots:
            void onNewDocument(NclDocument *nclDoc);

    };
}
}
}
#endif // PLUGINCONTROL_H
