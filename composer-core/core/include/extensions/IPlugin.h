#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
#include <QMutexLocker>
#include <QMutex>

#include "../model/Entity.h"
#include "../model/Document.h"
#include "../modules/LanguageControl.h"
using namespace composer::core::model;

namespace composer {
    namespace extension {

    //!  The interface for communication between the core and the plugin
    /*!
      This extension point is used by the core to emit and receive signals.
      These signals are transmited by the plugin to the core, and then the core
      retransmit this signal to the other plugins associated with the
      NclDocument.
      All the communication is performed using the signal/slot mechanism
      provided by QT. The plugin developer needs to implement all the slots in
      order to capture the changes in the NclDocument and emit the signals when
      itself makes a change.

      In short, the SLOT are messages from the CORE to PLUGIN, while the
      signals bellow are messages from PLUGIN to CORE.
    */
    class IPlugin : public QObject {
        Q_OBJECT

        protected:
             //! The Document binded with this particular plugin instance
             Document *doc;
             QString pluginInstanceID;
             QMutex mutex;
             ILanguageProfile *languageProfile;

        public:
             inline QString getPluginInstanceID() {
                 return this->pluginInstanceID;
             }

             inline void setPluginInstanceID(QString pluginInstID) {
                 this->pluginInstanceID = pluginInstID;
             }

             inline void setLanguageProfile(ILanguageProfile *languageProfile) {
                 this->languageProfile = languageProfile;
             }

             inline ILanguageProfile *getLanguageProfile() {
                 return this->languageProfile;
             }

             //! This call is used by the core to bind an existing NclDocument
             //! with this plugin instance.
             /*!
                   \param document an NclDocument instance
             */
             inline void setDocument(Document *document) {
                 QMutexLocker locker(&mutex);
                 this->doc = document;
             }

             //! This call is used by the core to recover the NclDocument
             //! instance previously binded with this plugin instance.
             /*!
                   \return nclDoc an NclDocument instance
             */
             inline Document* getDocument() {
                 QMutexLocker locker(&mutex);
                 return this->doc;
             }

             //! Return the main Widget
             /*!
               The core calls this method to retrieve the Plugin Graphical
               Interface. This Widget is going to be wrapped into a QDockWidget
               and then displayed for the user in the main interface

                \return QWidget - wrapping the plugin interface
             */
             virtual QWidget* getWidget() = 0;

             //! Saves specific settings particular to each plugin
             /*!
               The core calls this method to notify the plugin that the user
               is requesting to save this document.
               Some plugins need to save special settings for a particular
               document, in case that document is re-open in a different
               session.
                 \return bool - true if the save was successfull,
                                false otherwise
             */
             virtual bool saveSubsession() = 0;

        public slots:
             //! Calls the plugin to update the internal model from model
             /*!
               This call is invoked by the core in two situations:
                - When the user resquest an update to all plugins
                - When the plugin is loaded in runtime
                In the second case, the plugin is loaded, but did no receive
                the previous modifications, so it has to be forceed to reload
                the Document*.
              */
             virtual void updateFromModel() = 0;

             //! This is called by the core when a new Entity is added
             /*!
                This call is invoked by the core when a new Entity (from that
                particular types the plugin is listening) is added in the model.
             */
             virtual void onEntityAdded(QString pluginID, Entity *) = 0;

             //! This is called by the core when some error occurs
             /*!
               TODO:
             */
             virtual void errorMessage(QString error) = 0;

             //! Called by the core when an Entity is changed
             /*!
                TODO:
             */
             virtual void onEntityChanged(QString pluginID, Entity *) = 0;

             //! Called by the core before the Entity is removed.
             /*!
               TODO:
             */
             virtual void onEntityAboutToRemove(Entity *) = 0;

             //! Called by the core after the entity entityID is removed.
             /*!
               This messa
             */
             virtual void onEntityRemoved( QString pluginID,
                                           QString entityID) = 0;

        signals:
             //! This message can be used to add a new entity in the model
             /*!
               TODO:
             */
             void addEntity( QString type, QString parentEntityId,
                                    QMap<QString,QString>& atts, bool force);

             //! This message can be used to edit the attributes of an Entity
             /*!
               TODO:
             */
             void setAttributes( Entity *, QMap<QString,QString>& atts,
                                 bool force);

             /*!
              \brief This message can be used to ask the core to remove an
                    Entity.

              \param entity The pointer to the Entity must be removed.
              \param force It should be true if the user want to remove that
                        entity even the internal model will not be consistent.
             */
             void removeEntity(Entity * entity, bool force);

             //! Can be used to extend the current messages of Core
             /*!
               This message is send to all plugins without any validation.
               The content of the message can be any QObject and are useful
               to extend the curretn
             */
             void sendBroadcastMessage(const char* msg, void *obj);

    };

} } //end namespace

#endif // IPLUGIN_H
