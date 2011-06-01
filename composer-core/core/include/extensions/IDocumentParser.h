#ifndef IDOCUMENTPARSER_H
#define IDOCUMENTPARSER_H

#include <QObject>

#include "../model/Document.h"
using namespace composer::core::model;

namespace composer {
    namespace extension {

        class IDocumentParser : public QObject
        {
            Q_OBJECT

        public:
            virtual ~IDocumentParser() {}
            virtual bool parseDocument() = 0;
            virtual QString getParserName() = 0;

        signals:

            void addEntity( QString type, QString parentEntityId,
                            QMap<QString,QString>& atts, bool force);

        public slots:
            virtual void onEntityAddError(QString error) = 0;
            virtual void onEntityAdded(QString ID, Entity *entity) = 0;
        };
    } } //end namespace

#endif // IDOCUMENTPARSER_H
