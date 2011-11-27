/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "NCLDocumentParser.h"

namespace composer {
namespace language {

NCLDocumentParser::NCLDocumentParser(Project *project)
{
    this->project = project;
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
    QString uri = project->getLocation();

    QFile *file = new QFile(uri, this);
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

bool NCLDocumentParser::parseContent(const QString &str)
{
    QXmlInputSource inputSource;
    inputSource.setData(str);

    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);

    return reader.parse(inputSource);
}

bool NCLDocumentParser::startElement(const QString &,
                  const QString &,
                  const QString &qName,
                  const QXmlAttributes &attributes)
{
    QMap<QString,QString> atts;
    QString parentId = project->getUniqueId();

    if (qName != "ncl")
    {
        if(elementStack.size())
        {
            lockStack.lock();
            Entity *parentEntity = elementStack.top();
            lockStack.unlock();
            parentId = parentEntity->getUniqueId();
        }
    }

    for (int i=0 ;i < attributes.count(); i++)
        atts[attributes.qName(i)] = attributes.value(i);

    emit addEntity(qName, parentId, atts, false);

    return true;
}

bool NCLDocumentParser::endElement(const QString &namespaceURI,
                const QString &localName,
                const QString &qName)
{
    lockStack.lock();
    if(elementStack.size())
        elementStack.pop();
    lockStack.unlock();

    return true;
}

bool NCLDocumentParser::characters(const QString &)
{
    return true;
}

bool NCLDocumentParser::endDocument()
{
    emit parseFinished();
    return true;
}

bool NCLDocumentParser::fatalError(const QXmlParseException &exception)
{
    qWarning() << "Fatal error on line" << exception.lineNumber()
                    << ", column" << exception.columnNumber() << ":"
                    << exception.message();

    return false;
}

void NCLDocumentParser::onEntityAdded(QString , Entity *entity)
{
    lockStack.lock();
    elementStack.push(entity);
    lockStack.unlock();
}

void NCLDocumentParser::onEntityAddError(QString error)
{
    qWarning() << "NCLDocumentParser::onEntityAddError(" << error << ")";
}

} } //end namespace
