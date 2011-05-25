#ifndef MESSAGECONTROL_H
#define MESSAGECONTROL_H

#include <QObject>
#include <QDebug>

#include <exception>
using namespace std;

#include "../extensions/IDocumentParser.h"
using namespace composer::core::extension;

#include "../model/Entity.h"
#include "../model/Document.h"
using namespace composer::core::model;

#include "../extensions/IPlugin.h"
using namespace composer::core::extension::plugin;

namespace composer {
namespace core {
namespace module {
    class MessageControl : public QObject {
        Q_OBJECT
        private:
            Document *doc;

        public:
            MessageControl(Document *doc);
            ~MessageControl();

        public slots:
            void onAddEntity( QString type,
                           QString parentEntityId, QMap<QString,QString>& atts,
                           bool force);
            void onEditEntity(Entity *entity,
                              QMap<QString,QString>& atts, bool force);
            void onRemoveEntity(Entity *, bool force);

        signals:
            void entityAdded(QString ID, Entity *);
            void entityAddError(QString error);
            /** TODO: Lembrar se ele tiver mudado o ID */
            void entityChanged(QString ID, Entity *);
            void entityChangeError(QString error);
            /** Lembrar de ele apagar a sua referência interna */
            void aboutToRemoveEntity(Entity *);
            void entityRemoved(QString ID, QString entityId);
            void entityRemoveError(QString error);



    };
}
}
}
#endif // MESSAGECONTROL_H
