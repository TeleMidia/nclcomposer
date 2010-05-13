#ifndef ILAYOUTMESSAGE_H
#define ILAYOUTMESSAGE_H

#include <QObject>

#include <string>
#include <map>
using namespace std;

#include <model/ncm/Entity.h>
#include <model/ncm/NclDocument.h>
using namespace ncm;

namespace plugin {
    #define TOTALENTITIES 2
    typedef enum {
          REGION,
          REGIONBASE,
    }EntityType;

    class IPluginMessage : public QObject {
        Q_OBJECT

        private:
             NclDocument *nclDoc;
        public:
             IPluginMessage();
             void setNclDocument(NclDocument *nclDoc) {this->nclDoc = nclDoc; };
             NclDocument* getNclDocument() { return this->nclDoc; };
             virtual bool listenFilter(EntityType ) = 0;
        public slots:
             virtual void onEntityAdded(Entity *) = 0;
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

#endif // ILAYOUTMESSAGE_H
