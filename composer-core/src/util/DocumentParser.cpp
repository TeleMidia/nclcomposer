#include "../../include/util/DocumentParser.h"


DocumentParser::DocumentParser() {
    //nclDomDocument = NULL;
    util = EntityUtil::getInstance();
    messageControl = new MessageControl();
    connect(this,SIGNAL(addEntity(EntityType,string,
                        map<string,string>&,bool)),
            messageControl,SLOT(onAddEntity(EntityType,string,
                        map<string,string>&,bool)));
    connect(this,SIGNAL(createDocument(map<string,string>&)),
            messageControl,SLOT(onCreateDocument(map<string,string>&)));
    connect(messageControl,SIGNAL(documentCreated(NclDocument*)),
            this,SLOT(onDocumentCreated(NclDocument*)));

}

DocumentParser::~DocumentParser() {
    delete messageControl;
    messageControl = NULL;
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
        map<string,string> atts = util->toStdMap(root.attributes());
        emit createDocument(atts);
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

void DocumentParser::onDocumentCreated(NclDocument *nclDoc) {
    setNclDocument(nclDoc);
}


