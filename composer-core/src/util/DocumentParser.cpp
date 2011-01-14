#include "../../include/util/DocumentParser.h"

namespace composer {
namespace core {
namespace util {


DocumentParser::DocumentParser() {
    util = EntityUtil::getInstance();
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
    EntityType type = util->getEntityType(qName.toStdString());
    map<string,string> atts;
    //TODO - voltar aqui quando o modelo estiver fechado
//    if (type == NONE) {
//        qDebug() << "Element (" << qName << ") is not a valid NCL element";
//    }
    Entity *parentEntity = elementStack.top();
    string parentId = parentEntity->getUniqueId();

    qDebug() << "DocumentParser::startElement(" << qName << ")";

    for (int i=0 ;i < attributes.count(); i++){
        atts[attributes.qName(i).toStdString()] =
                                           attributes.value(i).toStdString();

        emit addEntity(type,parentId,atts,false);
    }

}

bool DocumentParser::endElement(const QString &namespaceURI,
                const QString &localName,
                const QString &qName)
{


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
    if (entity->getType() == NCL) {
//        setNclDocument((NclDocument*)entity);
          setUpParser(QString::fromStdString(
                     ((NclDocument*)entity)->getLocation()));
//        parseDocument();

    }
}

bool DocumentParser::listenFilter(EntityType entityType){
    if (entityType == NCL)
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


