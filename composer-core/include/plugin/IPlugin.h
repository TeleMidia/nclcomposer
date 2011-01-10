#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
#include <QMutexLocker>
#include <QMutex>

#include <string>
#include <map>
using namespace std;

#include <model/ncm/Entity.h>
#include <model/ncm/NclDocument.h>
using namespace ncm;

#include "../util/EntityUtil.h"
using namespace composer::core::util;

//!  The interface for communication between the core and the plugin
/*!
  This extension point is used by the core to emit and receive signals.
  These signals are transmited by the plugin to the core, and then the core
  retransmit this signal to the other plugins associated with the NclDocument.
  All the communication is performed using the signal/slot mechanism provided
  by QT. The plugin developer needs to implement all the slots in order to
  capture the changes in the NclDocument and emit the signals when itself makes
  a change.
*/

namespace composer {
namespace core {
namespace plugin {


    class IPlugin : public QObject {
        Q_OBJECT

        private:
             //! The NclDocument binded with this particular plugin instance
             NclDocument *nclDoc;
             QMutex mutex;
        public:
             //IPluginMessage();
             //! This call is used by the core to bind an existing NclDocument
             //! with this plugin instance.
             /*!
                   \param document a NclDocument instance
             */
             inline void setNclDocument(NclDocument *document) {
                 QMutexLocker locker(&mutex);
                 this->nclDoc = document;
             }
             //! This call is used by the core to recover the NclDocument
             //! instance previously binded with this plugin instance.
             /*!
                   \return nclDoc a NclDocument instance
             */
             inline NclDocument* getNclDocument() {
                 QMutexLocker locker(&mutex);
                 return this->nclDoc;
             }
             //! This is the filter method.
             /*!
                This call gives the possibilite to the core to filter
                wich Entities changes are goign to be notified for this plugin.
                Through this mechanism plugins can listen to changes in a specific
                set of Entities, and all the filter control is made by the core.
                \param the EntityType that is going to be filtered
             */
             virtual bool listenFilter(EntityType ) = 0;

             //! Return the main Widget
             /*!
               The core calls this method to retrieve the Plugin Interface
               This Widget is going to be wrapped into a QDockWidget
               and then displayed for the user in the main interface
                \return QWidget - wrapping the plugin interface
             */
             virtual QWidget* getWidget() = 0;
        public slots:
             //! This is called by the core when a new Entity is added
             /*!
                This call is invoked by the core when a new Entity that this
                particular plugin is listening is added.
             */
             virtual void onEntityAdded(Entity *) = 0;
             //! This is called by the core when the adding a new Entity was ignored
             /*!
                This call is invoked by the core to notify the plugin that the
                new Entity it was trying to add to e the NclDocument was ignored
                because of an error.
             */
             virtual void onEntityAddError(string error) = 0;
             /** TODO Lembrar se ele tiver mudado o ID */
             virtual void onEntityChanged(Entity *) = 0;
             virtual void onEntityChangeError(string error) = 0;
             /** Lembrar de ele apagar a sua referência interna */
             virtual void onEntityAboutToRemove(Entity *) = 0;
             virtual void onEntityRemoved(string entityID) = 0;
             virtual void onEntityRemoveError(string error) = 0;


        signals:
             void addEntity( EntityType entity, string parentEntityId,
                                    map<string,string>& atts, bool force);
             void editEntity(EntityType,Entity *, map<string,string>& atts,
                                                                    bool force);
             void removeEntity( Entity *, bool force);

    };
}
}
}

#endif // IPLUGIN_H
