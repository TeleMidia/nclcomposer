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

#include "../plugin/IPluginMessage.h"
using namespace composer::core::plugin;

namespace composer {
namespace core {
namespace module {
    class MessageControl : public QObject {
            Q_OBJECT
        private:
            Layout  *layoutFacade;
            Document *documentFacade;
            IPluginMessage *sender;
            QReadWriteLock lockSender;
            QMutex mutex;

            inline void setSender(IPluginMessage *sender) {
                QWriteLocker locker(&lockSender);
                this->sender = sender;
            }
            inline IPluginMessage* getSender() {
                QReadLocker locker(&lockSender);
                return this->sender;
            }
        public:
            MessageControl();
            ~MessageControl();
        private:
            void addRegion(string parentRegionId,
                           map<string,string>&, bool force);

            void editRegion( Region *, map<string,string>& atts,
                           bool force);
            void removeRegion( Region *, bool force);

            void addRegionBase(map<string,string>& atts,
                           bool force);

            void editRegionBase( Region *, map<string,string> &,
                           bool force);

            void removeRegionBase( Region *, bool force);

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
            void regionAdded(Region *);
            void regionAddError(string error);
            /** TODO Lembrar se ele tiver mudado o ID */
            void regionChanged(Region *);
            void regionChangeError(string error);
            /** Lembrar de ele apagar a sua referência interna */
            void aboutToRemoveRegion(Region *);
            void regionRemoved(string regionId);
            void regionRemoveError(string error);
            void regionBaseAdded(Region *);
            void regionBaseAddError(string error);
            /** TODO Lembrar se ele tiver mudado o ID */
            void regionBaseChanged(Region *);
            void regionBaseChangeError(string error);
            /** Lembrar de ele apagar a sua referência interna */
            void regionBaseRemoved(string regionBaseId);
            void regionBaseRemoveError(string error);
            void aboutToRemoveRegionBase(Region *);
    };
}
}
}
#endif // MESSAGECONTROL_H
