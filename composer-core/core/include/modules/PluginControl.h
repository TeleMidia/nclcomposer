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

        /*!
         \brief Manages all the plugins in the system.

         It is part of the PluginControl responsability:
            - Find the installed plugins in the system.
            - Load the installed plugins.
            - And connect each plugin with its respective message controllers.
        */
        class PluginControl : public QObject
        {
            Q_OBJECT
            SINGLETON(PluginControl)

        private:
            /*!
             \brief

            */
            PluginControl();
            /*!
             \brief

            */
            ~PluginControl();
            QHash<QString,IPluginFactory*> pluginFactories;
            /*!< PluginFactory by pluginID */
            QMultiHash<LanguageType, QString> pluginsByType;
            /*!< pluginID given LanguageType */

            /* TC by DocumentID */
            /* TODO: Is this right??
                And when we have plugins that are not related to documents???
            */
            QHash <QString, MessageControl*> messageControls; /*!< TODO */

            QMultiHash<QString, IPlugin*> pluginInstances;
            /*!< Plugin Instance given DocumentLocation */

            QMultiHash<IPlugin*, IPluginFactory*> factoryByPlugin;
            /*!< Maps each IPlugin to its corresponding IPluginFactory */

            /*!
             \brief Launchs a new plugin and connect it with the given
                MessageControl.

             \param plugin the plugin instance that must be connected.
             \param mControl
            */
            void launchNewPlugin(IPlugin *plugin,
                                 MessageControl *mControl);
            /*!
             \brief

             \param parser
             \param mControl
            */
            void connectParser(IDocumentParser *parser,
                               MessageControl *mControl);

        public:
            /*!
             \brief

             \param pluginsDirPath
            */
            void loadPlugins(QString pluginsDirPath);
            /*!
             \brief

             \param fileName
             \return IPluginFactory *
            */
            IPluginFactory* loadPlugin(QString fileName);
            /*!
             \brief

             \return QList<IPluginFactory *>
            */
            QList<IPluginFactory*> getLoadedPlugins();
            /*!
             \brief

             \param doc
             \return bool
            */
            bool releasePlugins(Document *doc);

        public slots:
            /*!
             \brief

             \param doc
            */
            void launchDocument(Document *doc);
            /*!
             \brief

             \param location
            */
            void savePluginsData(QString location);

        private slots:
            /*!
             \brief

             \param slot
             \param payload
            */
            void sendBroadcastMessage(const char *slot, void *payload);

        signals:
            /*!
             \brief

             \param documentdId
             \param location
            */
            void newDocumentLaunchedAndCreated(QString documentdId,
                                               QString location);
            /*!
             \brief

             \param QString
            */
            void notifyError(QString);
            /*!
             \brief

             \param
             \param
             \param documentId
            */
            void addPluginWidgetToWindow(IPluginFactory*, IPlugin*,
                                         QString documentId);
            /*!
             \brief

             \param
             \param
             \param
             \param n
            */
            void addPluginWidgetToWindow(IPluginFactory*,IPlugin*,
                                         Document*, int n);

        };
} } //end namespace

#endif // PLUGINCONTROL_H
