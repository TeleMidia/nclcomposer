#include "../../include/util/DocumentParser.h"

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

    EntityType type = EntityUtil::getEntityType(qName.toStdString());
    map<string,string> atts;

    if (type == NONE) {
        qDebug() << "Element (" << qName << ") is not part of the model";
        return true;
    }
    Entity *parentEntity = elementStack.top();

    string parentId = parentEntity->getUniqueId();

    for (int i=0 ;i < attributes.count(); i++){
        atts[attributes.qName(i).toStdString()] =
                                           attributes.value(i).toStdString();

        emit addEntity(type,parentId,atts,false);
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
            << ", " << QString::fromStdString(
                    EntityUtil::getEntityName(entity->getType())) << ")";

    if (entity->getType() == NCL) {
          setUpParser(QString::fromStdString(
                     ((NclDocument*)entity)->getLocation()));
          parseDocument();
    }
    elementStack.push(entity);
}

bool DocumentParser::listenFilter(EntityType entityType){
    return true;
}

void DocumentParser::onEntityAddError(string error){

}

/** TODO Lembrar se ele tiver mudado o ID */
void DocumentParser::onEntityChanged(QString ID, Entity *){

}

void DocumentParser::onEntityChangeError(string error){

}

/** Lembrar de ele apagar a sua referência interna */
void DocumentParser::onEntityAboutToRemove(Entity *){

}

void DocumentParser::onEntityRemoved(QString ID, string entityID){

}

void DocumentParser::onEntityRemoveError(string error){

}

}
}
}


