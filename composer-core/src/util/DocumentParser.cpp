#include "include/util/DocumentParser.h"

namespace composer {
namespace core {
namespace util {


DocumentParser::DocumentParser() {
    qDebug() << "DocumentParser::DocumentParser()";
}

DocumentParser::~DocumentParser() {

}

bool DocumentParser::setUpParser(QString uri) {

    qDebug() << "DocumentParser::setUpParser(" << uri << ")";
    file = new QFile(uri,this);
    if (!file->open(QIODevice::ReadWrite)) {
        qDebug() << "DocumentParser::setUpParser"
                 << tr("Could not open file %1\n").arg(uri);
        return false;
    }
    file->close();
    return true;
}

bool DocumentParser::parseDocument() {

    QXmlInputSource inputSource(file);
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);
    return reader.parse(inputSource);

}

bool DocumentParser::startElement(const QString &namespaceURI,
                  const QString &localName,
                  const QString &qName,
                  const QXmlAttributes &attributes)
{
    qDebug() << "DocumentParser::startElement(" << qName << ")";
    if (qName == "ncl") return true;

    QMap<QString,QString> atts;

    Entity *parentEntity = elementStack.top();

    QString parentId = parentEntity->getUniqueId();

    for (int i=0 ;i < attributes.count(); i++){
        atts[attributes.qName(i)] = attributes.value(i);

        emit addEntity(qName,parentId,atts,false);
    }
    return true;
}

bool DocumentParser::endElement(const QString &namespaceURI,
                const QString &localName,
                const QString &qName)
{
    elementStack.pop();
}

bool DocumentParser::characters(const QString &str) {

}

bool DocumentParser::fatalError(const QXmlParseException &exception) {
    qWarning() << "Fatal error on line" << exception.lineNumber()
                    << ", column" << exception.columnNumber() << ":"
                    << exception.message();

    return false;
}

void DocumentParser::onEntityAdded(QString ID, Entity *entity){
    qDebug() << "DocumentParser::onEntityAdded(" << ID
            << ", " << entity->getType() << ")";

    if (entity->getType() == "document") {
          setUpParser(((Document*)entity)->getLocation());
          parseDocument();
    }
    elementStack.push(entity);
}


void DocumentParser::onEntityAddError(QString error){

}

/** TODO Lembrar se ele tiver mudado o ID */
void DocumentParser::onEntityChanged(QString ID, Entity *){

}

void DocumentParser::onEntityChangeError(QString error){

}

/** Lembrar de ele apagar a sua referência interna */
void DocumentParser::onEntityAboutToRemove(Entity *){

}

void DocumentParser::onEntityRemoved(QString ID, QString entityID){

}

void DocumentParser::onEntityRemoveError(QString error){

}

}
}
}


