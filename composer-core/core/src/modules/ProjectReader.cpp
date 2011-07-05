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
        qWarning() << "ERROR: Could not open the file " << location;
        error = true;
    }

    QString content(qUncompress(file.readAll()));
//    qDebug() << content;
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
        qDebug() << "ModelSTR = " << modelStr;
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

//        qDebug() << "pluginData = " << pluginsData;
//        qDebug() << "pluginData size = " << pluginsData.size();
//        qDebug() << "startPluginData " << startPluginData;
        int endStartPluginData =
                pluginsData.indexOf("#\n", startPluginData
                                         + startPluginDataStr.size());
//        qDebug() << "endPluginData " << endStartPluginData;

        QString pluginID = pluginsData.mid(
              startPluginData + startPluginDataStr.size(),
              endStartPluginData- (startPluginData + startPluginDataStr.size())
            );
//        qDebug() << "PluginID = " << pluginID;

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
        qWarning() << "ERROR: File is corrupted " << location;

    file.close();

    qDebug() << project->toString();
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
            qWarning() << "trying to add an entity whithout an uniqueId";
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
    qWarning() << "Fatal error on line" << exception.lineNumber()
                    << ", column" << exception.columnNumber() << ":"
                    << exception.message();
    return false;
}

} } //end namespace
