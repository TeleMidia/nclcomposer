#ifndef MESSAGECONTROL_H
#define MESSAGECONTROL_H

#include <QObject>
#include <QDebug>

#include <exception>
using namespace std;

#include "../extensions/IDocumentParser.h"
using namespace composer::extension;

#include "../model/Entity.h"
#include "../model/Document.h"
using namespace composer::core::model;

#include "../extensions/IPlugin.h"
using namespace composer::extension;

namespace composer {
    namespace core {

        class MessageControl : public QObject {
            Q_OBJECT

        private:
            Document *doc;

        public:
            MessageControl(Document *doc);
            ~MessageControl();

        public slots:
            void onAddEntity( QString type,
                              QString parentEntityId,
                              QMap<QString,QString>& atts,
                              bool force);
            void onEditEntity(Entity *entity,
                              QMap<QString,QString> atts, bool force);
            void onRemoveEntity(Entity *, bool force);

        signals:
            void entityAdded(QString uniqueID, Entity *);
            void entityAddError(QString error);
            void entityChanged(QString uniqueID, Entity *);
            void entityChangeError(QString error);
            void entityRemoved(QString uniqueID, QString entityId);
            void entityRemoveError(QString error);
        };
    } }
#endif // MESSAGECONTROL_H
