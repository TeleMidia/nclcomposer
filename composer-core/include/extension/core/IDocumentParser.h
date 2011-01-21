#ifndef IDOCUMENTPARSER_H
#define IDOCUMENTPARSER_H

#include <QObject>

#include "../../model/Document.h"
using namespace composer::core::model;

namespace composer{
    namespace core {
        namespace extension {

                class IDocumentParser : public QObject
                {
                    Q_OBJECT

                public:
                    virtual ~IDocumentParser() {}
                    virtual bool parseDocument() = 0;

                signals:

                    void addEntity( QString type, QString parentEntityId,
                                    QMap<QString,QString>& atts, bool force);
                    void editEntity(Entity *, QMap<QString,QString>& atts,
                                    bool force);
                    void removeEntity( Entity *, bool force);

                public slots:

                };
        }
    }
}

#endif // IDOCUMENTPARSER_H
