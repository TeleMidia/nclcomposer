#include "NCLDocumentParser.h"

NCLDocumentParser::NCLDocumentParser(Document *doc)
{
    qDebug() << "NCLDocumentParser::NCLDocumentParser("
             << doc->getLocation() << ")";
    this->doc = doc;
}

NCLDocumentParser::~NCLDocumentParser()
{

}

QString NCLDocumentParser::getParserName()

{
    return "NCLDocumentParser";
}

bool NCLDocumentParser::parseDocument()
{
    qDebug() << "NCLDocumentParser::parseDocument("
             << doc->getLocation() << ")";

    QString uri = doc->getLocation();

    QFile *file = new QFile(uri,this);
    if (!file->open(QIODevice::ReadWrite)) {
        qDebug() << "DocumentParser::parseDocument"
                 << tr("Could not open file %1\n").arg(uri);
        return false;
    }

    file->close();

    QXmlInputSource inputSource(file);
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);
    return reader.parse(inputSource);

}


bool NCLDocumentParser::startElement(const QString &namespaceURI,
                  const QString &localName,
                  const QString &qName,
                  const QXmlAttributes &attributes)
{

    QMap<QString,QString> atts;
    QString parentId = doc->getUniqueId();

    if (qName != "ncl")
    {
        lockStack.lock();

        Entity *parentEntity = elementStack.top();

        lockStack.unlock();

        parentId = parentEntity->getUniqueId();
    }

    for (int i=0 ;i < attributes.count(); i++)
        atts[attributes.qName(i)] = attributes.value(i);

    emit addEntity(qName,parentId,atts,false);

    return true;
}

bool NCLDocumentParser::endElement(const QString &namespaceURI,
                const QString &localName,
                const QString &qName)
{

    lockStack.lock();
    elementStack.pop();
    //sync.wakeAll();
    lockStack.unlock();
}

bool NCLDocumentParser::characters(const QString &str)
{

}

bool NCLDocumentParser::fatalError(const QXmlParseException &exception)
{
    qWarning() << "Fatal error on line" << exception.lineNumber()
                    << ", column" << exception.columnNumber() << ":"
                    << exception.message();

    return false;
}

void NCLDocumentParser::onEntityAdded(QString ID, Entity *entity)
{
    qDebug() << "DocumentParser::onEntityAdded(" << ID
            << ", " << entity->getType() << ")";

    lockStack.lock();
    elementStack.push(entity);
    //sync.wakeAll();
    lockStack.unlock();
}


void NCLDocumentParser::onEntityAddError(QString error)
{
    qWarning() << "NCLDocumentParser::onEntityAddError(" << error << ")";
    //sync.wakeAll();
}


