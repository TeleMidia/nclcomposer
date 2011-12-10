/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "modules/ProjectReader.h"

namespace composer {
    namespace core {

ProjectReader::ProjectReader()
{

}

ProjectReader::~ProjectReader()
{

}

Project *ProjectReader::readFile(QString location)
{
    QFile file(location);
    bool error = false;
    project = NULL;

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "ERROR: Could not open the file " << location;
        error = true;
    }

    QByteArray data = file.readAll();
    QString content(qUncompress(data));
    /* READING MODEL */
    QString startCpModelStr = "#COMPOSER_MODEL#\n";
    QString endCpModelStr = "\n#END_COMPOSER_MODEL#";

    int startCpModel = content.indexOf(startCpModelStr)+startCpModelStr.size();
    int endCpModel = content.indexOf(endCpModelStr);

    project = new Project();
    project->setLocation(location);

    if(content != "")
    {
        QString modelStr = content.mid(startCpModel, endCpModel-startCpModel);
        parseModelString(modelStr);
    }
    /* FINISH READING MODEL */

    /* READING PLUGIN DATA */
    QString pluginsData = content.mid(endCpModel + endCpModelStr.size());
    QString startPluginDataStr = "#COMPOSER_PLUGIN_DATA ";
    QString endPluginDataStr = "\n#END_COMPOSER_PLUGIN_DATA#";
    int pos = 0;

    while(pos >= 0 && pos < pluginsData.size())
    {
        int startPluginData = pluginsData.indexOf(startPluginDataStr, pos);
        int endStartPluginData =
                pluginsData.indexOf("#\n", startPluginData
                                         + startPluginDataStr.size());

        QString pluginID = pluginsData.mid(
              startPluginData + startPluginDataStr.size(),
              endStartPluginData- (startPluginData + startPluginDataStr.size())
            );

        int endPluginData = pluginsData.indexOf( endPluginDataStr,
                                                 startPluginData);

        // +2 and -2 because of the previously #\n
        QString data = pluginsData.mid( endStartPluginData+2,
                                        endPluginData-endStartPluginData-2);

        pos = endPluginData + endPluginDataStr.size() + 1;

        project->setPluginData(pluginID, data.toAscii());
    }
    /* FINISH READING MODEL */

    if(error)
        qDebug() << "ERROR: File is corrupted " << location;

    file.close();

    return project;
}

bool ProjectReader::parseModelString(const QString &str)
{
    QXmlInputSource inputSource;
    inputSource.setData(str);

    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);

    return reader.parse(inputSource);
}

bool ProjectReader::startElement( const QString &namespaceURI,
                                  const QString &localName,
                                  const QString &qName,
                                  const QXmlAttributes &attributes)
{
    QMap<QString,QString> atts;
    QString parentId = project->getUniqueId();
    QString uniqueId = "";

    Entity *parentEntity = NULL;
    if (qName != "document")
    {
        lockStack.lock();
        parentEntity = elementStack.top();
        lockStack.unlock();
    }

    for (int i=0 ;i < attributes.count(); i++)
    {
        if(attributes.qName(i) != "uniqueEntityId")
            atts[attributes.qName(i)] = attributes.value(i);
        else
            uniqueId = attributes.value(i);
    }

    Entity *entity = NULL;
    if(qName != "document" && parentEntity != NULL)
    {
        if(uniqueId == "")
            qDebug() << "trying to add an entity whithout an uniqueId";
        else
        {
            entity = new Entity(uniqueId, atts, project);
            entity->setType(qName);
            project->addEntity(entity, parentEntity->getUniqueId());
        }
    }
    else
        entity = project;

    lockStack.lock();
    elementStack.push(entity);
    lockStack.unlock();
    return true;
}

bool ProjectReader::endElement( const QString &namespaceURI,
                                const QString &localName,
                                const QString &qName)
{
    lockStack.lock();
    if(elementStack.size())
        elementStack.pop();
    lockStack.unlock();

    return true;
}

bool ProjectReader::characters(const QString &str)
{
    return true;
}

bool ProjectReader::fatalError(const QXmlParseException &exception)
{
    qDebug() << "Fatal error on line" << exception.lineNumber()
                    << ", column" << exception.columnNumber() << ":"
                    << exception.message();
    return false;
}

} } //end namespace
