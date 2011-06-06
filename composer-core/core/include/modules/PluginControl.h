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


#include "MessageControl.h"
#include "LanguageControl.h"
using namespace composer::core;

#include "../model/Document.h"
using namespace composer::core::model;

#include "../util/Singleton.h"
using namespace composer::core::util;

#include "../extensions/IDocumentParser.h"
#include "../extensions/ILanguageProfile.h"
#include "../extensions/IPluginFactory.h"
#include "../extensions/IPlugin.h"
using namespace composer::extension;

namespace composer {
    namespace core {

        class PluginControl : public QObject
        {
            Q_OBJECT
            SINGLETON(PluginControl)

        private:
                    PluginControl();
            ~PluginControl();
            /* PluginFactory by pluginID */
            QHash<QString,IPluginFactory*> pluginFactories;
            /* pluginID by LanguageType */
            QMultiHash<LanguageType, QString> pluginsByType;

            /* TC by DocumentID */
            /* TODO: Is this right?? And when we have plugins that are not
            related to documents???
            */
            QHash<QString,MessageControl*> messageControls;

            /* Plugin Instance by DocumentLocation */
            QMultiHash<QString,IPlugin*> pluginInstances;
            /* Relate each IPlugin to its correspondent IPluginFactory */
            QMultiHash<IPlugin*,IPluginFactory*> factoryByPlugin;

            void launchNewPlugin(IPlugin *plugin,
                                 MessageControl *mControl);
            void connectParser(IDocumentParser *parser,
                               MessageControl *mControl);

        public:
            void loadPlugins(QString pluginsDirPath);
            IPluginFactory* loadPlugin(QString fileName);
            QList<IPluginFactory*> getLoadedPlugins();
            bool releasePlugins(Document *doc);

        public slots:
            void launchDocument(Document *doc);
            void saveDocument(QString location);

        private slots:
            void sendBroadcastMessage(const char *slot, void *payload);

        signals:
            void newDocumentLaunchedAndCreated(QString documentdId,
                                               QString location);
            void notifyError(QString);
            void addPluginWidgetToWindow(IPluginFactory*, IPlugin*,
                                         QString projectId, QString documentId);
            void addPluginWidgetToWindow(IPluginFactory*,IPlugin*,
                                         Document*, int n);

        };
} } //end namespace

#endif // PLUGINCONTROL_H
