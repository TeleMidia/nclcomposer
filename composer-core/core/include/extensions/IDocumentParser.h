#ifndef IDOCUMENTPARSER_H
#define IDOCUMENTPARSER_H

#include <QObject>

#include "../model/Document.h"
using namespace composer::core::model;

namespace composer {
    namespace extension {

        /*!
         \brief An abstract interface to document parsers.
        */
        class IDocumentParser : public QObject
        {
            Q_OBJECT

        public:
            virtual ~IDocumentParser() {}
            /*!
             \brief Parses the content of the document. This function must be
              implemented by concrete Document Parsers.

             \return bool returns true if success and false otherwise.
            */
            virtual bool parseDocument() = 0;
            /*!
             \brief Returns the name of the Document Parser. This name should
                be unique. This function must be implemented by concrete
                Document Parsers.

             \return QString the name of the Document Parser.
            */
            virtual QString getParserName() = 0;

        signals:

            /*!
             \brief Signal emitted when the parser find a new entity. This
                function must be implemented by concrete Document Parsers.

             \param type the type of the entity (XML tagname).
             \param parentEntityId uniqueId of the father.
             \param atts attributes of this entity.
             \param force true if the entity must be added even if this cause
                an error in the model.
            */
            void addEntity( QString type, QString parentEntityId,
                            QMap<QString,QString>& atts, bool force);

        public slots:
            /*!
             \brief This slot is called when there is a error in the model
                (commonly after a addEntity call). This function must be
                implemented by concrete Document Parsers.

             \param error The description of the error.
            */
            virtual void onEntityAddError(QString error) = 0;
            /*!
             \brief This slot is called after the addEntity has success. This
                function must be implemented by concrete Document Parsers.

             \param uniqueID the unique id of the entity in the model.
             \param entity the pointer to the entity.
            */
            virtual void onEntityAdded(QString uniqueID, Entity *entity) = 0;
        };
    } } //end namespace

#endif // IDOCUMENTPARSER_H
