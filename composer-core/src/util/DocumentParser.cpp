#include "../../include/util/DocumentParser.h"


DocumentParser::DocumentParser() {
    //nclDomDocument = NULL;
    util = EntityUtil::getInstance();
    messageControl = new MessageControl();
    connect(this,SIGNAL(addEntity(EntityType,string,
                        map<string,string>&,bool)),
            messageControl,SLOT(onAddEntity(EntityType,string,
                        map<string,string>&,bool)));

}

DocumentParser::~DocumentParser() {
    delete messageControl;
    messageControl = NULL;
    EntityUtil::releaseInstance();

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

        return false;
    }
    return true;
}

bool DocumentParser::parseDocument() {
    QDomElement root = nclDomDocument.documentElement();

    if (root.tagName() != "ncl") {
        qDebug() << tr("Not a NCL file.\n");
        return false;
    }
    return true;
}

bool DocumentParser::parseElement(QDomElement element,
                                  QDomElement parentElement) {

    QString parentId;
    map<string,string> atts;
    EntityType type = util->getEntityType(element.tagName());
    if (type == NONE)
        return false;
    parentId = parentElement.attribute("id","");





}

