#include "OutlineViewPlugin.h"

OutlineViewPlugin::OutlineViewPlugin()
{
    window = new NCLTreeWidget(0);
    doc = NULL;

    connect(window, SIGNAL(elementAdded(QString,QString,QMap<QString,QString>&,bool)),
            this, SIGNAL(addEntity(QString,QString,QMap<QString,QString>&,bool)));
}

OutlineViewPlugin::~OutlineViewPlugin()
{
    delete window;
    window = NULL;
}

QWidget* OutlineViewPlugin::getWidget()
{
    return window;
}

void OutlineViewPlugin::onEntityAdded(QString ID, Entity *entity)
{
    QString line = "<" + entity->getType() + "> </" + entity->getType() + ">\n";

    window->addElement(0, 0, entity->getType(), entity->getUniqueId(), 0, 0);
}

void OutlineViewPlugin::onEntityAddError(QString error)
{
    qDebug() << "OutlineViewPlugin::onEntityAddError(" << error << ")";
}

void OutlineViewPlugin::onEntityChanged(QString ID, Entity * entity)
{
    QString line = "PLUGIN (" + ID + ") changed the Entity (" +
                   entity->getType() + " - " + entity->getUniqueId() +")";
    //TODO: All
}

void OutlineViewPlugin::onEntityChangeError(QString error)
{
    //TODO: All
}

void OutlineViewPlugin::onEntityAboutToRemove(Entity *)
{

}

void OutlineViewPlugin::onEntityRemoved(QString ID, QString entityID)
{
    QString line = "PLUGIN (" + ID + ") removed Entity (" +
                   entityID + ")";
    //TODO: All
}

void OutlineViewPlugin::onEntityRemoveError(QString error)
{
    //TODO: All
}

