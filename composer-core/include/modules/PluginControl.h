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

#include "../model/Document.h"
using namespace composer::core::model;

#include "../util/DocumentParserFactory.h"
#include "../util/Singleton.h"
using namespace composer::core::util;

#include "TransactionControl.h"
#include "LanguageControl.h"
using namespace composer::core::module;

#include "../extensions/IDocumentParser.h"
#include "../extensions/ILanguageProfile.h"
using namespace composer::core::extension;

#include "../extensions/IPluginFactory.h"
#include "../extensions/IPlugin.h"
using namespace composer::core::extension::plugin;


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
            /* PluginFactory by pluginID */
            QHash<QString,IPluginFactory*> pluginFactories;
            /* pluginID by LanguageType */
            QMultiHash<LanguageType, QString> pluginsByType;
            /* TC by DocumentID */
            QHash<QString,TransactionControl*> transactionControls;
            /* Plugin Instance by DocumentID */
            QMultiHash<QString,IPlugin*> pluginInstances;

            void launchNewPlugin(IPlugin *plugin,
                                 TransactionControl *tControl);
            void connectParser(IDocumentParser *parser,
                                 TransactionControl *tControl);

        public:
            void loadPlugins(QString pluginsDirPath);

        public slots:
            void onNewDocument(QString documentId, QString location);

        signals:
            void newDocumentLaunchedAndCreated(QString documentdId,
                                               QString location);
            void notifyError(QString);

    };
}
}
}
#endif // PLUGINCONTROL_H
