#ifndef TRANSACTIONCONTROL_H
#define TRANSACTIONCONTROL_H

#include <QObject>
#include <QMutex>
#include <QDebug>

#include <model/ncm/Entity.h>
#include <model/ncm/NclDocument.h>
using namespace composer::model::ncm;

#include <model/functional/Layout.h>
#include <model/functional/Document.h>
using namespace composer::model::ncm::functional;

#include <model/util/EntityUtil.h>
using namespace composer::model::util;

#include "../plugin/IPlugin.h"
using namespace composer::core::plugin;

namespace composer {
namespace core {
namespace module {
    class TransactionControl : public QObject {
        Q_OBJECT
        private:
            NclDocument *nclDoc;
            QMutex lockAtt;

        public:
            TransactionControl(NclDocument *nclDoc);
            ~TransactionControl();
        private:
            void addNcl(QString ID, map<string,string>&, bool force);

            void addRegion(QString ID, string parentRegionId,
                           map<string,string>&, bool force);

            void editRegion(QString ID, composer::model::ncm::Region *,
                            map<string,string>& atts, bool force);

            void removeRegion(QString ID, composer::model::ncm::Region *,
                              bool force);

            void addRegionBase(QString ID, map<string,string>& atts,
                           bool force);

            void editRegionBase(QString ID, composer::model::ncm::Region *,
                                map<string,string> &, bool force);

            void removeRegionBase(QString ID, composer::model::ncm::Region *,
                                  bool force);

            void addEntity(QString ID, EntityType entity, string parentEntityId,
                           map<string,string>& atts, bool force);

            void editEntity(QString ID, EntityType, Entity *, map<string,string>& atts,
                            bool force);

            void removeEntity(QString ID, EntityType type,Entity *entity, bool force);

        public slots:

            void onAddEntity( EntityType type,
                           string parentEntityId, map<string,string>& atts,
                           bool force);
            void onEditEntity(Entity *entity,
                              map<string,string>& atts, bool force);
            void onRemoveEntity(Entity *, bool force);


        signals:

            void nclAdded(QString ID, Entity *);

            void regionAdded(QString ID, Entity *);
            void regionAddError(string error);
            /** TODO Lembrar se ele tiver mudado o ID */
            void regionChanged(QString ID, Entity *);
            void regionChangeError(string error);
            /** Lembrar de ele apagar a sua referência interna */
            void aboutToRemoveRegion(Entity *);
            void regionRemoved(QString ID, string regionId);
            void regionRemoveError(string error);
            void regionBaseAdded(QString ID, Entity *);
            void regionBaseAddError(string error);
            /** TODO Lembrar se ele tiver mudado o ID */
            void regionBaseChanged(QString ID, Entity *);
            void regionBaseChangeError(string error);
            /** Lembrar de ele apagar a sua referência interna */
            void regionBaseRemoved(QString ID, string regionBaseId);
            void regionBaseRemoveError(string error);
            void aboutToRemoveRegionBase(Entity *);

    };
}
}
}
#endif // TRANSACTIONCONTROL_H
