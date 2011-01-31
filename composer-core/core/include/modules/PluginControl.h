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

#include "../model/Document.h"
using namespace composer::core::model;

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
            /* Plugin Instance by DocumentLocation */
            QMultiHash<QString,IPlugin*> pluginInstances;

            void launchNewPlugin(IPlugin *plugin,
                                 TransactionControl *tControl);
            void connectParser(IDocumentParser *parser,
                                 TransactionControl *tControl);

        public:
            void loadPlugins(QString pluginsDirPath);
            IPluginFactory* loadPlugin(QString fileName);
            QList<IPluginFactory*> getLoadedPlugins();
            bool releasePlugins(Document *doc);

        public slots:
            void launchDocument(Document *doc);

        signals:
            void newDocumentLaunchedAndCreated(QString documentdId,
                                               QString location);
            void notifyError(QString);
            void addPluginWidgetToWindow(IPluginFactory*,IPlugin*,
                                    QString projectId, QString documentId);
            void addPluginWidgetToWindow(IPluginFactory*,IPlugin*,
                                         Document*);

    };
        }
    }
}
#endif // PLUGINCONTROL_H
