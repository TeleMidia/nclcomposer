#include "../../include/util/DocumentParser.h"

namespace composer {
namespace core {
namespace plugin {


DocumentParser::DocumentParser() {
    //nclDomDocument = NULL;
    util = EntityUtil::getInstance();
    //ModuleControl *moduleControl = (ModuleControl*) ModuleControl::getModule("module");

    /*connect(this,SIGNAL(addEntity(EntityType,string,
                        map<string,string>&,bool)),
            moduleControl->getModule("message"),
            SLOT(onAddEntity(EntityType,string,
                        map<string,string>&,bool))); */

}

DocumentParser::~DocumentParser() {
    EntityUtil::releaseInstance();
//    delete nclDoc;
//    nclDoc = NULL;
}

bool DocumentParser::setUpParser(QString uri) {

    QFile *file = new QFile(uri,this);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << tr("DOM Parser") << tr("Could not open file %1\n")
                                        .arg(uri);
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!nclDomDocument.setContent
        (file, true, &errorStr, &errorLine, &errorColumn)) {

        qDebug() << tr("DOM Parser") <<
                             tr("Parser error at line %1 , column %2 : %3\n")
                             .arg(errorLine).arg(errorColumn).arg(errorStr);
        file->close();
        return false;
    } else {
        map<string,string> atts;
        atts["xmlns"] = "EDTV Basic Profile";
        emit addEntity(NCL,"",atts,false);
    }
    file->close();
    return true;
}

bool DocumentParser::parseDocument() {
    QDomElement root = nclDomDocument.documentElement();

    if (root.tagName() != "ncl") {
        qDebug() << tr("Not a NCL file.\n");
        return false;
    } else {
        QDomNodeList children = root.childNodes();
        for (int i = 0 ; i < children.count() ; i++) {
            if (children.at(i).isElement())
                parseElement(children.at(i).toElement());
        }
    }
    return true;
}

bool DocumentParser::parseElement(QDomElement element) {

    string parentId;
    map<string,string> atts;
    QDomNamedNodeMap attsElement;
    QDomElement parentElement;

    if (element.parentNode().isElement()) {
        parentElement = element.parentNode().toElement();
    } else {
        qDebug() << tr("parseElement but the parent is not a element (%1)")
                                                    .arg(element.nodeType());
        return false;
    }

    EntityType type = util->getEntityType(element.tagName());

    //qDebug() << "element (" << element.tagName() << ")";
    /*
    //TODO - lembrar de voltar quando tiver completo
    if (type == NONE) {
        qDebug() << tr("parseElement but is not a valid NCL element (%1)")
                                                    .arg(element.tagName());
        return false;
    }*/

    parentId = parentElement.attribute("id","").toStdString();
    attsElement = element.attributes();
    atts = util->toStdMap(attsElement);
    emit addEntity(type,parentId,atts,true);

    QDomNodeList children = element.childNodes();
    for (int i = 0 ; i < children.count() ; i++) {
        if (children.at(i).isElement())
            parseElement(children.at(i).toElement());
    }
    return true;
}

void DocumentParser::onEntityAdded(Entity *entity){
    setNclDocument((NclDocument*)entity);
    parseDocument();
}

bool DocumentParser::listenFilter(EntityType entityType){
    if (entityType == NCL)
        return true;
}

void DocumentParser::onEntityAddError(string error){

}

/** TODO Lembrar se ele tiver mudado o ID */
void DocumentParser::onEntityChanged(Entity *){

}

void DocumentParser::onEntityChangeError(string error){

}

/** Lembrar de ele apagar a sua referência interna */
void DocumentParser::onEntityAboutToRemove(Entity *){

}

void DocumentParser::onEntityRemoved(string entityID){

}

void DocumentParser::onEntityRemoveError(string error){

}

}
}
}


