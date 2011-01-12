#ifndef MESSAGECONTROL_H
#define MESSAGECONTROL_H

#include <QObject>

#include <model/ncm/Region.h>
using namespace ncm;

#include <QReadWriteLock>
#include <QMutex>
#include <QDebug>

#include <map>
#include <string>
using namespace std;

#include <model/ncm/NclDocument.h>
using namespace ncm;

#include <model/functional/Layout.h>
#include <model/functional/Document.h>
using namespace ncm::functional;

#include "../plugin/IPlugin.h"
using namespace composer::core::plugin;

#include "../util/Singleton.h"
using namespace composer::core::util;

namespace composer {
namespace core {
namespace module {

    class MessageControl : public QObject, public Singleton<MessageControl> {
     Q_OBJECT

      friend class Singleton<MessageControl>;
      private:
            MessageControl();
            ~MessageControl();

            Layout  *layoutFacade;
            Document *documentFacade;
            IPlugin *sender;
            QReadWriteLock lockSender;
            QMutex mutex;

            //TODO - rever essa logica
            inline void setSender(IPlugin *sender) {
                QWriteLocker locker(&lockSender);
                this->sender = sender;
            }
            inline IPlugin* getSender() {
                QReadLocker locker(&lockSender);
                return this->sender;
            }

        private:
            void addNcl(map<string,string>&, bool force);

            void addRegion(string parentRegionId,
                           map<string,string>&, bool force);

            void editRegion( ncm::Region *, map<string,string>& atts,
                           bool force);
            void removeRegion( ncm::Region *, bool force);

            void addRegionBase(map<string,string>& atts,
                           bool force);

            void editRegionBase( ncm::Region *, map<string,string> &,
                           bool force);

            void removeRegionBase( ncm::Region *, bool force);

            void addEntity(EntityType entity, string parentEntityId,
                           map<string,string>& atts, bool force);

            void editEntity(EntityType, Entity *, map<string,string>& atts,
                            bool force);

            void removeEntity(EntityType type,Entity *entity, bool force);

        public slots:

            void onAddEntity( EntityType entity,
                           string parentEntityId, map<string,string>& atts,
                           bool force);
            void onEditEntity(EntityType, Entity *, map<string,string>& atts,
                           bool force);
            void onRemoveEntity(EntityType type, Entity *, bool force);


        signals:
            void nclAdded(Entity *);

            void regionAdded(ncm::Region *);
            void regionAddError(string error);
            /** TODO Lembrar se ele tiver mudado o ID */
            void regionChanged(ncm::Region *);
            void regionChangeError(string error);
            /** Lembrar de ele apagar a sua referência interna */
            void aboutToRemoveRegion(ncm::Region *);
            void regionRemoved(string regionId);
            void regionRemoveError(string error);
            void regionBaseAdded(ncm::Region *);
            void regionBaseAddError(string error);
            /** TODO Lembrar se ele tiver mudado o ID */
            void regionBaseChanged(ncm::Region *);
            void regionBaseChangeError(string error);
            /** Lembrar de ele apagar a sua referência interna */
            void regionBaseRemoved(string regionBaseId);
            void regionBaseRemoveError(string error);
            void aboutToRemoveRegionBase(ncm::Region *);
    };
}
}
}
#endif // MESSAGECONTROL_H
