#include "PropertiesViewPlugin.h"
#include "core/modules/LanguageControl.h"

namespace composer {
    namespace plugin {
        namespace property {

PropertiesViewPlugin::PropertiesViewPlugin()
{
    window = new PropertyEditor(0);
    doc = NULL;
}

PropertiesViewPlugin::~PropertiesViewPlugin()
{
    delete window;
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
    Entity *entity = doc->getEntityBydId(*id);

    window->setTagname(entity->getType());

    QMap <QString, QString>::iterator begin, end, it;

    entity->getAttributeIterator(begin, end);
    for (it = begin; it != end; ++it)
    {
        window->setAttributeValue(it.key(), it.value());
    }
}

}}} //end namespace
