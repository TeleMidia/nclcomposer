#include "PropertiesViewPlugin.h"
#include "core/modules/LanguageControl.h"

namespace composer {
    namespace plugin {
        namespace property {

PropertiesViewPlugin::PropertiesViewPlugin()
{
    window = new PropertyEditor(0);
    doc = NULL;
    currentEntity = NULL;

    connect(window, SIGNAL(propertyChanged(QString, QString)),
            this, SLOT(updateCurrentEntityAttr(QString, QString)));
}

PropertiesViewPlugin::~PropertiesViewPlugin()
{
    //delete window;
}

QWidget* PropertiesViewPlugin::getWidget()
{
    return window;
}

void PropertiesViewPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
    QString line = "<" + entity->getType() + "> </" + entity->getType() + ">\n";

    qDebug() << line;
}

void PropertiesViewPlugin::errorMessage(QString error)
{
    qDebug() << "PropertiesViewPlugin::onEntityAddError(" << error << ")";
}

void PropertiesViewPlugin::onEntityChanged(QString pluginID, Entity * entity)
{
    QString line = "PLUGIN (" + pluginID + ") changed the Entity (" +
                   entity->getType() + " - " + entity->getUniqueId() +")";

}

void PropertiesViewPlugin::onEntityAboutToRemove(Entity *entity)
{
//    entityToRemoveId = ""
}

void PropertiesViewPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
    QString line = "PLUGIN (" + pluginID + ") removed Entity (" +
                   entityID + ")";

    qDebug() << line;
}

bool PropertiesViewPlugin::saveSubsession()
{
    //TODO: All
    return true;
}

void PropertiesViewPlugin::updateFromModel()
{
    //TODO: All
}

void PropertiesViewPlugin::changeSelectedEntity(void *param){
    QString *id = (QString*)param;
    currentEntity = doc->getEntityBydId(*id);
    if(currentEntity != NULL)
    {
        QString name;
        if( currentEntity->hasAttribute("id") )
            name = currentEntity->getAttribute("id");
        else if(currentEntity->hasAttribute("name"))
            name = currentEntity->getAttribute("name");
        else
            name = "Unknown";

        window->setTagname(currentEntity->getType(), name);

        QMap <QString, QString>::iterator begin, end, it;
        currentEntity->getAttributeIterator(begin, end);
        for (it = begin; it != end; ++it)
        {
            window->setAttributeValue(it.key(), it.value());
        }
    }
}

void PropertiesViewPlugin::updateCurrentEntityAttr(QString attr, QString value)
{
    if (currentEntity != NULL)
    {
        if(currentEntity->hasAttribute("attr") &&
           currentEntity->getAttribute("attr") == value)
        {
            //do nothing
            return;
        }
        else
        {
            QMap <QString, QString> attrs;
            QMap <QString, QString>::iterator begin, end, it;
            currentEntity->getAttributeIterator(begin, end);
            for (it = begin; it != end; ++it)
            {
                attrs.insert(it.key(), it.value());
            }
            attrs.insert(attr, value);

            emit setAttributes(currentEntity, attrs, false);
        }
    }
}

}}} //end namespace
