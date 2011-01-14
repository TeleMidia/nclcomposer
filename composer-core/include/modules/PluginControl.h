#ifndef PLUGINCONTROL_H
#define PLUGINCONTROL_H

#include <QString>
#include <QObject>
#include <QDir>
#include <QPluginLoader>
#include <QObject>
#include <QMultiHash>
#include <QList>
#include <QHashIterator>
#include <QDebug>

#include <map>
#include <string>
using namespace std;

#include <model/ncm/NclDocument.h>
using namespace composer::model::ncm;

#include <model/functional/Document.h>
using namespace composer::model::ncm::functional;

#include <model/util/Singleton.h>
using namespace composer::model::util;

#include "TransactionControl.h"
using namespace composer::core::module;

#include "../plugin/IPluginFactory.h"
#include "../plugin/IPlugin.h"
using namespace composer::core::plugin;

#include "../util/DocumentParserFactory.h"
using namespace composer::core::util;


//TODO - fazer um gerenciar de maneira eficiente
//       quando o usuario fechar o plugin tirar ele da memoria

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
            QHash<QString,TransactionControl*> transactionControls;
            QMultiHash<QString,IPlugin*> pluginInstances;

            void connectRegion(IPlugin *,TransactionControl *tControl);
            void connectRegionBase(IPlugin *, TransactionControl *tControl);

        public:
            void loadPlugins(QString pluginsDirPath);
            void launchNewPlugin(IPlugin *plugin,
                                 TransactionControl *tControl);

        public slots:
            void onNewDocument(QString documentId, QString location);

        signals:
            void newDocumentLaunchedAndCreated(QString documentdId,
                                               QString location);
            void notifyError(QString);

            void addNcl(QString ID, Entity *);

    };
}
}
}
#endif // PLUGINCONTROL_H
